/**********************************************************\

  Auto-generated H3DPluginAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "H3DPluginAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn H3DPluginAPI::H3DPluginAPI(const H3DPluginPtr& plugin, const FB::BrowserHostPtr host)
///
/// @brief  Constructor for your JSAPI object.  You should register your methods, properties, and events
///         that should be accessible to Javascript from here.
///
/// @see FB::JSAPIAuto::registerMethod
/// @see FB::JSAPIAuto::registerProperty
/// @see FB::JSAPIAuto::registerEvent
///////////////////////////////////////////////////////////////////////////////
H3DPluginAPI::H3DPluginAPI(const H3DPluginPtr& plugin, const FB::BrowserHostPtr& host) : m_plugin(plugin), m_host(host)
{
    registerMethod("echo",      make_method(this, &H3DPluginAPI::echo));
    registerMethod("testEvent", make_method(this, &H3DPluginAPI::testEvent));

    registerMethod("loadScene", make_method(this, &H3DPluginAPI::loadScene));

    // Read-write property
    registerProperty("testString",
                     make_property(this,
                        &H3DPluginAPI::get_testString,
                        &H3DPluginAPI::set_testString));

    // Read-only property
    registerProperty("version",
                     make_property(this,
                        &H3DPluginAPI::get_version));
}

///////////////////////////////////////////////////////////////////////////////
/// @fn H3DPluginAPI::~H3DPluginAPI()
///
/// @brief  Destructor.  Remember that this object will not be released until
///         the browser is done with it; this will almost definitely be after
///         the plugin is released.
///////////////////////////////////////////////////////////////////////////////
H3DPluginAPI::~H3DPluginAPI()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @fn H3DPluginPtr H3DPluginAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
H3DPluginPtr H3DPluginAPI::getPlugin()
{
    H3DPluginPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}



// Read/Write property testString
std::string H3DPluginAPI::get_testString()
{
    return m_testString;
}
void H3DPluginAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string H3DPluginAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

// Method echo
FB::variant H3DPluginAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo(msg, n++);
    return msg;
}

void H3DPluginAPI::testEvent(const FB::variant& var)
{
    fire_fired(var, true, 1);
}

void H3DPluginAPI::loadScene ( const std::string& str ) {
  getPlugin()->loadScene ( str );
}