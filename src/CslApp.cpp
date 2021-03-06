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
#include "engine/CslEngine.h"
#include "CslFrame.h"
#include "CslIPC.h"
#include "CslApp.h"
#ifdef __WXMAC__
#include <Carbon/Carbon.h>
#endif

IMPLEMENT_APP(CslApp)


BEGIN_EVENT_TABLE(CslApp,wxApp)
    EVT_END_SESSION(CslApp::OnEndSession)
END_EVENT_TABLE()


#ifdef __WXMAC__
static pascal OSErr MacCallbackGetUrl(const AppleEvent *in,AppleEvent *out,long ptr)
{
    Size l=0;
    OSErr err=noErr;
    DescType type=typeChar;

    if ((err=AESizeOfParam(in,keyDirectObject,&type,&l))==noErr && l)
    {
        char buf[l+1];

        if ((err=AEGetParamPtr(in,keyDirectObject,type,0,&buf,l,&l))==noErr && l)
        {
            buf[l]=0;
            const CslApp& app=::wxGetApp();
            app.IpcCall(A2U(buf),app.GetTopWindow());
        }
    }

    return noErr;
}
#endif

bool CslApp::OnInit()
{
    m_engine=NULL;
    m_single=NULL;
    m_shutdown=CSL_SHUTDOWN_NONE;

    ::wxSetWorkingDirectory(wxPathOnly(wxTheApp->argv[0]));
    g_basePath=::wxGetCwd();

    m_locale.Init(wxLocale::GetSystemLanguage());
    m_locale.AddCatalogLookupPathPrefix(LOCALEPATH);
#ifdef __WXGTK__
    m_locale.AddCatalogLookupPathPrefix(g_basePath+wxString(wxT("/lang")));
#endif
    if (m_locale.AddCatalog(CSL_NAME_SHORT_STR))
        m_lang=m_locale.GetCanonicalName();

#ifdef __WXMAC__
    wxSystemOptions::SetOption(wxT("mac.listctrl.always_use_generic"),1);
    //enables Command-H, Command-M and Command-Q at least when not in fullscreen
    wxSetEnv(wxT("SDL_SINGLEDISPLAY"),wxT("1"));
    wxSetEnv(wxT("SDL_ENABLEAPPEVENTS"),wxT("1"));
    //TODO wxApp::SetExitOnFrameDelete(false);
    //register event handler for URI schemes
    AEInstallEventHandler(kInternetEventClass,kAEGetURL,
                          NewAEEventHandlerUPP((AEEventHandlerProcPtr)MacCallbackGetUrl),0,false);
#endif

    wxString uri;

    for (wxInt32 i=1;i<wxApp::argc;i++)
    {
        uri=wxApp::argv[i];

        if (!uri.StartsWith(CSL_URI_SCHEME_STR))
            uri.Empty();
    }

    wxString lock=wxString::Format(wxT(".%s-%s.lock"),CSL_NAME_SHORT_STR,wxGetUserId().c_str());
    m_single=new wxSingleInstanceChecker(lock);

    if (m_single->IsAnotherRunning())
    {
        if (uri.IsEmpty())
            uri=wxT("show");

        IpcCall(uri);
        return true;
    }

    m_engine=new CslEngine;

    wxInitAllImageHandlers();

    CslFrame* frame=new CslFrame(NULL,wxID_ANY,wxEmptyString,wxDefaultPosition);
    SetTopWindow(frame);
    frame->Show();

    if (!uri.IsEmpty())
        IpcCall(uri,frame);

    return true;
}

int CslApp::OnRun()
{
    if (GetTopWindow())
        wxApp::OnRun();

    return 0;
}

int CslApp::OnExit()
{
    if (m_engine)
        delete m_engine;

    if (m_single)
        delete m_single;

    return 0;
}

void CslApp::OnEndSession(wxCloseEvent& event)
{
    LOG_DEBUG("\n");

    m_shutdown=CSL_SHUTDOWN_FORCE;

    event.Skip();
}

int CslApp::FilterEvent(wxEvent& event)
{
    if (event.GetEventType()==wxEVT_KEY_DOWN)
        CslToolTip::ResetTip();

    return -1;
}

void CslApp::IpcCall(const wxString& value,wxEvtHandler *evtHandler) const
{
    if (evtHandler)
    {
        CslIpcEvent evt(CslIpcEvent::IPC_COMMAND,value);

        wxPostEvent(evtHandler,evt);
    }
    else
    {
        CslIpcClient client;

        if (client.Connect(CSL_IPC_HOST,CSL_IPC_SERV,CSL_IPC_TOPIC))
#if wxCHECK_VERSION(2,9,0)
            client.GetConnection()->Poke(CSL_NAME_SHORT_STR,value.c_str());
#else
            client.GetConnection()->Poke(CSL_NAME_SHORT_STR,(wxChar*)value.c_str());
#endif

    }
}
