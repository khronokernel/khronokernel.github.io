---
layout: post
title: "Big Sur and legacy Mac support: Audio"
date: 2021-10-24 7:00:00 -0600
categories: macOS
---

In this multi-part blog post, I'll be discussing my research on running unsupported Macs in newer macOS versions through OpenCore Legacy Patcher. Today we'll be going over all the trials and tribulations of **fixing audio support in macOS Big Sur!**

### Quick Summary

For those unfamiliar with my work, I am the co-author of [OpenCore Legacy Patcher](https://github.com/dortania/OpenCore-Legacy-Patcher) alongside my good friend, the brilliant [Dhinak. G](https://github.com/dhinakg). Our goal with this project is to allow for a near-native experience of unsupported Macs on newer OSes through the power of in-memory patching with [OpenCorePkg](https://github.com/acidanthera/OpenCorePkg) and [Lilu](https://github.com/acidanthera/Lilu).

## Understanding the audio situation

As some of you may be familiar, with macOS Mojave and newer, Apple removed support for numerous legacy Macs that did not have Metal support in their drivers. With this, Apple also decided to do some cleaning and strip certain sections of the OS of this old cruft. One of these sections being AppleHDA.

AppleHDA is a fairly simple kernel extension for setting up audio support for devices. It does this by parsing the Layout ID detected in the firmware (via DeviceProperties) and loads the associated audio map off of disk.

* These maps can be located at `/System/Library/Extensions/AppleHDA.kext/Contents/Resources/`
* Layout IDs are present on the HDEF device in IOService as `layout-id`, data is generally presented in Hexadecimal form but supports Integer and ASCII
* Layout data is generally presented as 2 files: layout{x}.xml and Platforms.xml. Platforms.xml represents chipset definitions while layout.xml is reserved for model-specific data

AppleHDA has other plugins that handle digital audio, etc but for this topic, we'll be limiting ourselves to AppleHDA as it's our main culprit for legacy Macs.

## Fixing audio support

To fix audio support, we have 2 methods:

* Delete the existing AppleHDA off disk and install High Sierra's extension
* Attempt to re-add the layout IDs to disk

What dosdude1 did with his patchers is the former, but overall this is not very efficient as we're using more and more legacy kexts that may fail to link with the KC in the future. With OpenCore Legacy Patcher, I was determined to have a reliable system that required zero root volume patching. So I was off to work.

The hackintosh community already developed a powerful tool called AppleALC, where the main goal was to patch AppleHDA in-memory with unsupported data sets (namely for hackintoshes). I had already experimented with AppleALC modifying existing data sets for my HP Elite X2 G1 so was fairly confident in fixing audio support for these legacy machines. However, I was very wrong.

## The troubles

While the basic idea of simply editing the layouts via AppleALC seems quite simple, the execution was a huge headache. Overall it took me about 2 to 3 months to implement a proper AppleALC-based approach only after hours of endless head bashing.

We had 2 main issues to resolve. The first was Lilu, the patching core of AppleALC, who didn't want to communicate with our MacBook7,1 test system. After a bit of trial and error, we found the issue to be with the chipset vendor check as only Intel and AMD were supported while the MacBook7,1 was Nvidia. [A quick patch](https://github.com/acidanthera/Lilu/commit/575bd85c759fcec66c02a0ab960b5d8dad095c69) fixed this.

Now our second issue was the most time-consuming. AppleHDA refused to actually process our injected layout data for unknown reasons. We weren't sure exactly why due to lacklustre debug information from AppleHDA so I started to experiment with editing the layout IDs. We had 3 main theories as to why this may be:

* AppleHDA's layout ID data structure changed, invalidating old configs
* AppleHDA had stripped Nvidia codec support
* AppleHDA had hardcoded layout IDs (ie. we must edit existing data)

Going 1 by 1:

1. Validated not to be true, as the majority of supported layout sets were unedited between High Sierra and Big Sur
2. AppleHDA's initial probing had not been changed drastically
3. Majority of layout IDs were not written into the binary 

After squashing these 3 theories, I was quite stumped. I eventually became desperate and started to strip layout data to generate as minimal configs as possible. Eventually, I had fixed the issue, and we had full working audio! But what was the issue?

Well apparently with macOS Mojave and newer, newly built kexts had a hard ceiling of 512KB per XML file loaded. And our PathMap file was 600KB...

* Legacy kexts were unaffected by this as they were never compiled with this new ceiling in place, explaining why the old AppleHDA was still functional

After discovering this, I had a pretty easy solution in mind: split the PathMap file between 2 codecs to work around the 512KB limitation.

High Sierra's AppleHDA supported 2 codec vendors, CirrusLogic (2009 and newer) and Realtek (2008 and older). So when implementing the AppleALC patches, I wrote a PathMap for each codec. And with this, we had reliable, native-like audio!

Looking back, the solution was insanely simple but without any other discussions online about this limitation, we really had no other road to go down besides throwing everything at the wall till something stuck.