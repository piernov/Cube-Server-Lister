/***************************************************************************
 *   Copyright (C) 2007-2009 by Glen Masgai                                *
 *   mimosius@users.sourceforge.net                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "Csl.h"
#include "CslUDP.h"

DEFINE_EVENT_TYPE(wxCSL_EVT_PING)

BEGIN_EVENT_TABLE(CslUDP,wxEvtHandler)
    EVT_SOCKET(wxID_ANY,CslUDP::OnSocketEvent)
END_EVENT_TABLE()


wxUint32 CslUDP::m_bytesIn=0,CslUDP::m_bytesOut=0;
wxUint32 CslUDP::m_packetsIn=0,CslUDP::m_packetsOut=0;


CslUDP::CslUDP(wxEvtHandler *evtHandler) : wxEvtHandler(),
        m_evtHandler(evtHandler),m_socket(NULL)
{
    //start of Dynamic Ports (49152 through 65535)
    wxUint16 port=0xc000;

    wxIPV4address address;
    address.AnyAddress();

    while (!m_socket)
    {
        address.Service(port);
        m_socket=new wxDatagramSocket(address,wxSOCKET_NOWAIT);

        if (!m_socket->IsOk())
        {
            delete m_socket;
            m_socket=NULL;

            if (++port==0xffff)
            {
                LOG_DEBUG("Couldn't create socket\n");
                return;
            }
        }
    }

    LOG_DEBUG("Ping port: %li\n",address.Service());

    m_socket->SetEventHandler(*this);
    m_socket->SetNotify(wxSOCKET_INPUT_FLAG);
    m_socket->Notify(true);
}

CslUDP::~CslUDP()
{
    delete m_socket;
}

void CslUDP::OnSocketEvent(wxSocketEvent& event)
{
    if (event.GetSocketEvent()!=wxSOCKET_INPUT)
        return;

    wxIPV4address addr;
    wxUint32 size;
    CslUDPPacket *packet=new CslUDPPacket(CSL_MAX_PACKET_SIZE);

    m_socket->RecvFrom(addr,packet->Data(),CSL_MAX_PACKET_SIZE);

    if (m_socket->Error())
    {
        packet->FreeData();
#ifndef __WXMSW__
        LOG_DEBUG("Error receiving packet: %s\n",U2A(GetSocketError(m_socket->LastError())));
#endif
    }

    size=m_socket->LastCount();
    packet->SetAddr(addr);
    packet->SetSize(size);
    m_bytesIn+=size;
    m_packetsIn++;

    wxCommandEvent evt(wxCSL_EVT_PING);
    evt.SetClientData(packet);
    wxPostEvent(m_evtHandler,evt);
}

bool CslUDP::SendPing(CslUDPPacket *packet)
{
    wxInt32 size=packet->Size();

    m_socket->SendTo(packet->Address(),packet->Data(),size);

    packet->SetSize(0);
    delete packet;

    if (m_socket->Error())
    {
        LOG_DEBUG("Error sending packet: %s\n",U2A(GetSocketError(m_socket->LastError())));
        return false;
    }

    m_bytesOut+=size;
    m_packetsOut++;

    return true;
}

wxUint32 CslUDP::GetPacketCount(wxUint32 type)
{
    switch (type)
    {
        case CSL_UDP_TRAFFIC_IN:
            return m_packetsIn;
        case CSL_UDP_TRAFFIC_OUT:
            return m_packetsOut;
    }
    return 0;
}

wxUint32 CslUDP::GetTraffic(wxUint32 type,bool overhead)
{
    wxUint32 bytes,packets;

    switch (type)
    {
        case CSL_UDP_TRAFFIC_IN:
            bytes=m_bytesIn;
            packets=m_packetsIn;
            break;
        case CSL_UDP_TRAFFIC_OUT:
            bytes=m_bytesOut;
            packets=m_packetsOut;
            break;
        default:
            return 0;
    }

    return bytes+packets*(overhead ? CSL_UDP_OVERHEAD:0);
}

const wxString CslUDP::GetSocketError(wxInt32 code) const
{
    switch (code)
    {
        case wxSOCKET_NOERROR:
            return wxT("No error happened");
        case wxSOCKET_INVOP:
            return wxT("Invalid operation");
        case wxSOCKET_IOERR:
            return wxT("Input/Output error");
        case wxSOCKET_INVADDR:
            return wxT("Invalid address passed to wxSocket");
        case wxSOCKET_INVSOCK:
            return wxT("Invalid socket(uninitialized)");
        case wxSOCKET_NOHOST:
            return wxT("No corresponding host");
        case wxSOCKET_INVPORT:
            return wxT("Invalid port");
        case wxSOCKET_WOULDBLOCK:
            return wxT("The socket is non-blocking and the operation would block");
        case wxSOCKET_TIMEDOUT:
            return wxT("The timeout for this operation expired");
        case wxSOCKET_MEMERR:
            return wxT("Memory exhausted");
        default:
            break;
    }

    return wxEmptyString;
}
