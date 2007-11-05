
#include "CslArt.h"

#ifndef __WXMSW__
#include "img/connect_16.xpm"
#include "img/connect_pw_16.xpm"
#include "img/about_16.xpm"
#if defined(__WXMSW__) || defined(__WXMAC__)
#include "img/add_16.xpm"
#include "img/delete_16.xpm"
#include "img/reload_16.xpm"
#include "img/settings_16.xpm"
#endif // __WXMSW__ || __WXMAC__
#endif // __WXMSW__

wxBitmap CslArt::CreateBitmap(const wxArtID& id,const wxArtClient& client,const wxSize& size)
{
    if (id!=wxART_NONE)
    {
        if (client==wxART_MENU)
        {
            if (id==wxART_CONNECT)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("CONNECT_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(connect_16_xpm);
#endif
            else if (id==wxART_CONNECT_PW)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("CONNECT_PW_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(connect_pw_16_xpm);
#endif
            else if (id==wxART_ABOUT)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("ABOUT_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(about_16_xpm);
#endif

#if defined(__WXMSW__) || defined(__WXMAC__)
            else if (id==wxART_ADD_BOOKMARK)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("ADD_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(add_16_xpm);
#endif

            else if (id==wxART_DEL_BOOKMARK)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("DEL_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(delete_16_xpm);
#endif

            else if (id==wxART_RELOAD)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("RELOAD_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(reload_16_xpm);
#endif
            else if (id==wxART_SETTINGS)
#ifdef __WXMSW__
                return wxBitmap(wxIcon(wxT("SETTINGS_16"),wxBITMAP_TYPE_ICO_RESOURCE,16,16));
#else
                return wxBitmap(settings_16_xpm);
#endif
#endif //__WXMSW__ || __WXMAC__
        }
    }
    return wxNullBitmap;
}

wxBitmap CslArt::GetMenuBitmap(const wxArtID& id)
{
    return wxArtProvider::GetBitmap(id,wxART_MENU);
}
