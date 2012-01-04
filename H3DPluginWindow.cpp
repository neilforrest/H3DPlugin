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
/// \file H3DPluginWindow.cpp
/// \brief CPP file for H3DPluginWindow.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "H3DPluginWindow.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DPluginWindow::database( "WxWidgetsWindow", 
                                      &(newInstance<H3DPluginWindow>), 
                                      typeid( H3DPluginWindow ),
                                      &(H3DWindowNode::database) );


H3DPluginWindow::H3DPluginWindow( FB::PluginWindowWin* _fbWindow,
                                  Inst< SFInt32     > _width,
                                  Inst< SFInt32     > _height,
                                  Inst< SFBool      > _fullscreen,
                                  Inst< SFBool      > _mirrored,
                                  Inst< RenderMode  > _renderMode, 
                                  Inst< SFViewpoint > _viewpoint,
                                  Inst< SFInt32     > _posX,
                                  Inst< SFInt32     > _posY,
                                  Inst< SFBool      > _manualCursorControl,
                                  Inst< SFString    > _cursorType ) :
  H3DWindowNode( _width, _height, _fullscreen, _mirrored, _renderMode,
                 _viewpoint, _posX, _posY, _manualCursorControl,
                 _cursorType ),
  fbWindow ( _fbWindow ) {
  type_name = "H3DPluginWindow";
  database.initFields( this );
}

void H3DPluginWindow::initWindow() {
  enableOpenGL( fbWindow->getHWND(), hDC, hRC );
  SetFocus( fbWindow->getHWND() );
}

void H3DPluginWindow::swapBuffers() {
  SwapBuffers( hDC );
  EndPaint(fbWindow->getHWND(), &ps);
}

void H3DPluginWindow::setFullscreen( bool fullscreen ) {
}

void H3DPluginWindow::makeWindowActive() {
  // Reshape the window to match current dimentions
  FB::Rect pos= fbWindow->getWindowPosition();
  int w= pos.right-pos.left;
  int h= pos.bottom-pos.top;
  if ( width->getValue() != w || height->getValue() != h ) {
    reshape ( w, h );
  }

  wglMakeCurrent( hDC, hRC );
  hDC= BeginPaint(fbWindow->getHWND(), &ps);
}

void H3DPluginWindow::enableOpenGL(HWND _handleWnd, HDC& _hDC, HGLRC& _hRC) { 
  PIXELFORMATDESCRIPTOR pfd;
  int format;
 
  // get the device context (DC)
  _hDC = GetDC( _handleWnd );
 
  // set the pixel format for the DC
  ZeroMemory( &pfd, sizeof( pfd ) );
  pfd.nSize = sizeof( pfd );
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
  format = ChoosePixelFormat( _hDC, &pfd );
  SetPixelFormat( _hDC, format, &pfd );
 
  // create and enable the render context (RC)
  _hRC = wglCreateContext( _hDC );
  wglMakeCurrent( _hDC, _hRC );
}
 
void H3DPluginWindow::disableOpenGL(HWND _handleWnd, HDC _hDC, HGLRC _hRC) {
  wglMakeCurrent( NULL, NULL );
  wglDeleteContext( _hRC );
  ReleaseDC( _handleWnd, _hDC );
}