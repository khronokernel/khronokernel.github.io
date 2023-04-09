---
layout: page
title: Portfolio
permalink: /portfolio/
---

Noteworthy projects of mine:

* [OpenCore Legacy Patcher](#opencore-legacy-patcher)
* [FeatureUnlock](#featureunlock)
* [CryptexFixup](#cryptexfixup)
* [Dortania Documentation](#dortania-documentation)
* [Miscellaneous Design Work](#miscellaneous-design-work)

### [OpenCore Legacy Patcher](https://github.com/dortania/OpenCore-Legacy-Patcher)

Project dedicated to keeping old, unsupported Macs out of the land fill and running the latest versions of macOS! Project relies on many components, including [OpenCorekg's](https://github.com/acidanthera/OpenCorePkg) sophisticated injection system, [Lilu's](https://github.com/acidanthera/Lilu) kernel hooking and our own Python-based root volume patcher to restore hardware support including legacy Metal and OpenGL graphics rendering.

OpenCore Legacy Patcher is maintained by a small group of enthusiasts, and the work is no small task. We've reversed engineered many closed source binaries and frameworks, documented many unknown aspects of the operating system and develop patches and utilities to help get these older systems booted.


### [FeatureUnlock](https://github.com/acidanthera/FeatureUnlock)

XNU Kernel Extension dedicated to unlocking OS features for all Macs, relying on [Lilu's](https://github.com/acidanthera/Lilu) Kernel Hooking APIs. Unlocked features include Night Shift, SideCar, AirPlay to Mac, Universal Control and Continuity Camera for models as old as the 2007 iMac.


### [CryptexFixup](https://github.com/acidanthera/CryptexFixup)

XNU Kernel Extension dedicated to restoring pre-Haswell OS support to macOS Ventura, relying on [Lilu's](https://github.com/acidanthera/Lilu) Kernel Hooking APIs. System is based on installing the Apple Silicon Cryptex, and having legacy Macs rely on Rosetta's x86_64 dyld shared cache.

* Based on original Cryptex Findings: [macOS Ventura and the new dyld shared cache system](https://khronokernel.github.io/macos/2022/06/22/VENTURA-DYLD.html)


### Dortania Documentation

An organization dedicated to documenting the many aspects of macOS booting on PC hardware, we have consolidated much of the complicated nature of hackintoshing into multiple cohesive guides that many non-technical users can easily follow.

* [OpenCore Install Guide](https://dortania.github.io/OpenCore-Install-Guide/)
  * Guide dedicated to the process of setting up, booting and installing a Hackintosh.
  * Includes documentation on laptops, desktops, and machines in between.
* [OpenCore Post-Install](https://dortania.github.io/OpenCore-Post-Install/)
  * Guide dedicated to the clean up and miscellaneous fixes of hackintoshes once installed.

Other guides are listed here:

* [Dortania's Getting Started](https://dortania.github.io/getting-started/)



### Miscellaneous Design Work

While the majority of my work is on documentation with Dortania and Acidanthera, I do also work on design projects including website assets and app icons.

* [ProperTree](#propertree)
* [MaciASL](#maciasl)
* [Dortania Assets](#dortania-assets)

#### ProperTree

* [Repo](https://github.com/CorpNewt/ProperTree)

A multiplatform plist editor written in python, I designed the icon with the idea of how plist files have ever-expanding branches of new entries and dictionaries. I additionally refreshed the icon's design with Big Sur's release.

![](../images/topbar/design/ProperTree-Icon.png)

#### MaciASL

* [Repo](https://github.com/acidanthera/MaciASL)

A simple yet useful ACPI editor for macOS, the original icon dated back to 2013. I wanted to update the design to be more Big Sur fitting, but keeping the original charm of the old icon.

![](../images/topbar/design/MaciASL-Icon.png)

#### Dortania Assets

As I am the main author and maintainer of [Dortania](https://dortania.github.io/), I also work a lot with the site's design and icons. Here are just a few of the icons I've designed for the organization:

![](../images/topbar/design/Dortania-Assets.png)