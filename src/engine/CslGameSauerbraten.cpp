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
#include "CslEngine.h"
#include "CslGameSauerbraten.h"

#include "../img/sb_16_png.h"
#include "../img/sb_24_png.h"


enum { MM_AUTH = -1, MM_OPEN, MM_VETO, MM_LOCKED, MM_PRIVATE, MM_PASSWORD };
enum { CS_ALIVE, CS_DEAD, CS_SPAWNING, CS_LAGGED, CS_EDITING, CS_SPECTATOR };

CslGameSauerbraten::CslGameSauerbraten()
{
    m_name=CSL_DEFAULT_NAME_SB;
    m_defaultMasterConnection=CslMasterConnection(CSL_DEFAULT_MASTER_SB,CSL_DEFAULT_MASTER_PORT_SB);
    m_capabilities=CSL_CAPABILITY_EXTINFO | CSL_CAPABILITY_CUSTOM_CONFIG |
                   CSL_CAPABILITY_CONNECT_PASS | CSL_CAPABILITY_CONNECT_ADMIN_PASS;
#ifdef __WXMAC__
    m_clientSettings.ConfigPath=::wxGetHomeDir();
    m_clientSettings.ConfigPath<<wxT("/Library/Application Support/sauerbraten");
#elif __WXGTK__
    m_clientSettings.ConfigPath=::wxGetHomeDir()+wxT("/.sauerbraten");
#endif
    m_icon16=BitmapFromData(wxBITMAP_TYPE_PNG,sb_16_png,sizeof(sb_16_png));
    m_icon24=BitmapFromData(wxBITMAP_TYPE_PNG,sb_24_png,sizeof(sb_24_png));
}

CslGameSauerbraten::~CslGameSauerbraten()
{
}

const wxChar* CslGameSauerbraten::GetVersionName(wxInt32 prot) const
{
    static const wxChar* versions[] =
    {
		wxT("Collect"),wxT("Justice"),wxT("Trooper"),wxT("CTF"),wxT("Assassin"),wxT("Summer"),
        wxT("Spring"),wxT("Gui"),wxT("Water"),wxT("Normalmap"),
        wxT("Sp"),wxT("Occlusion"),wxT("Shader"),wxT("Physics"),
        wxT("Mp"),wxT(""),wxT("Agc"),wxT("Quakecon"),wxT("Independence")
    };

    wxInt32 v=CSL_LAST_PROTOCOL_SB-prot;

    if (v<0 || (size_t)v>=sizeof(versions)/sizeof(versions[0]))
    {
        static wxString version=wxString::Format(wxT("%d"),prot);
        return version.c_str();
    }
    else
        return versions[v];
}

const wxChar* CslGameSauerbraten::GetModeName(wxInt32 mode,wxInt32 prot) const
{
    if (prot<257)
    {
        static const wxChar* modes[] =
        {
            wxT("ffa/default"),wxT("coopedit"),wxT("ffa/duel"),wxT("teamplay"),
            wxT("instagib"),wxT("instagib team"),wxT("efficiency"),wxT("efficiency team"),
            wxT("insta arena"),wxT("insta clan arena"),wxT("tactics arena"),wxT("tactics clan arena"),
            wxT("capture"),wxT("insta capture"),wxT("regen capture"),wxT("assassin"),
            wxT("insta assassin"),wxT("ctf"),wxT("insta ctf")
        };
        return (mode>=0 && (size_t)mode<sizeof(modes)/sizeof(modes[0])) ?
               modes[mode] : T2C(_("unknown"));
    }
    else
    {
        static const wxChar* modes[] =
        {
            wxT("ffa"),wxT("coop edit"),wxT("teamplay"),wxT("instagib"),wxT("instagib team"),
            wxT("efficiency"),wxT("efficiency team"),wxT("tactics"),wxT("tactics team"),
            wxT("capture"),wxT("regen capture"),wxT("ctf"),wxT("insta ctf"),
            wxT("protect"),wxT("insta protect"),wxT("hold"),wxT("insta hold"),wxT("efficiency ctf"),wxT("efficiency protect"),wxT("efficiency hold"),
            wxT("collect"),wxT("insta collect"),wxT("efficiency collect")
        };
        return (mode>=0 && (size_t)mode<sizeof(modes)/sizeof(modes[0])) ?
               modes[mode] : T2C(_("unknown"));
    }

    return _("unknown");
}

