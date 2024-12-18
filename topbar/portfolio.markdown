---
layout: page
title: Portfolio
permalink: /portfolio/
---

Noteworthy projects of mine:

* [Vulnerability Research](#vulnerability-research)
* [Conference Talks](#conference-talks)
* [OpenCore Legacy Patcher](#opencore-legacy-patcher)
* [FeatureUnlock](#featureunlock)
* [CryptexFixup](#cryptexfixup)
* [Dortania Documentation](#dortania-documentation)

### Vulnerability Research

Sample of some security vulnerabilities found in macOS applications commonly deployed in enterprise environments:

| Reference ID | Product | Affected Versions | Description |
| --- | --- | --- | --- |
| ________________________________________________ | ____________________________________________________________________________________ | ____________________________________________________________ | ____________________________________________________________ |
| [CVE-2024-27822](https://support.apple.com/en-ca/HT214106){:target="_blank"} | macOS | 14.4.1 and older | Local Privilege Escalation |
| [CVE-2024-34331](https://khronokernel.com/macos/2024/05/30/CVE-2024-34331.html){:target="_blank"} | Parallels Desktop | 19.3.0 and older | Local Privilege Escalation |
| [CVE-2024-4395](https://www.cve.org/CVERecord?id=CVE-2024-4395){:target="_blank"} | Jamf Compliance Editor | 1.3.0 and older | Local Privilege Escalation |
| [CVE-2024-38811](https://support.broadcom.com/web/ecx/support-content-notification/-/external/content/SecurityAdvisories/0/24939){:target="_blank"} | VMware Fusion | v13.5 and older | Arbitrary Code Execution |
| [CVE-2024-25545](https://www.cve.org/CVERecord?id=CVE-2024-25545){:target="_blank"} | Weave Desktop | Unresolved | Arbitrary Code Execution |
| [Synology-SA-24:05](https://www.synology.com/en-us/security/advisory/Synology_SA_24_05){:target="_blank"} | Synology Surveillance Station Client | 2.1.3-2474 and older | Arbitrary Code Execution |
| [CVE-2024-23755](https://www.cve.org/CVERecord?id=CVE-2024-23755){:target="_blank"} | ClickUp Desktop App | 3.3.76 and older | Arbitrary Code Execution |
| [CVE-2023-50975](https://www.cve.org/CVERecord?id=CVE-2023-50975){:target="_blank"} | TD Advanced Dashboard | 3.0.3 and older | Arbitrary Code Execution |
| [CVE-2023-7245](https://www.cve.org/CVERecord?id=CVE-2023-7245){:target="_blank"} | OpenVPN | 3.4.7 and older | Arbitrary Code Execution |
| [CVE-2023-50975](https://gist.github.com/khronokernel/2598c067d0f49b0f0a4c8b01cf129d34){:target="_blank"} | TD Advanced Dashboard | 3.0.3 and older | Arbitrary Code Execution |
| [CVE-2023-44077](https://www.cve.org/CVERecord?id=CVE-2023-44077){:target="_blank"} | ShareBrowser XPC Services | 6.1.5.27 and older | Local Privilege Escalation |


### Conference Talks

#### [Apple’s (not so) Rapid Security Response](https://www.youtube.com/watch?v=Fz6QtdjhtB8)
##### Objective by the Sea v7.0, December 2024

A look into Apple’s Rapid Security Response system, including the precursor, implementation
details, challenges and where Apple’s been secretly deploying them to this day.

#### [Electron Security: Making your Mac a worse place?](https://www.youtube.com/watch?v=e6u-qLruXjs)
##### MacDevOpsYVR 2024, June 2024

Discusses the dangers of misconfigured Electron fuses for TCC bypasses, and unveiling
[Lectricus](https://github.com/ripeda/lectricus){:target="_blank"}, a utility for programmatically detecting these vulnerable applications.

#### [Legacy Macs, Modern Solutions](https://www.youtube.com/watch?v=iTlQN_47Kcg)
##### BSides Calgary 2023, November 2023

Deep dive into macOS’ tier 2 citizenship, and how [OpenCore Legacy Patcher](https://github.com/dortania/OpenCore-Legacy-Patcher){:target="_blank"} uses this to
restore support for legacy machines allowing new features and security updates.


### [OpenCore Legacy Patcher](https://github.com/dortania/OpenCore-Legacy-Patcher)

Project dedicated to keeping old, unsupported Macs out of the land fill and running the latest versions of macOS! Project relies on many components, including [OpenCorePkg's](https://github.com/acidanthera/OpenCorePkg) sophisticated injection system, [Lilu's](https://github.com/acidanthera/Lilu) kernel hooking and our own Python-based root volume patcher to restore hardware support including legacy Metal and OpenGL graphics rendering.

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