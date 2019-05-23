# Mini Shell extension Framework (MSF)

## Build Status

[![Build status](https://ci.appveyor.com/api/projects/status/9o2gbepqfcw1g141/branch/master?svg=true)](https://ci.appveyor.com/project/vbaderks/msf/branch/master)

## Introduction

The Mini Shell extension Framework (MSF) is a collection of C++17 template classes that can be
used under the LGPL licence to create shell extensions for the Microsoft Window Explorer.
Microsoft Windows Shell Extensions are in essence COM objects. MSF relies on the Microsoft Active Template Library (ATL) to
provide support for implementing COM objects.

## Shell Extensions Overview

### Supported

* Info Tip Handler (IPersistFile, IQueryInfo)
* Property Sheet Handler (IShellExtInitImpl, IShellPropSheetExt)
* Shortcut Menu Handler (IContextMenu, IContextMenu2, IContextMenu3)
* Copy Hook Handler (ICopyHook)
* Icon Overlay Handler (IShellIconOverlayIdentifier)
* Namespace Extension (IShellView, IShellFolder, IShellFolder2)

### Legacy

The list below are shell extension types not used anymore in recent Windows versions.

* Column Provider (IColumnProvider). Not used anymore by Windows Vista or newer.

## Documentation

A couple of articles discussing how to create shell extensions are available on codeproject:

* <https://www.codeproject.com/Articles/8063/The-Mini-Shell-Extension-Framework-Part-I>

* <https://www.codeproject.com/Articles/9421/The-Mini-Shell-extension-Framework-Part-II>

* <https://www.codeproject.com/Articles/11674/The-Mini-Shell-Extension-Framework-Part-III>

## Operating Systems and Compilers

To reduce maintenance effort MSF has 3 support levels:

* Supported. This is the compiler \ OS used to validate MSF.
  In practice this will be latest available RTM versions of the OS and compiler.
  The thought behind it is that most people that create shell extensions are
  using the latest available compiler and target the latest version of Windows.

* On-demand. These versions are not longer developed or tested, but support is
   not removed from the code base.

* Legacy support. These versions are
   not longer actively maintained and support is removed to keep the code clean.
   The source code in the repository is tagged to make it easy to retrieve
   old versions that have support for these compilers \ Windows versions.

### Supported Operating Systems

* Windows 10

### On-demand Operating Systems

* Windows 7

### Legacy Operating Systems

* Windows 8.1
* Windows 8.0
* Windows Vista
* Windows XP
* Windows 2000
* Windows ME
* Windows 98

### Supported Compilers

* Visual Studio 2019 16.1 (ATL 14.0)
* Visual Studio 2017 (ATL 14.0)

### Legacy Compilers

Support has been removed from latest release. Recommendation is to upgrade the compiler, but older release are available in the history of the repository.

* Visual Studio 2015 Update 3 (ATL 14.0)
* Visual Studio 2013 Update 5 (ATL 12.0)
* Visual Studio 2012 Update 5 (ATL 11.0)
* Visual Studio 2010 SP1 (ATL 10.0)
* Visual Studio 2008 SP1 (ATL 9.0)
* Visual Studio 2005 SP1 (ATL 8.0)
* Visual Studio .NET 2003 (ATL 7.1)
* Visual Studio .NET 2002 (ATL 7.0), needs latest Windows SDK

Note: Latest source code is only validated with Visual Studio 2019 and tested on Windows 10 x64.