const wxChar* CslGameSauerbraten::GetWeaponName(wxInt32 n) const
{
    static const wxChar* weapons[] =
    {
        _("Fist"),_("Shotgun"),_("Chaingun"),_("Rocketlauncher"),
        _("Rifle"),_("Grenadelauncher"),_("Pistol"),_("Fireball"),
        _("Iceball"),_("Slimeball"),_("Bite")
    };
    return (n>=0 && (size_t)n<sizeof(weapons)/sizeof(weapons[0])) ?
           weapons[n] : T2C(_("unknown"));
}

bool CslGameSauerbraten::ModeHasFlags(wxInt32 mode,wxInt32 prot) const
{
    if (prot<257)
        return mode>16 && mode<19;
    else
        return mode>10 && mode<15;

    return false;
}

bool CslGameSauerbraten::ModeHasBases(wxInt32 mode,wxInt32 prot) const
{
    if (prot<257)
        return mode>11 && mode<15;
    else
        return mode>8 && mode<11;

    return false;
}

wxInt32 CslGameSauerbraten::ModeScoreLimit(wxInt32 mode,wxInt32 prot) const
{
    if (ModeHasBases(mode,prot))
        return 10000;
    if (ModeHasFlags(mode,prot))
        return 10;

    return -1;
}

wxInt32 CslGameSauerbraten::GetBestTeam(CslTeamStats& stats,wxInt32 prot) const
{
    wxInt32 i,best=-1;

    if (stats.TeamMode && stats.m_stats.length()>0 && stats.m_stats[0]->Ok)
    {
        best=0;

        for (i=1;i<stats.m_stats.length() && stats.m_stats[i]->Ok;i++)
        {
            if (stats.m_stats[i]->Score>=stats.m_stats[best]->Score)
                best=i;
        }
    }

    return best;
}

bool CslGameSauerbraten::ParseDefaultPong(ucharbuf& buf,CslServerInfo& info) const
{
    vector<int>attr;
    wxUint32 l,numattr;
    char text[_MAXDEFSTR];
    bool wasfull=info.IsFull();

    l=getint(buf);
    if (info.HasRegisteredEvent(CslServerEvents::EVENT_EMPTY) && info.Players>0 && !l)
        info.SetEvents(CslServerEvents::EVENT_EMPTY);
    else if (info.HasRegisteredEvent(CslServerEvents::EVENT_NOT_EMPTY) && !info.Players && l>0)
        info.SetEvents(CslServerEvents::EVENT_NOT_EMPTY);
    info.Players=l;

    numattr=getint(buf);
    loopj(numattr) attr.add(getint(buf));

    if (numattr>=1)
    {
        info.Protocol=attr[0];
        info.Version=GetVersionName(info.Protocol);
    }
    if (numattr>=2)
        info.GameMode=GetModeName(attr[1],info.Protocol);
    if (numattr>=3)
    {
        info.TimeRemain=attr[2];
		if (info.Protocol >= 258) {
			int time = info.TimeRemain;
			info.TimeRemain = info.TimeRemain/60;
			if (time > 0)
					info.TimeRemain++;
		}
        if (info.Protocol<254)
            info.TimeRemain++;
    }
    if (numattr>=4)
    {
        info.PlayersMax=attr[3];

        if (info.HasRegisteredEvent(CslServerEvents::EVENT_FULL) && !wasfull && info.IsFull())
            info.SetEvents(CslServerEvents::EVENT_FULL);
        else if (info.HasRegisteredEvent(CslServerEvents::EVENT_NOT_FULL) && wasfull && !info.IsFull())
            info.SetEvents(CslServerEvents::EVENT_NOT_FULL);
    }

    l=info.MM;
    info.MM=CSL_SERVER_OPEN;
    info.MMDescription.Empty();

    if (numattr>=5)
    {
        if (attr[4]==MM_AUTH)
        {
            info.MMDescription<<wxT("0 (O/AUTH)");
            info.MM=MM_AUTH;
        }
        else if (attr[4]==MM_PASSWORD)
        {
            info.MMDescription<<wxT("PASS");
            info.MM|=CSL_SERVER_PASSWORD;
        }
        else
        {
            info.MMDescription=wxString::Format(wxT("%d"),attr[4]);

            if (attr[4]==MM_OPEN)
                info.MMDescription<<wxT(" (O)");
            else if (attr[4]==MM_VETO)
            {
                info.MMDescription<<wxT(" (V)");
                info.MM=CSL_SERVER_VETO;
            }
            else if (attr[4]==MM_LOCKED)
            {
                if (info.HasRegisteredEvent(CslServerEvents::EVENT_LOCKED) &&
                    CSL_MM_IS_VALID(l) && !CSL_SERVER_IS_LOCKED(l))
                    info.SetEvents(CslServerEvents::EVENT_LOCKED);
                info.MMDescription<<wxT(" (L)");
                info.MM=CSL_SERVER_LOCKED;

            }
            else if (attr[4]==MM_PRIVATE)
            {
                if (info.HasRegisteredEvent(CslServerEvents::EVENT_PRIVATE) &&
                    CSL_MM_IS_VALID(l) && !CSL_SERVER_IS_PRIVATE(l))
                    info.SetEvents(CslServerEvents::EVENT_PRIVATE);
                info.MMDescription<<wxT(" (P)");
                info.MM=CSL_SERVER_PRIVATE;
            }
        }
    }

    getstring(text,buf);
    info.Map=A2U(text);
    getstring(text,buf);
    l=(wxInt32)strlen(text);
    FixString(text,&l,1);
    info.SetDescription(A2U(text));

    return !buf.overread();
}

