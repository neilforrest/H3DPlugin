/**********************************************************\

  Auto-generated H3DPluginAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "H3DPlugin.h"

#ifndef H_H3DPluginAPI
#define H_H3DPluginAPI

class H3DPluginAPI : public FB::JSAPIAuto
{
public:
    H3DPluginAPI(const H3DPluginPtr& plugin, const FB::BrowserHostPtr& host);
    virtual ~H3DPluginAPI();

    H3DPluginPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    std::string get_data();
    void set_data(const std::string& val);

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(fired, 3, (const FB::variant&, bool, int));
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));
    FB_JSAPI_EVENT(notify, 0, ());

    // Method test-event
    void testEvent(const FB::variant& s);

    void loadScene ( const std::string& url );

private:
    H3DPluginWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
    std::string m_data;
};

#endif // H_H3DPluginAPI

