/***************************************************************************
 *   Copyright (C) 2007 by Glen Masgai                                     *
 *   mimosius@gmx.de                                                       *
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

#ifndef CSLLISTCTRLPLAYER_H
#define CSLLISTCTRLPLAYER_H

/**
    @author Glen Masgai <mimosius@gmx.de>
*/

#include "CslListCtrl.h"
#include "engine/CslGame.h"
#include "engine/CslTools.h"


class CslListCtrlPlayer : public CslListCtrl
{
    public:
        enum
        {
            CSL_LISTPLAYER_MICRO_SIZE, CSL_LISTPLAYER_MINI_SIZE,
            CSL_LIST_PLAYER_DEFAULT_SIZE, CSL_LIST_PLAYER_DEFAULT_SIZE_DLG
        };

        CslListCtrlPlayer(wxWindow* parent,wxWindowID id,const wxPoint& pos=wxDefaultPosition,
                          const wxSize& size=wxDefaultSize,long style=wxLC_ICON,
                          const wxValidator& validator=wxDefaultValidator,
                          const wxString& name=wxListCtrlNameStr);

        void ListInit(const wxInt32 view);
        void ListAdjustSize(const wxSize& size=wxDefaultSize);
        void UpdateData();

        void ServerInfo(CslServerInfo *info);
        CslServerInfo* ServerInfo() { return m_info; }
        wxInt32 View() { return m_view; }

        static wxSize BestSizeMicro;
        static wxSize BestSizeMini;

    private:
        wxInt32 m_view;
        CslServerInfo *m_info;

        CslListSortHelper m_sortHelper;
#ifdef __WXMSW__
        static wxInt32 m_imgOffsetY;

        void OnEraseBackground(wxEraseEvent& event);
#endif
        void OnColumnLeftClick(wxListEvent& event);
        void OnItemActivated(wxListEvent& event);
        void OnContextMenu(wxContextMenuEvent& event);
        void OnMenu(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()

    protected:
        void ListSort(const wxInt32 column);

        void GetToolTipText(wxInt32 row,wxString& title,wxArrayString& text);

        static int wxCALLBACK ListSortCompareFunc(long item1,long item2,long data);
};

#endif