bool CslGameSauerbraten::ParsePlayerPong(wxUint32 protocol,ucharbuf& buf,CslPlayerStatsData& info) const
{
    char text[_MAXDEFSTR];

    info.ID=getint(buf);
    if (protocol>=104)
        info.Ping=getint(buf);
    getstring(text,buf);
    info.Name=A2U(text);
    getstring(text,buf);
    info.Team=A2U(text);
    info.Frags=getint(buf);
    if (protocol>=104)
        info.Flagscore=getint(buf);
    info.Deaths=getint(buf);
    info.Teamkills=getint(buf);
    if (protocol>=103)
        info.Accuracy=getint(buf);
    info.Health=getint(buf);
    info.Armour=getint(buf);
    info.Weapon=getint(buf);
    info.Privileges=getint(buf);
    switch (getint(buf))
    {
        case CS_ALIVE:     info.State=CSL_PLAYER_STATE_ALIVE; break;
        case CS_DEAD:      info.State=CSL_PLAYER_STATE_DEAD; break;
        case CS_SPAWNING:  info.State=CSL_PLAYER_STATE_SPAWNING; break;
        case CS_LAGGED:    info.State=CSL_PLAYER_STATE_LAGGED; break;
        case CS_EDITING:   info.State=CSL_PLAYER_STATE_EDITING; break;
        case CS_SPECTATOR: info.State=CSL_PLAYER_STATE_SPECTATOR; break;
        default:           info.State=CSL_PLAYER_STATE_UNKNOWN; break;
    }

    return !buf.overread();
}

bool CslGameSauerbraten::ParseTeamPong(wxUint32 protocol,ucharbuf& buf,CslTeamStatsData& info) const
{
    wxInt32 i;
    char text[_MAXDEFSTR];

    getstring(text,buf);
    info.Name=A2U(text);
    info.Score=getint(buf);
    i=getint(buf);
    if (i>0)
        while (i--)
            info.Bases.add(getint(buf));

    return !buf.overread();
}

void CslGameSauerbraten::SetClientSettings(const CslGameClientSettings& settings)
{
    CslGameClientSettings set=settings;

    if (set.GamePath.IsEmpty() || !::wxDirExists(set.GamePath))
        return;
    if (set.ConfigPath.IsEmpty() || !::wxDirExists(set.ConfigPath))
        set.ConfigPath=set.GamePath;
#ifdef __WXMAC__
    if (set.Binary.IsEmpty())
        set.Binary=set.GamePath+wxT("sauerbraten.app/Contents/MacOS/sauerbraten");
    if (set.Options.IsEmpty())
        set.Options=wxT("-r");
#endif
    if (set.Binary.IsEmpty() || !::wxFileExists(set.Binary))
        return;

    m_clientSettings=set;
}

