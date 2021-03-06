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

#ifndef CSLDLGSETTINGS_H
#define CSLDLGSETTINGS_H

/**
 @author Glen Masgai <mimosius@users.sourceforge.net>
*/

#include "CslSettings.h"


class CslGamePage : public wxPanel
{
    public:
        CslGamePage(wxWindow *parent,CslGame* game);
        ~CslGamePage();

        bool SaveSettings(wxString *message);

    private:
        void OnPicker(wxFileDirPickerEvent& event);
        void OnCommandEvent(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText *label_exe,*label_gamepath,*label_cfgpath;
        wxStaticText *label_pre_script,*label_post_script,*label_expert;
        wxTextCtrl *text_ctrl_options,*text_ctrl_pre_script,*text_ctrl_post_script;
        wxFilePickerCtrl *filepicker;
        wxDirPickerCtrl *dirpickergame,*dirpickercfg;

        CslGame *m_game;
};

class CslDlgSettings : public wxDialog
{
    public:
        // begin wxGlade: CslDlgSettings::ids
        // end wxGlade
        CslDlgSettings(wxWindow* parent,int id=wxID_ANY,
                       const wxString& title=wxEmptyString,const wxPoint& pos=wxDefaultPosition,
                       const wxSize& size=wxDefaultSize,long style=wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

    private:
        // begin wxGlade: CslDlgSettings::methods
        void set_properties();
        void do_layout();
        // end wxGlade

        CslSettings m_settings;

        void OnSpinCtrl(wxSpinEvent& event);
        void OnPicker(wxFileDirPickerEvent& event);
        void OnCommandEvent(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()

    protected:
        // begin wxGlade: CslDlgSettings::attributes
        wxStaticBox* sizer_3_staticbox;
        wxStaticBox* sizer_2_staticbox;
        wxStaticBox* sizer_1_staticbox;
        wxStaticBox* sizer_network_staticbox;
        wxStaticBox* sizer_output_staticbox;
        wxStaticBox* sizer_tts_staticbox;
        wxStaticBox* sizer_systray_staticbox;
        wxStaticBox* sizer_threshold_staticbox;
        wxStaticBox* sizer_times_staticbox;
        wxStaticBox* sizer_colours_staticbox;
        wxListbook* notebook_games;
        wxPanel* notebook_pane_games;
        wxBitmapButton* button_colour_empty;
        wxBitmapButton* button_colour_mm1;
        wxBitmapButton* button_colour_off;
        wxBitmapButton* button_colour_mm2;
        wxBitmapButton* button_colour_full;
        wxBitmapButton* button_colour_mm3;
        wxPanel* notebook_pane_colour;
        wxSpinCtrl* spin_ctrl_update;
        wxCheckBox* checkbox_play_update;
        wxSpinCtrl* spin_ctrl_wait;
        wxSpinCtrl* spin_ctrl_min_playtime;
        wxSpinCtrl* spin_ctrl_server_cleanup;
        wxCheckBox* checkbox_server_cleanup_favourites;
        wxCheckBox* checkbox_server_cleanup_stats;
        wxSpinCtrl* spin_ctrl_tooltip_delay;
        wxSpinCtrl* spin_ctrl_ping_good;
        wxSpinCtrl* spin_ctrl_ping_bad;
        wxCheckBox* checkbox_systray;
        wxCheckBox* checkbox_systray_close;
        wxCheckBox* checkbox_tts;
        wxSpinCtrl* spin_ctrl_tts_volume;
        wxCheckBox* checkbox_game_output;
        wxDirPickerCtrl* dirpicker_game_output;
        wxPanel* notebook_pane_other;
        wxTreeCtrl* tree_ctrl_network;
        wxStaticText* label_7;
        wxTextCtrl* text_ctrl_1;
        wxStaticText* label_8;
        wxSpinCtrl* spin_ctrl_1;
        wxStaticText* label_9;
        wxTextCtrl* text_ctrl_2;
        wxStaticText* label_10;
        wxCheckBox* checkbox_1;
        wxStaticText* label_11;
        wxTextCtrl* text_ctrl_3;
        wxStaticText* label_12;
        wxTextCtrl* text_ctrl_4;
        wxListBox* list_box_1;
        wxButton* button_2;
        wxButton* button_3;
        wxListBox* list_box_2;
        wxButton* button_4;
        wxButton* button_5;
        wxPanel* notebook_pane_irc;
        wxNotebook* notebook_settings;
        wxButton* button_ok;
        wxButton* button_cancel;
        // end wxGlade

        void SetButtonColour(wxBitmapButton *button,wxButton *refButton,wxColour& colour);
        bool ValidateSettings();
}; // wxGlade: end class


#endif // CSLDLGSETTINGS_H
