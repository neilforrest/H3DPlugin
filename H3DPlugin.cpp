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

#include <H3D/MouseSensor.h>

using namespace H3D;

H3DPlugin::StaticState H3DPlugin::staticState;

void H3DPlugin::drawThreaded() {
  ThreadBase::setMainThreadId ( ThreadBase::getCurrentThreadId() );

  while ( H3DPlugin::idle() ) {
    // Give event callbacks a chance to get the mutex
    boost::this_thread::sleep(boost::posix_time::milliseconds(1));
  }
}

void H3DPlugin::addH3DThreadCallback ( CallbackFunc callback, void* data ) {
  boost::mutex::scoped_lock lock ( staticState.h3d_mutex );
  staticState.callbacks.push_back ( make_pair<CallbackFunc, void*>(callback, data) );
}

void H3DPlugin::addH3DThreadCallbackBlocking ( CallbackFunc callback, void* data ) {
  addH3DThreadCallback ( callback, data );
  
  boost::mutex::scoped_lock lk(staticState.h3d_mutex);
  staticState.callbacks_executed= false;

  while ( !staticState.callbacks_executed ) {
    staticState.callbacks_executed_condition.wait(lk);
  }
}

void H3DPlugin::loadSceneCallback ( void* data ) {
  H3DPlugin* plugin= static_cast<H3DPlugin*>(data);
  if ( plugin->scene.get() ) {
    plugin->scene->loadSceneRoot ( plugin->sceneUrl );
  }
}

void H3DPlugin::loadScene ( const std::string& url ) {
  staticState.h3d_mutex.lock();
  sceneUrl= url;
  staticState.h3d_mutex.unlock();

  addH3DThreadCallbackBlocking ( loadSceneCallback, this );
}

std::streamsize H3DPlugin::ConsoleStreamBuf::xsputn ( const char * s, 
							  std::streamsize n ) {
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

  // Start the main H3D thread
  staticState.h3d_thread= boost::thread(&H3DPlugin::drawThreaded);
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

  staticState.h3d_mutex.lock();
  staticState.h3d_finished= true;
  staticState.h3d_mutex.unlock();
  staticState.h3d_thread.join();
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  H3DPlugin constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
H3DPlugin::H3DPlugin() : 
  h3dPluginWindow ( NULL ),
  pluginWindowWin ( NULL )
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

  //// Redirect the console to browser's java script console
  //// Firefox: See FireBug console for output
  //console_stream_buf = new ConsoleStreamBuf( *m_host );
  //console_stream.reset( new ostream( console_stream_buf ) );
  //H3DUtil::Console.setOutputStream( *console_stream );

  // Load the requested scene
  if ( m_params.find ( "data" ) != m_params.end() ) {

    std::string url= m_host->getDOMWindow()->getLocation();
    FB::URI loc = FB::URI::fromString(url);
    url= url.substr( 0, url.size()-loc.filename().size() );
    loadScene ( url + m_params["data"].cast<std::string>() );
  }
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

bool H3DPlugin::idle () {
  boost::mutex::scoped_lock lock ( staticState.h3d_mutex );

  /// TODO: There are issues when using multiple scenes
  /// * Flickering in Firefox
  /// * Background (white)
  /// * Navigation
  if ( staticState.count <= 1 ) {
    for( set< Scene * >::iterator i = Scene::scenes.begin();
      i != Scene::scenes.end();
      i++ ) {
      if( (*i)->isActive() ) {
        try {
          (*i)->idle();
        } catch (const Exception::H3DException &e) {
          Console(4) << e << endl;
        }
      }
    }
  }

  for ( CallbackList::iterator i= staticState.callbacks.begin(); i != staticState.callbacks.end(); ++i ) {
    (*(*i).first) ( (*i).second );
  }
  staticState.callbacks.clear();

  staticState.callbacks_executed= true;
  staticState.callbacks_executed_condition.notify_all();

  return !staticState.h3d_finished;
}

void H3DPlugin::onMouseDownCallback ( void* data ) {
  EventData* d= static_cast<EventData*>(data);
  if ( d->plugin->h3dPluginWindow ) {
    d->plugin->h3dPluginWindow->onMouseButtonAction( MouseSensor::LEFT_BUTTON, MouseSensor::DOWN );
  }
}

bool H3DPlugin::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
  EventData d ( this, evt );
  addH3DThreadCallbackBlocking ( onMouseDownCallback, &d );
  return false;
}