wxString CslGameSauerbraten::GameStart(CslServerInfo *info,wxUint32 mode,wxString& error)
{
    wxString address,password,path,script;
    wxString bin=m_clientSettings.Binary;
    wxString opts=m_clientSettings.Options;
    wxString preScript=m_clientSettings.PreScript;
    wxString postScript=m_clientSettings.PostScript;

    if (m_clientSettings.Binary.IsEmpty() || !::wxFileExists(m_clientSettings.Binary))
    {
        error=_("Client binary for game Sauerbraten not found!\nCheck your settings.");
        return wxEmptyString;
    }
    if (m_clientSettings.GamePath.IsEmpty() || !::wxDirExists(m_clientSettings.GamePath))
    {
        error=_("Game path for game Sauerbraten not found!\nCheck your settings.");
        return wxEmptyString;
    }
    if (m_clientSettings.ConfigPath.IsEmpty() || !::wxDirExists(m_clientSettings.ConfigPath))
    {
        error=_("Config path for game Sauerbraten not found!\nCheck your settings.");
        return wxEmptyString;
    }

    path=m_clientSettings.ConfigPath;
#ifdef __WXMSW__
    //binary must be surrounded by quotes if the path contains spaces
    bin=wxT("\"")+m_clientSettings.Binary+wxT("\"");
    // use Prepend() and do not use opts+= here, since -q<path> must be before -r
    opts.Prepend(wxT("\"-q")+path.RemoveLast()+wxT("\" "));
#else
    CmdlineEscapeSpaces(bin);
    CmdlineEscapeSpaces(path);
    // use Prepend() and do not use opts+= here, since -q<path> must be before -r
    opts.Prepend(wxT("-q")+path+wxT(" "));
#endif //__WXMSW__

    ProcessScript(*info,mode,preScript);
    ProcessScript(*info,mode,postScript);

    if (!preScript.IsEmpty())
    {
        preScript<<wxT(";");
        CmdlineEscapeQuotes(preScript);
    }
    if (!postScript.IsEmpty())
    {
        postScript.Prepend(wxT(";"));
        CmdlineEscapeQuotes(postScript);
    }

    address=info->Host;

    password<<wxT("\"");
    password<<(mode==CslServerInfo::CSL_CONNECT_PASS ? info->Password :
               mode==CslServerInfo::CSL_CONNECT_ADMIN_PASS ?
               info->PasswordAdmin : wxString(wxEmptyString));
    password<<wxT("\"");

    CmdlineEscapeQuotes(password);

    if (info->GamePort!=GetDefaultGamePort() || !password.IsEmpty())
        address<<wxString::Format(wxT(" %d"),info->GamePort);

    script=wxString::Format(wxT("%sconnect %s %s%s"),
                            preScript.c_str(),
                            address.c_str(),password.c_str(),
                            postScript.c_str());
#ifdef __WXMSW__
    opts<<wxT(" \"-x")<<script<<wxT("\"");
#else
    opts<<wxT(" -x")<<CmdlineEscapeSpaces(script);
#endif

    bin<<wxT(" ")<<opts;

    LOG_DEBUG("start client: %s\n",U2A(bin));

    return bin;
}

wxInt32 CslGameSauerbraten::GameEnd(wxString& error)
{
    return CSL_ERROR_NONE;
}


bool CslGameSauerbraten::GetMapImagePaths(wxArrayString& paths) const
{
    wxInt32 pos;
    wxString path;

    if (!m_clientSettings.GamePath.IsEmpty())
    {
        path<<m_clientSettings.GamePath<<wxT("packages")<<PATHDIV<<wxT("base")<<PATHDIV;
        paths.Add(path);
    }

    //TODO look for extra package directories
    if ((pos=m_clientSettings.Options.Find(wxT("-k")))!=wxNOT_FOUND)
    {
    }

    return !paths.IsEmpty();
}
