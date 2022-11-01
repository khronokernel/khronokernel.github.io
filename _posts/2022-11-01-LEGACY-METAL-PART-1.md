---
layout: post
title: "macOS Ventura and legacy Metal: Part 1; Catching up on the past"
date: 2022-08-01 7:00:00 -0600
categories: macOS
---

With the release of macOS Ventura, Apple dropped all Metal 1 GPUs (Intel Haswell, Broadwell) as well as some Metal 2 ones (Intel Skylake and AMD GCN 1-3). In this multi-part blog post, we'll talk about the challenges faced trying to restore support for many older Graphics Cards. Today's post will be focusing primarily on the challenges faced in the past with macOS Big Sur and Monterey.

----------

* [Catching up with the past: Big Sur and Monterey](#catching-up-with-the-past-big-sur-and-monterey)
  * [Battle against on-disk binaries](#battle-against-on-disk-binaries)
  * [First removal of Metal graphics drivers](#first-removal-of-metal-graphics-drivers)
* [Where do we go from here?](#where-do-we-go-from-here)

# Catching up with the past: Big Sur and Monterey

Before we dive too much into macOS Ventura's situation, we should first discuss some of the changes and challenges presented by the older OSes (ie. Big Sur and Monterey) concerning graphics patching:

* [Battle against on-disk binaries](#battle-against-on-disk-binaries)
* [First removal of Metal graphics drivers](#first-removal-of-metal-graphics-drivers)

## Battle against on-disk binaries

With the very first developer beta of macOS Big Sur, we see that Apple's moved away from on-disk binaries towards a massive binary called the [`dyld_shared_cache`](https://github.com/apple-oss-distributions/dyld/tree/main). This giant binary, located at `/System/Library/dyld`, hosts most of the frameworks binaries in macOS, interlinks them to aid with function calls, and tries to speed up many of the core userspace functions in macOS. This cache additionally strips a lot of the information from binaries, namely important portions used for stand-alone usage.

* Note: In macOS Monterey, Apple split the massive `dyld_shared_cache` into multiple parts to save on OS Update sizes.

Because of this shift to dyld usage, we no longer have framework binaries that we can use to downgrade in the future.

Up until after [11.0 Beta 6 (20A5364e)](https://archive.org/details/install-assistant-11.0-dp-6), Apple hadn't merged the majority of the graphics stack into the dyld just yet. Thus we're able to extract some binaries from this OS build, namely:

* MTLCompiler.framework
  * Only 3802 and 3902 are present, 31001 was never on-disk
* GPUCompiler.framework
  * Same 3802 and 3902, no 31001
  * Only partially available, libLLVM.dylib is inside the dyld
* Metal and OpenGL .bundles (from `/System/Library/Extensions`, GPU architecture specific)

While this seems like the last build we can grab binaries, we actually have an extra workaround. Specifically, macOS has a disk image called the RAM disk (SURamdisk). This is used during OS installs/updates, and on-disk binaries are available! However there's a catch, most of the frameworks are missing, and only a select few are present that are needed for the OS update stage.

* Currently up to, and including macOS Ventura, these RAM disks are still present and can be extracted from OS installers relatively easily inside of SharedSupport.dmg (located inside of `Install macOS.app/Contents/SharedSupport`)
  * `/Volumes/Shared Support/com_apple_MobileAsset_MacSoftwareUpdate/{random hex string, around 11GB~}.zip`:
    * `/AssetData/usr/standalone/update/ramdisk/x86_64SURamDisk.dmg`

The main binaries to extract from the RAM disk would be `Metal.framework`
* Regarding Metal.framework, keep in mind that `MTLCompilerService.xpc` is present on disk and not in the RAM disk.

Some of these remaining challenges were alleviated recently with [SentientBot's](https://github.com/ASentientBot) work on [DSCE](https://github.com/moraea/dsce). The goal of this tool is to reverse Apple's embedding and optimization of binaries inside the dyld. Thus even if a binary was never shipped standalone, we're able to still extract it.

* Notes:
  * Due to the nature of extracting, Apple's code signature is not retained and thus needs extra love to work with macOS's Library Validation.
  * DSCE was not available during Big Sur and Monterey's development of OpenCore Legacy Patcher. This tool was developed due to the dire situation we ended up with in macOS Ventura.
  * DCSE is not a perfect solution unfortunately due to how much information Apple strips during the creation of the `dyld_shared_cache`, which makes it more difficult when no original binaries are present to compare to.

## First removal of Metal graphics drivers

With macOS Monterey, Apple first removed the Ivy Bridge graphics stack and later on the Nvidia Kepler stack with 12.0 Beta 7. Here this is our first time experiencing perfectly functional drivers being removed from the OS.

With Ivy Bridge, the process was fairly straightforward once the binaries were acquired ([reference](https://github.com/dortania/OpenCore-Legacy-Patcher/blob/b0d4dd158f0e651f2cf815750323f9683a7d30ff/data/sys_patch_dict.py#L721-L747)):

```py
"/System/Library/Extensions": {
	"AppleIntelHD4000GraphicsGLDriver.bundle":  "11.0 Beta 6",
	"AppleIntelHD4000GraphicsMTLDriver.bundle": "11.0 Beta 6",
	"AppleIntelHD4000GraphicsVADriver.bundle":  "11.3 Beta 1",
	"AppleIntelFramebufferCapri.kext":          "11.4",
	"AppleIntelHD4000Graphics.kext":            "11.4",
	"AppleIntelIVBVA.bundle":                   "11.4",
	"AppleIntelGraphicsShared.bundle":          "11.4", # libIGIL-Metal.dylib pulled from 11.0 Beta 6
},
```

* .kext's were able to be pulled from the root volume in newer OSes easily, however most .bundles were not for the most part:
  * VA and Shared bundles are still present on the root volume, though verify all binaries are accounted for.
    * ex. AMD VA binaries were merged onto disk with 11.0 Beta 7
  * Usage of 11.3 Beta 1 is simply due to DRM strangeness with Ivy Bridge, unrelated to actual graphics acceleration

Thankfully with Monterey, no important compilers were dropped so only the GPU-specific graphics stack needed some work.

An additional note with Monterey is that this is the first time we've lost a Metal compiler, and actually didn't even notice from a patcher perspective. Specifically, the 3902 compiler inside of MTLCompiler.framework and GPUCompiler.framework were removed and the remaining 2 compilers are 3802 and 31001.

* We can see the majority of the 3902 functions were absorbed into 31001 allowing for seamless transitions for users.

While not confirmed, we believe the following about the compilers variants:

* 3802: 
  * Metal 1.x and 2.0
  * Introduced with macOS High Sierra (previously known as just MTLCompiler)
  * Removed with macOS Ventura
* 3809: 
  * Metal 2.1 (2.2 included with Catalina)
  * Introduced with macOS Mojave
  * Merged into 31001 with Monterey
* 31001:
  * Metal 2.3+ (3.0 included with Ventura)
  * Introduced with macOS Big Sur

# Where do we go from here?

Now that we understand the situation we've been in before, the next blog post will discuss the situation Apple threw us into with macOS Ventura and the challenges faced in restoring support for our older machines.
