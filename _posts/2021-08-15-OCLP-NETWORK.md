---
layout: post
title:  "Big Sur and legacy Mac support: Networking"
date:   2021-08-15 7:00:00 -0600
categories: macOS
---

In this multi-part blog post, I'll be discussing my research on running unsupported Macs in macOS Big Sur through OpenCore Legacy Patcher. Today we'll be going over all the trials and tribulation of **fixing networking support in macOS Big Sur!**

### Quick Summary

For those unfamiliar with my work, I, Mykola Grymalyuk, am the co-author of [OpenCore Legacy Patcher](https://github.com/dortania/OpenCore-Legacy-Patcher) alongside my good friend, the brilliant [Dhinak. G](https://github.com/dhinakg). Our goal with this project is to allow for a near-native experience of unsupported Macs on newer OSes through the power of in-memory patching with [OpenCorePkg](https://github.com/acidanthera/OpenCorePkg) and [Lilu](https://github.com/acidanthera/Lilu).

### Table of Contents

This blog post will be split into 2 parts, Ethernet and Wifi:

* [Ethernet](#ethernet)
  * [Nvidia and Marvel](#nvidia-and-marvel)
  * [Broadcom](#broadcom)
* [Wifi](#wifi)
  * [BCM943224 and BCM94331](#bcm943224-and-bcm94331)
  * [BCM94322, BCM94328 and Atheros](#bcm94322-BCM94328-and-atheros)

## Ethernet

Overall Apple's Ethernet stack is fairly straight forward and [open-sourced for others to integrate into.](https://opensource.apple.com/source/IONetworkingFamily/IONetworkingFamily-151.40.1/) This is the reason we're able to have [3rd party SmallTree extensions for i210 NICs](https://small-tree.com/support/downloads/gigabit-ethernet-driver-download-page/) as well as why overall there is *usually* little difficulties for hackintosh communities to develop their own kernel extensions.

### Nvidia and Marvel

With macOS Catalina, both Nvidia and Marvel NICs were dropped as the last Macs to ship with these controllers were dropped in High Sierra/Mojave. Re-adding support was as simple as taking the nForce and Marvel-Yukon Ethernet kexts from Mojave and injecting them via OpenCore. No extra patches are needed for the binaries thankfully.

* [Marvel Yukon Ethernet](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Ethernet/MarvelYukonEthernet-v1.0.0.zip)
* [nForce Ethernet](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Ethernet/nForceEthernet-v1.0.0.zip)

### Broadcom

With macOS Big Sur, Apple had refactored the BCM5701 Ethernet extension to [support ARM64-based hardware](https://www.apple.com/newsroom/2020/06/apple-announces-mac-transition-to-apple-silicon/) as many Thunderbolt devices and even the M1 Mac mini and 24" iMac use them. With this opportunity, Apple decided to strengthen security by enforce VT-D to be active when using this driver to help prevent [local DMA attacks.](https://support.apple.com/en-ca/guide/security/seca4960c2b5/web) However one small issue, **pre-Ivy Bridge Macs don't support VT-D for DMA protection!** 

This new security measure resulted in quite the headache for myself to resolve, I attempted numerous ways to downgrade security by bypassing XNU's VT-D enforcement as well as bin-patching Big Sur's AppleBCM5701Ethernet binary to no avail. In addition, I was required to work with users remotely so could never deep dive on testing without local hardware.

I eventually bit the bullet and bought an iMac11,2 on Kijiji to resolve this as well as AppleALC issues we were facing at the time. And within 2 hours, the Broadcom bug was finally fixed through patching [conflicting symbols](http://nickdesaulniers.github.io/blog/2016/08/13/object-files-and-symbols/) of Catalina's BCM5701 extension and injecting it through OpenCore. 

Note: Symbols needed to be patched as the Big Sur kext would conflict and break our Catalina driver

* [Catalina's Patched BCM5701 Ethernet](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Ethernet/CatalinaBCM5701Ethernet-v1.0.1.zip)

## Wifi

And now comes the more frustrating part of supporting legacy hardware, the Wireless stack! Apple's IO80211 is a closed source framework that's constantly being updated to combat many of the vulnerabilities found out in the wild. Because of this, re-adding support to macOS and retaining functionality is actually quite difficult.

### BCM943224 and BCM94331

With Big Sur, we were given a glimpse into Apple's unification of the IO80211 networking stack. Specifically the deprecation of multiple sub kexts within IO80211 and only having 1 main extension to manage devices.

When Apple wrote this new stack, they decided to kill off all non-BCM94360/94350 cards as no Macs supported would need them. However, the stack was written in such a way that it still supported the BCM943224 and BCM94331 cards. From this observation, **attempting to spoof the wireless card to a supported model would actually restore full functionality!**

Note: to allow for better legacy OS support within OpenCore Legacy Patcher, [I decided to inject the BCM943224 and BCM94331 device IDs into Big Sur's kext](https://github.com/dortania/OpenCore-Legacy-Patcher/commit/b1d58511313f1cc3c0a8b888e70a3029d3af0963) instead of spoofing the Wireless card into a newer model.

### BCM94322, BCM94328 and Atheros

Unfortunately with these older chipsets, they're not supported with the refactored IO80211 stack in Big Sur. Thus we needed not only to inject the old extension, we need to re-inject the old IO80211 stack entirely. This is quite cumbersome however after a ton of bin-patching to resolve symbols, these cards are working beautifully now. 

Note: BCM94328 chipsets were dropped so long ago (Sierra) that they also needed a back-ported corecrypto to allow for injection, otherwise erroring out on missing symbols.

Note 2: Due to the injection of a whole IO80211 stack, many native networking features can break including WPA and AirDrop to Mac support. Without an insane amount of working reversing the stack like [itlwm](https://github.com/OpenIntelWireless/itlwm), these issues cannot be resolved.

* [BCM94328 supported IO80211Family (El Capitan)](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Wifi/IO80211ElCap-v1.0.0.zip)
  * [Associated El Capitan corecrypto](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Wifi/corecaptureElCap-v1.0.0.zip)
* [Atheros supported IO80211Family (High Sierra)](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Wifi/IO80211HighSierra-v1.0.0.zip)
* [BCM94322 supported IO80211Family (Mojave)](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/28746bbe2b03ae4aa5f5265a1bb50f48785b2a23/payloads/Kexts/Wifi/IO80211Mojave-v1.0.0.zip)

