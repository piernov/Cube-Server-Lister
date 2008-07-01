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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include "engine/CslGame.h"
#include "engine/CslTools.h"


class CslListCtrlPlayer : public wxListCtrl
{
    public:
        enum {CSL_LISTPLAYER_MICRO_SIZE, CSL_LISTPLAYER_MINI_SIZE, CSL_LIST_PLAYER_DEFAULT_SIZE };

        CslListCtrlPlayer(wxWindow* parent,wxWindowID id,const wxPoint& pos=wxDefaultPosition,
                          const wxSize& size=wxDefaultSize,long style=wxLC_ICON,
                          const wxValidator& validator=wxDefaultValidator,
                          const wxString& name=wxListCtrlNameStr);

        void ListInit(const wxUint32 view);
        void ListAdjustSize();
        void ListUpdatePlayerData(CslGame& game,CslPlayerStats& stats);

        void SetInfo(CslServerInfo *info);
        CslServerInfo* GetInfo() { return m_info; }
        wxInt32 GetView() { return m_view; }

        static void CreateImageList();

        static wxSize BestSizeMicro;
        static wxSize BestSizeMini;

    private:
        wxUint32 m_view;
        CslListSortHelper m_sortHelper;

        CslServerInfo *m_info;

        static wxImageList ListImageList;

#ifdef __WXMSW__
        void OnEraseBackground(wxEraseEvent& event);
#endif
        void OnSize(wxSizeEvent& event);
        void OnColumnLeftClick(wxListEvent& event);
        void OnItemActivated(wxListEvent& event);
        void OnContextMenu(wxContextMenuEvent& event);
        void OnMenu(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()

    protected:
        void ListSort(const wxInt32 column);
        void ShowPanelMap(const bool show);

        static int wxCALLBACK ListSortCompareFunc(long item1,long item2,long data);
};

#endif