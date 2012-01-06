#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for H3DPlugin
#
#\**********************************************************/

set(PLUGIN_NAME "H3DPlugin")
set(PLUGIN_PREFIX "HPL")
set(COMPANY_NAME "NeilForrest")

# ActiveX constants:
set(FBTYPELIB_NAME H3DPluginLib)
set(FBTYPELIB_DESC "H3DPlugin 1.0 Type Library")
set(IFBControl_DESC "H3DPlugin Control Interface")
set(FBControl_DESC "H3DPlugin Control Class")
set(IFBComJavascriptObject_DESC "H3DPlugin IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "H3DPlugin ComJavascriptObject Class")
set(IFBComEventSource_DESC "H3DPlugin IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID a3632248-8e0a-5e1a-9e03-c6ced72f4c94)
set(IFBControl_GUID e38e3006-d4df-5942-9cbf-3791b779c398)
set(FBControl_GUID 36ef11ba-8daf-5059-98f4-ed2e0246a2ed)
set(IFBComJavascriptObject_GUID 32c9b5d8-eb40-5f1f-a80d-b10962f0c780)
set(FBComJavascriptObject_GUID ef14ec29-5493-55c6-8b63-a201c377d658)
set(IFBComEventSource_GUID a83b559f-496d-5ed9-827c-91771b6f37f9)

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "NeilForrest.H3DPlugin")
set(MOZILLA_PLUGINID "www.h3dapi.org/H3DPlugin")

# strings
set(FBSTRING_CompanyName "Neil Forrest")
set(FBSTRING_FileDescription "X3D model in XML format;X3D model in VRML format")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2011 Neil Forrest")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "H3DPlugin")
set(FBSTRING_FileExtents "x3d;x3dv")
set(FBSTRING_PluginName "H3DPlugin")
set(FBSTRING_MIMEType "model/x3d+xml;model/x3d+vrml")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)

add_firebreath_library(log4cplus)