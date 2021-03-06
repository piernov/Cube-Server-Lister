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

#ifndef CSLLISTCTRLSERVER_H
#define CSLLISTCTRLSERVER_H

/**
 @author Glen Masgai <mimosius@users.sourceforge.net>
*/

#include "CslListCtrl.h"


#define CSL_HIGHLIGHT_FOUND_SERVER   1<<0
#define CSL_HIGHLIGHT_FOUND_PLAYER   1<<1
#define CSL_HIGHLIGHT_SEARCH_PLAYER  1<<2
#define CSL_HIGHLIGHT_LOCKED         1<<3


class CslListServerData
{
    public:
        CslListServerData(CslServerInfo *info) :
                Info(info)
        {
            Reset();
        }

        CslListServerData& operator=(const CslServerInfo& info)
        {
            Host=info.Host;
            Description=info.GetBestDescription();
            GameMode=info.GameMode;
            Map=info.Map;
            MMDescription=info.MMDescription;
            MM=info.MM;
            Protocol=info.Protocol;
            Ping=info.Ping;
            TimeRemain=info.TimeRemain;
            Players=info.Players;
            PlayersMax=info.PlayersMax;
            return *this;
        }

        void Reset()
        {
            Host.Empty();
            Description.Empty();
            GameMode.Empty();
            Map.Empty();
            MMDescription.Empty();
            MM=-2;
            Protocol=-1;
            Ping=-1;
            TimeRemain=-2;
            Players=-1;
            PlayersMax=-1;
            ImgId=-1;
            HighLight=0;
        }

        wxInt32 SetHighlight(wxInt32 type,const bool value)
        {
            if (value)
                HighLight|=type;
            else
                HighLight&=~type;

            return HighLight;
        }

        CslServerInfo *Info;
        wxString Host;
        wxString Description;
        wxString GameMode;
        wxString Map;
        wxString MMDescription;
        wxInt32 MM;
        wxInt32 Protocol;
        wxInt32 Ping;
        wxInt32 TimeRemain;
        wxInt32 Players;
        wxInt32 PlayersMax;
        wxInt32 ImgId;
        wxUint32 HighLight;
};

WX_DEFINE_ARRAY_PTR(CslListServerData*,t_aCslListServerData);


class CslListCtrlServer : public CslListCtrl
{
    public:
        enum { CSL_LIST_MASTER, CSL_LIST_FAVOURITE };

        CslListCtrlServer(wxWindow* parent,wxWindowID id,const wxPoint& pos=wxDefaultPosition,
                          const wxSize& size=wxDefaultSize,long style=wxLC_ICON,
                          const wxValidator& validator=wxDefaultValidator,
                          const wxString& name=wxListCtrlNameStr);
        ~CslListCtrlServer();

        void ListInit(CslListCtrlServer *sibling);
        wxUint32 ListUpdate(vector<CslServerInfo*>& servers);
        void ListClear();
        void ListSort(wxInt32 column=-1);
        void ToggleSortArrow();
        wxUint32 ListSearch(const wxString& search);
        wxUint32 ListFilter();
        bool ListUpdateServer(CslServerInfo *info);
        void RemoveServer(CslListServerData *server,CslServerInfo *info,wxInt32 id);
        void ListRemoveServers();
        void ListDeleteServers();
        void ListAdjustSize(const wxSize& size);

        void SetMasterSelected(bool selected) { m_masterSelected=selected; }
        void Highlight(wxInt32 type,bool high,bool sort,CslServerInfo *info=NULL,wxListItem *item=NULL);
        wxUint32 GetPlayerCount();

    private:
        wxInt32 m_id;
        CslGame *m_game;

        bool m_masterSelected;

        CslListCtrlServer *m_sibling;

        bool m_processSelectEvent;
#ifdef __WXMSW__
        bool m_dontAdjustSize;
#endif
        t_aCslListServerData m_selected;
        t_aCslListServerData m_servers;
        wxString m_searchString;
        wxInt32 *m_filterFlags;
        wxInt32 m_filterVersion;

        wxImageList m_imgList;

        CslListSortHelper m_sortHelper;

#ifdef __WXMSW__
        void OnColumnDragStart(wxListEvent& event);
        void OnColumnDragEnd(wxListEvent& event);
#endif
        void OnSize(wxSizeEvent& event);
        void OnKeyDown(wxKeyEvent &event);
        void OnColumnLeftClick(wxListEvent& event);
        void OnItemActivated(wxListEvent& event);
        void OnItemSelected(wxListEvent& event);
        void OnItemDeselected(wxListEvent& event);
        void OnContextMenu(wxContextMenuEvent& event);
        void OnMenu(wxCommandEvent& event);

        static int wxCALLBACK ListSortCompareFunc(long item1,long item2,IntPtr data);

        DECLARE_EVENT_TABLE()

    protected:
        void ListCreateGameBitmaps();
        wxInt32 ListFindItem(CslServerInfo *info,wxListItem& item);
        void ListDeleteItem(wxListItem& item);
        bool ListSearchItemMatches(CslServerInfo *info);
        bool ListFilterItemMatches(CslServerInfo *info);

        void GetToolTipText(wxInt32 row,CslToolTipEvent& event);
        wxSize GetImageListSize()
        {
            wxInt32 x,y;
            if (m_imgList.GetSize(0,x,y))
                return wxSize(x,y);
            return wxDefaultSize;
        }
};

#endif //CSLLISTCTRLSERVER_H
