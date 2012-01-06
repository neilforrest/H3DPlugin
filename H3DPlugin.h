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

    static LPTHREAD_START_ROUTINE drawThreaded( LPVOID lpParam );
    static H3D::Scene::CallbackCode loadSceneCallback ( void* data );

    bool render ();

    void loadScene ( const std::string& url );

    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

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
  boost::mutex h3d_mutex;
  boost::condition h3d_inited_condition;
  bool h3d_inited;
  std::string sceneUrl;

  bool h3d_finished;

  boost::thread h3d_thread;

  FB::PluginWindowWin* pluginWindowWin;
  FB::PluginWindow* pluginWindow;

  // the buffer object for console_stream. Pointer is owned by
  // the console_stream object.
  ConsoleStreamBuf *console_stream_buf;

  // The console stream. The contents of other_thread_output is eventuelly
  // transferred to this stream.
  std::auto_ptr< std::ostream >console_stream;
};


#endif

