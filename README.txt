Security Disclaimer!!
=====================

In its present form, this plugin is provided for development purposes only. Installation of this
plugin will create major security issues for you! 

We recomend that you disable this plugin when your are not using it, and that you only visit the official
example pages when the plugin is enabled.

In summary: Install this plugin at your own risk!


Requirements
============

H3DAPI, CMake

 * Ensure CMake is installed and that the CMake bin is in that PATH
 * Ensure H3DAPI is installed and compiled and that the H3D_ROOT environment variable is set correctly

Optional:

For installer:
Wix - http://wix.sourceforge.net/

Build instructions
==================

In Git Bash
-----------

mkdir FireBreath
cd FireBreath
git clone git://github.com/firebreath/FireBreath.git firebreath-dev

mkdir firebreath-projects
cd firebreath-projects
git clone git://github.com/neilforrest/H3DPlugin.git H3DPlugin


In a command prompt
-------------------

CD to the FireBreath directory created earlier

firebreath-dev\prep2010.cmd firebreath-projects firebreath-build

Open FireBreath-build/FireBreath.sln in Visual Studio

Switch to release mode and build the ALL_BUILD target

Installation
============

regsvr32 firebreath-build\bin\H3DPlugin\Release\npH3DPlugin.dll to install for all browsers
Or run the Wix generated installer
Or copy npH3DPlugin.dll to C:\Program Files (x86)\Mozilla Firefox\plugins (for Firefox installation only)

Testing
=======

Verify that the plugin is found by the browser (in firefox visit about:plugins)
To test the plugin visit the plugin test page in the browser