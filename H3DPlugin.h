/**********************************************************\

  Auto-generated H3DPlugin.h

  This file contains the auto-generated main plugin object
  implementation for the H3DPlugin project

\**********************************************************/
#ifndef H_H3DPluginPLUGIN
#define H_H3DPluginPLUGIN

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/KeyboardEvents.h"
#include "PluginEvents/AttachedEvent.h"

#include "PluginCore.h"

//opengl thread stuff
#include <windows.h>

#include <stdio.h>
//#include <strsafe.h>

#include "H3DPluginWindow.h"

#include <H3D/Scene.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

FB_FORWARD_PTR(H3DPlugin)
class H3DPlugin : public FB::PluginCore
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    H3DPlugin();
    virtual ~H3DPlugin();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();

    static void drawThreaded();
    static void loadSceneCallback ( void* data );

    static bool idle ();

    void loadScene ( const std::string& url );

    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    struct EventData {
      EventData ( H3DPlugin* _plugin, FB::PluginEvent* _event ) : 
        plugin ( _plugin ), event ( _event ) {}
      
      H3DPlugin* plugin;
      FB::PluginEvent* event;
    };

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::MouseDownEvent, onMouseDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseUpEvent, onMouseUp, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseScrollEvent, onMouseScroll, FB::PluginWindow)

        EVENTTYPE_CASE(FB::KeyDownEvent, onKeyDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::KeyUpEvent, onKeyUp, FB::PluginWindow)

        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *);
    virtual bool onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *);
    virtual bool onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *);
    virtual bool onMouseScroll(FB::MouseScrollEvent *evt, FB::PluginWindow *);

    virtual bool onKeyDown(FB::KeyDownEvent *evt, FB::PluginWindow *);
    virtual bool onKeyUp(FB::KeyUpEvent *evt, FB::PluginWindow *);

    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

protected:

  class ConsoleStreamBuf: public std::streambuf {
  public:
    ConsoleStreamBuf ( FB::BrowserHost& host ) : m_host ( &host ) {}

    /// This function needs to be here in order for xsputn to be called.
    /// Not sure why..
    int overflow( int c ) {
      // return something different from EOF
      return 0;
    }

    /// Transfer the string to wxTextCtrl.
    std::streamsize xsputn ( const char * s, std::streamsize n );

  protected:
    FB::BrowserHost* m_host;
  };

  H3DUtil::AutoRef< H3D::Scene > scene;
  H3D::H3DPluginWindow* h3dPluginWindow;
  std::string sceneUrl;

  typedef void (*CallbackFunc)(void *data);

  typedef vector<pair<CallbackFunc, void*>> CallbackList;

  static void onWindowAttachedCallback ( void* data );
  static void onWindowDetachedCallback ( void* data );

  static void onMouseDownCallback ( void* data );
  static void onMouseUpCallback ( void* data );
  static void onMouseMoveCallback ( void* data );
  static void onMouseScrollCallback ( void* data );

  static void onKeyDownCallback ( void* data );
  static void onKeyUpCallback ( void* data );

  static void addH3DThreadCallback ( CallbackFunc callback, void* data );
  static void addH3DThreadCallbackBlocking ( CallbackFunc callback, void* data );

  struct StaticState {
    StaticState () : h3d_finished ( false ), count ( 0 ), callbacks_executed ( false ) {}

    boost::mutex h3d_mutex;
    
    bool h3d_finished;
    boost::thread h3d_thread;
    CallbackList callbacks;
    int count;

    bool callbacks_executed;
    boost::condition callbacks_executed_condition;
  };

  static StaticState staticState;

  FB::PluginWindowWin* pluginWindowWin;
  FB::PluginWindow* pluginWindow;

  // the buffer object for console_stream.
  ConsoleStreamBuf *console_stream_buf;

  // The console stream.
  std::auto_ptr< std::ostream >console_stream;
};


#endif

