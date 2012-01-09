//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2007, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file H3DPluginWindow.h
/// \brief Header file for H3DPluginWindow.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DPLUGINWINDOW_H__
#define __H3DPLUGINWINDOW_H__

#include <H3D/H3DWindowNode.h>

#include "PluginWindow.h"
#include "PluginCore.h"

#ifdef FB_WIN
#include "PluginWindowWin.h"
#endif

namespace H3D {

  class H3DPluginWindow : public H3DWindowNode {
  public:

    /// Constructor.
    H3DPluginWindow( FB::PluginWindowWin* _fbWindow  = 0, 
                     Inst< SFInt32     > _width      = 0,
                     Inst< SFInt32     > _height     = 0,
                     Inst< SFBool      > _fullscreen = 0,
                     Inst< SFBool      > _mirrored   = 0,
                     Inst< RenderMode  > _renderMode = 0, 
                     Inst< SFViewpoint > _viewpoint  = 0,
                     Inst< SFInt32     > _posX       = 0,
                     Inst< SFInt32     > _posY       = 0,
                     Inst< SFBool      > _manualCursorControl = 0,
                     Inst< SFString    > _cursorType = 0 );

    virtual ~H3DPluginWindow();

    /// Creates and initializes a wxWindow for OpenGL rendering
    virtual void initWindow();

    /// Calls wxGLCanvas::SwapBuffers
    virtual void swapBuffers(); 

    /// Initializes wxWindow
    virtual void initWindowHandler() { }

    /// Sets fullscreen mode.
    virtual void setFullscreen( bool fullscreen );

    /// Make this the current window.
    virtual void makeWindowActive();
    
    /// Overidden to disable sharing context as it causes a crash when more than
    /// one plugin instance is running.
    virtual void initialize();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:

    void enableOpenGL(HWND _handleWnd, HDC& _hdc, HGLRC& _hRC);
    void disableOpenGL(HWND _handleWnd, HDC _hDC, HGLRC _hRC);

    FB::PluginWindowWin* fbWindow;

    HDC hDC;
    HGLRC hRC;

    PAINTSTRUCT ps;
  };
}

#endif