void H3DPlugin::onMouseUpCallback ( void* data ) {
  EventData* d= static_cast<EventData*>(data);
  if ( d->plugin->h3dPluginWindow ) {
    d->plugin->h3dPluginWindow->onMouseButtonAction( MouseSensor::LEFT_BUTTON, MouseSensor::UP );
  }
}

bool H3DPlugin::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
  EventData d ( this, evt );
  addH3DThreadCallbackBlocking ( onMouseUpCallback, &d );
  return false;
}

void H3DPlugin::onMouseMoveCallback ( void* data ) {
  EventData* d= static_cast<EventData*>(data);
  FB::MouseMoveEvent* evt= static_cast<FB::MouseMoveEvent*>(d->event);

  if ( d->plugin->h3dPluginWindow ) {
    d->plugin->h3dPluginWindow->onMouseMotionAction( evt->m_x, evt->m_y );
  }
}

bool H3DPlugin::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
  EventData d ( this, evt );
  addH3DThreadCallbackBlocking ( onMouseMoveCallback, &d );
  return false;
}

void H3DPlugin::onMouseScrollCallback ( void* data ) {
  EventData* d= static_cast<EventData*>(data);
  FB::MouseScrollEvent* evt= static_cast<FB::MouseScrollEvent*>(d->event);

  if ( d->plugin->h3dPluginWindow ) {
    if ( evt->m_dy > 0 ) {
      d->plugin->h3dPluginWindow->onMouseWheelAction(
        MouseSensor::FROM );
    } else {
      d->plugin->h3dPluginWindow->onMouseWheelAction(
        MouseSensor::TOWARDS );
    }
  }
}

bool H3DPlugin::onMouseScroll(FB::MouseScrollEvent *evt, FB::PluginWindow *)
{
  EventData d ( this, evt );
  addH3DThreadCallbackBlocking ( onMouseScrollCallback, &d );
  return false;
}

void H3DPlugin::onKeyDownCallback ( void* data ) {
  EventData* d= static_cast<EventData*>(data);
  FB::KeyDownEvent* evt= static_cast<FB::KeyDownEvent*>(d->event);

  if ( d->plugin->h3dPluginWindow ) {
    d->plugin->h3dPluginWindow->onKeyDown( evt->m_os_key_code, false );
  }
}

bool H3DPlugin::onKeyDown(FB::KeyDownEvent *evt, FB::PluginWindow *)
{
  EventData d ( this, evt );
  addH3DThreadCallbackBlocking ( onKeyDownCallback, &d );
  return false;
}

void H3DPlugin::onKeyUpCallback ( void* data ) {
  EventData* d= static_cast<EventData*>(data);
  FB::KeyUpEvent* evt= static_cast<FB::KeyUpEvent*>(d->event);

  if ( d->plugin->h3dPluginWindow ) {
    d->plugin->h3dPluginWindow->onKeyUp( evt->m_os_key_code, false );
  }
}

bool H3DPlugin::onKeyUp(FB::KeyUpEvent *evt, FB::PluginWindow *)
{
  EventData d ( this, evt );
  addH3DThreadCallbackBlocking ( onKeyUpCallback, &d );
  return false;
}

void H3DPlugin::onWindowAttachedCallback ( void* data ) {
  H3DPlugin* plugin= static_cast < H3DPlugin* > ( data );

  plugin->scene.reset ( new Scene );
  plugin->h3dPluginWindow= new H3DPluginWindow ( plugin->pluginWindowWin );
  plugin->scene->window->push_back ( plugin->h3dPluginWindow );

  if ( plugin->sceneUrl != "" ) {
    plugin->scene->loadSceneRoot ( plugin->sceneUrl );
  }

  staticState.count++;
}

bool H3DPlugin::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *window )
{
  // The window is attached; act appropriately
  pluginWindow = window;
  pluginWindowWin = dynamic_cast<FB::PluginWindowWin*>(window);

  // We need to wait until OpenGL has been initialized by the H3D thread
  // otherwise Firefox will crash on resize of mouse down events
  // See: http://forum.firebreath.org/topic/181/
  addH3DThreadCallbackBlocking ( onWindowAttachedCallback, this );

  return false;
}

void H3DPlugin::onWindowDetachedCallback ( void* data ) {
  H3DPlugin* plugin= static_cast < H3DPlugin* > ( data );

  plugin->scene.reset ( NULL );
  staticState.count--;
}

bool H3DPlugin::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *) {
  addH3DThreadCallbackBlocking ( onWindowDetachedCallback, this );

  // The window is about to be detached; act appropriately
  return false;
}