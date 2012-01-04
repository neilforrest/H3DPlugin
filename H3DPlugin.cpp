/**********************************************************\

  Auto-generated H3DPlugin.cpp

  This file contains the auto-generated main plugin object
  implementation for the H3DPlugin project

\**********************************************************/

#include <sstream>
 
#include <stdio.h>

#include "H3DPluginAPI.h"

#include "H3DPlugin.h"

#include "DOM/Window.h"
#include "URI.h"
 
#include "PluginEvents/KeyCodes.h"
 
#ifdef FB_WIN
#include "PluginWindowWin.h"
#include "PluginWindowlessWin.h"
#ifdef HAS_LEAKFINDER
#define XML_LEAK_FINDER
#include "LeakFinder/LeakFinder.h"
#endif
#endif
 
#ifdef HAS_LEAKFINDER
boost::scoped_ptr<LeakFinderXmlOutput> FBTestPlugin::pOut;
#endif

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>

#include "win_common.h"
#include "logging.h"
 
FB::PluginWindowWin* pluginWindowWin;
FB::PluginWindow* pluginWindow;

#include <H3D/MouseSensor.h>

using namespace H3D;
 
LPTHREAD_START_ROUTINE H3DPlugin::drawThreaded( LPVOID lpParam )
{
  H3DPlugin* plugin= static_cast<H3DPlugin*>(lpParam);

  plugin->h3d_mutex.lock();
  plugin->scene.reset ( new Scene );

  plugin->h3dPluginWindow= new H3DPluginWindow ( pluginWindowWin );
  plugin->scene->window->push_back ( plugin->h3dPluginWindow );
  if ( plugin->sceneUrl != "" ) {
    try {
      plugin->scene->loadSceneRoot( plugin->sceneUrl );
    } 
    catch (const Exception::H3DException &e) {
      Console(4) << e << endl;
    }
  }

  // OpenGL is initialized, it's ok for the
  // attach window function to return now
  plugin->h3d_inited= true;
  plugin->h3d_inited_condition.notify_all();
  plugin->h3d_mutex.unlock();

  while ( true ) {
    boost::mutex::scoped_lock lock ( plugin->h3d_mutex );
    try {
      plugin->scene->idle ();
    } 
    catch (const Exception::H3DException &e) {
      Console(4) << e << endl;
    }
  }

  return 0;
}

void H3DPlugin::loadScene ( const std::string& url ) {
  boost::mutex::scoped_lock lock ( h3d_mutex );
  if ( !scene.get() ) {
    sceneUrl= url;
  } else {
    try {
      scene->loadSceneRoot ( url );
    } 
    catch (const Exception::H3DException &e) {
      Console(4) << e << endl;
    }
  }
}

std::streamsize H3DPlugin::ConsoleStreamBuf::xsputn ( const char * s, 
							  std::streamsize n ) {
  //
    m_host->htmlLog ( s );
  return n;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn H3DPlugin::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void H3DPlugin::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn H3DPlugin::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void H3DPlugin::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  H3DPlugin constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
H3DPlugin::H3DPlugin() : 
  h3d_inited ( false ), 
  h3dPluginWindow ( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  H3DPlugin destructor.
///////////////////////////////////////////////////////////////////////////////
H3DPlugin::~H3DPlugin()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void H3DPlugin::onPluginReady()
{
  // When this is called, the BrowserHost is attached, the JSAPI object is
  // created, and we are ready to interact with the page and such.  The
  // PluginWindow may or may not have already fire the AttachedEvent at
  // this point.

  // Attempt to redirect the console to browser's java script console
  // Firefox: See FireBug console for output
  console_stream_buf = new ConsoleStreamBuf( *m_host );
  console_stream.reset( new ostream( console_stream_buf ) );
  H3DUtil::Console.setOutputStream( *console_stream );
}

void H3DPlugin::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<H3DPlugin> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr H3DPlugin::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<H3DPluginAPI>(FB::ptr_cast<H3DPlugin>(shared_from_this()), m_host);
}

bool H3DPlugin::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
  boost::mutex::scoped_lock lock ( h3d_mutex );
  if ( h3dPluginWindow ) {
    h3dPluginWindow->onMouseButtonAction( MouseSensor::LEFT_BUTTON, MouseSensor::DOWN );
  }
  return false;
}

bool H3DPlugin::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
  boost::mutex::scoped_lock lock ( h3d_mutex );
  if ( h3dPluginWindow ) {
    h3dPluginWindow->onMouseButtonAction( MouseSensor::LEFT_BUTTON, MouseSensor::UP );
  }
  return false;
}

bool H3DPlugin::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
  boost::mutex::scoped_lock lock ( h3d_mutex );
  if ( h3dPluginWindow ) {
    h3dPluginWindow->onMouseMotionAction( evt->m_x, evt->m_y );
  }
  Console(4) << "onMouseMove: " << evt->m_x << ", " << evt->m_y << endl;
  return false;
}

bool H3DPlugin::onMouseScroll(FB::MouseScrollEvent *evt, FB::PluginWindow *)
{
  /*boost::mutex::scoped_lock lock ( h3d_mutex );
  if ( h3dPluginWindow ) {
    h3dPluginWindow->onMouseWheelAction(
      MouseSensor::FROM );
  }*/
  Console(4) << "onMouseScroll: " << evt->m_x << ", " << evt->m_y << ", " << evt->m_dx << ", " << evt->m_dy << ", " << endl;
  return false;
}

bool H3DPlugin::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *window )
{
    // The window is attached; act appropriately
    pluginWindow = window;
    pluginWindowWin = dynamic_cast<FB::PluginWindowWin*>(window);
 
    // Start the main H3D thread
    boost::thread t(boost::bind(&H3DPlugin::drawThreaded,  this));
    
    // We need to wait until OpenGL has been initialized by the H3D thread
    // otherwise Firefox will crash on resize of mouse down events
    // See: http://forum.firebreath.org/topic/181/
    boost::mutex::scoped_lock lk(h3d_mutex);
    while ( !h3d_inited ) {
      h3d_inited_condition.wait(lk);
    }

    return false;
}

bool H3DPlugin::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    return false;
}