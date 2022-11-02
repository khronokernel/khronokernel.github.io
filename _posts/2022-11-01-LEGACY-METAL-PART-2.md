---
layout: post
title: "macOS Ventura and legacy Metal: Part 2; New challenges with Ventura"
date: 2022-11-01 7:00:01 -0600
categories: macOS
---

With part 2 of this blog post, we'll be going over the challenges introduced with macOS Ventura and unsupported Metal GPUs. If you haven't read part 1, you can find it here: [macOS Ventura and legacy Metal: Part 1; Catching up on the past](./LEGACY-METAL-PART-1.md)

----------

* [Back to the present: Ventura and legacy Metal](#back-to-the-present-ventura-and-legacy-metal)
  * [Dropped GPUs](#dropped-gpus)
  * [Dropped Compilers](#dropped-compilers)
* [Developing Patches](#developing-patches)
  * [Extracting 3802 Compilers](#extracting-3802-compilers)
  * [Resolving Compiler Calls](#resolving-compiler-calls)
  * [Adjusting Struct Offsets](#adjusting-struct-offsets)
  * [Miscellaneous Oddities](#miscellaneous-oddities)

# Back to the present: Ventura and legacy Metal

Now that we've discussed the challenges we've faced in the past relating to legacy Metal, we can now discuss how Ventura's made our situation more difficult:

* [Dropped GPUs](#dropped-gpus)
* [Dropped Compilers](#dropped-compilers)

## Dropped GPUs

With Ventura, the following GPUs have been removed:

* Intel Haswell, Broadwell, and Skylake
* AMD GCN 1, 2 and 3

This is in addition to the previously removed Graphics Architectures being Intel Ivy Bridge and Nvidia Kepler in Monterey. The most important note from this info is that macOS Ventura no longer officially supports any Metal 1 Macs.

## Dropped Compilers

And the area we're most concerned about is the dropped compilers:

* 3802 MTLCompiler.framework
* 3802 GPUCompiler.framework
  * Present in 13.0 Beta 1, removed in Beta 2

Originally we were unsure whether the removal of these compilers affects us, namely because Metal 2 GPUs do not seem to invoke the 3802 compilers in Monterey. Only Metal 1 GPUs will occasionally use 3802, but the majority of the time rely on 31001.

* You can see which compiler is being used by monitoring `MTLCompiler` in console, and expanding on the path (click the header above `Subsystem`)
* Later on we'll notice what scenarios require the legacy 3802 stack

In addition to this, Apple has deprecated the `MTLGPUFamilyMac1` class from MTLGPUFamily in Ventura:

* [MTLGPUFamilyMac1](https://developer.apple.com/documentation/metal/mtlgpufamily/mtlgpufamilymac1?language=objc)
  * macOS 10.15–13.0 Deprecated
  * Note: Deprecation does not mean removal, however, new Ventura-specific applications will target Metal 2 by default

# Developing Patches

Now that we know some challenges ahead of us, we can start working on developing patches for Ventura. 

The main hurdles we need to overcome:

* [Extracting 3802 Compilers](#extracting-3802-compilers)
* [Resolving Compiler Calls](#resolving-compiler-calls)
* [Adjusting Struct Offsets](#adjusting-struct-offsets)
* [Miscellaneous Oddities](#miscellaneous-oddities)


## Extracting 3802 Compilers

The first challenge we face is obtaining 3802 Compilers for our Metal 1 GPUs. As we discussed in [Part 1](./LEGACY-METAL-PART-1.md), we don't have access to any semi-modern compilers. Thus we need to find a way to extract them from Apple's `dyld_shared_cache`.

Thanks to the amazing work of [SentientBot](https://github.com/ASentientBot), the [DSCE](https://github.com/moraea/dsce) project was written explicitly in mind for extracting binaries from this cache. Thus with a simple incantation, we can extract any binary we need from the cache:

```bash
DSC_Extract ~/Developer/dyld_repo/12.5/dyld_shared_cache_x86_64 /System/Library/PrivateFrameworks/MTLCompiler.framework /System/Library/PrivateFrameworks/GPUCompiler.framework
```

And from here, our biggest technical hurdle for legacy Metal has been resolved!

## Resolving Compiler Calls

Now that we've gotten our 3802 binaries, we next need to ensure macOS properly uses them. The easiest way to find this by following a crash log on Ventura with a Metal 1 Mac, booting with the stock GPU/MTLCompiler:

* [MTLCompilerService Crash](https://pastebin.com/jUT8cijj)

```
Thread 0 Crashed::  Dispatch queue: com.apple.main-thread
0   libsystem_kernel.dylib        	    0x7ff8065f437e __pthread_kill + 10
1   libsystem_pthread.dylib       	    0x7ff80662c20f pthread_kill + 263
2   libsystem_c.dylib             	    0x7ff806575cb8 abort + 123
3   SkyLight                      	    0x7ff80b5d3c94 MetalShader::CopyPipelineState(MetalContext*, bool, bool) + 3228
4   SkyLight                      	    0x7ff80b72e90a MetalTiledBacking::RenderToDestination(MetalContext*, WSCompositeSourceLayer*, WSCompositeDestination*) + 638
5   SkyLight                      	    0x7ff80b739335 MetalCompositeLayers + 41
6   SkyLight                      	    0x7ff80b738da6 CompositorMetal::composite(WSCompositeSourceLayer*, WSCompositeDestination*, bool, bool) + 326
7   SkyLight                      	    0x7ff80b733911 CompositorMetal::CompositeLayersToDestination(WSCompositeSourceLayer*, WSCompositeDestination*, unsigned long long) + 239
8   SkyLight                      	    0x7ff80b5fdf8a WS::Displays::CDDisplay::render_update(WS::Displays::Update&, Compositor*, unsigned int, WSCompositeSourceLayer*) + 876
9   SkyLight                      	    0x7ff80b69ce88 WS::Updater::UpdateDisplays(CGXConnection*, bool) + 19176
10  SkyLight                      	    0x7ff80b68e616 update_display_callback(void*, double) + 1644
11  SkyLight                      	    0x7ff80b70adbe run_timer_pass + 502
12  SkyLight                      	    0x7ff80b70aed5 non_coalesced_timer_handler + 16
13  SkyLight                      	    0x7ff80b747a87 post_port_data + 250
14  SkyLight                      	    0x7ff80b747724 CGXRunOneServicesPass + 1672
15  SkyLight                      	    0x7ff80b748153 server_loop + 91
16  SkyLight                      	    0x7ff80b747fb3 SLXServer + 1233
17  WindowServer                  	       0x10d85848c 0x10d855000 + 13452
18  dyld                          	    0x7ffb21462105 start + 2325
```

Tracing back the errors, we get the following:

```c++
// Metal.framework/Versions/A/XPCServices/MTLCompilerService.xpc/Contents/MacOS/MTLCompilerService
   MTLConnectionCtx::MTLConnectionCtx()               -> __ZN16MTLConnectionCtxC2Ei()
   __ZN16MTLConnectionCtxC2Ei                         -> CompilerPluginInterface::CompilerPluginInterface()
   CompilerPluginInterface::CompilerPluginInterface() -> __ZN23CompilerPluginInterfaceC2Ei()
```

When we throw MTLCompilerService into a decompiler, we see the following:

| Ventura 13.0 Beta 2 Function | Monterey 12.4 Function |
| :--- | :--- |
| ![](/images/posts/2022-11-01-LEGACY-METAL/13.0-CompilerPluginInterface.png) | ![](/images/posts/2022-11-01-LEGACY-METAL/12.4-CompilerPluginInterface.png) |

Let's rewrite this function into something easier to read:

```c++
int CompilerPluginInterface(int BUILD_VERSION) {
   
   char compiler_plugin_path[] = "";

   // Determine the path to the compiler plugin
    if (BUILD_VERSION == 31001) {
        compiler_plugin_path = "/System/Library/PrivateFrameworks/MTLCompiler.framework/Versions/31001/MTLCompiler";
    } else if (BUILD_VERSION == 3802) {
        compiler_plugin_path = "/System/Library/PrivateFrameworks/MTLCompiler.framework/Versions/3802/MTLCompiler";
    }

    // dlopen the compiler plugin
    void *handle = dlopen(compiler_plugin_path, RTLD_LAZY);
    if (handle) {
        // Starts compiling
   } else {
      // Error Logging
   }
}
```

From the above, we see that MTLCompilerInterface no longer checks for the 3802 path in Ventura. Thus to support Metal 1 GPUs, we'll want to downgrade the MTLCompilerService.xpc service.

However once downgraded, we'll get Sandboxing errors, thus to resolve this we'll add a generic Sandbox profile:

```
/System/Library/Sandbox/Profiles/com.apple.mtlcompilerservice.sb
```
```
(version 1)

(allow default)
```

Credit to [flagers](https://github.com/flagersgit) for the original finding on Sandbox profiling. Error message without the Sandbox Profile patch:
```
MTLCompilerService: (libsandbox.1.dylib) sandbox compile: invalid data type of path filter; expected pattern, got boolean
```

## Adjusting Struct Offsets

The final major hurdle we're presented with is resolving Struct Offsets relating to Metal.framework. Currently the 2 graphics architectures that don't support the native Metal.framework stack is Nvidia Kepler and AMD GCN 1-3.

* Fun fact: When 12.5 Beta 3 broke Nvidia Kepler support, this was because of changed offsets as well

Thanks to the amazing work of the aforementioned [ASentientBot](https://github.com/ASentientBot), they developed patches to fix the offsets to ensure they point to the correct locations in Metal.framework:

* [Nvidia Kepler Struct Patches](https://github.com/moraea/amys-hacking-tutorial/blob/e60c7df9ee9eb4954f1ac38c8d6d64839a9aa680/kelper%20without%20metal%20downgrade/GeForce.tool)
* [AMD GCN 1-3 Struct Patches](https://github.com/moraea/amys-hacking-tutorial/blob/18f3ede317b6edec3069f73e246c77067225c1c1/gcn%20without%20metal%20downgrade/GCN.tool)

Once these were fixed, the stock Metal.framework could be used.

## Miscellaneous Oddities

While developing Ventura support for OpenCore Legacy Patcher, we found some additional oddities. Not enough to write in detail, however interesting nonetheless:

* Intel Broadwell is labeled as Metal 1, yet is a 31001 GPU
  * Thus is treated as Metal 2, and Metal 1 bugs do not occur
* AMD Legacy GCN with stock Metal.framework generates corrupted translucent image caches
  * Forcing WindowServer into not caching the rendered files ensures proper opaque images are always created
* Hardware Encoding/Decoding on Legacy GCN is currently unsupported
  * Unfortunately we're unable to extract functional VADriver and VADriver2 for these GPUs
* Crashes in Weather, Fontbook, and News Widgets may occur on Intel Ivy Bridge and Haswell iGPUs
  * This is due to RenderBox.framework getting botched data from GPUCompiler's libLLVM.dylib

Other bugs relating to the beta patches can be found on the associated Github Issue:

* [Legacy Metal Graphics Support and macOS Ventura](https://github.com/dortania/OpenCore-Legacy-Patcher/issues/1008)

# Results

And with much anticipation, we were able to get legacy Metal GPUs working in macOS Ventura! 

| 2012 Mac mini (HD4000) | 2014 Mac mini (Iris 5100) |
| :--- | :--- |
| ![](/images/posts/2022-11-01-LEGACY-METAL/Macmini61.png) | ![](/images/posts/2022-11-01-LEGACY-METAL/Macmini71.png) |

| 2013 Mac Pro (D300) | 2008 Mac Pro (GT710) |
| :--- | :--- |
| ![](/images/posts/2022-11-01-LEGACY-METAL/METAL-MacPro61.png) | ![](/images/posts/2022-11-01-LEGACY-METAL/METAL-MacPro31-Kepler.png) |


# Closing Thoughts

Having spent the last 4+ months working on macOS Ventura, I must say it's a relief knowing we've been able to achieve such a monumentous feat.

I do want to thank everyone who've contributed to development either through research and development, providing insight, or simply by offering their machines up for internal testing. Without these amazing people, we would not be able to get this far!

* [ASentientBot](https://github.com/asentientbot), [EduCovas](https://github.com/educovas), [Flagers](https://github.com/flagers), [Crystal](https://github.com/crystall1nedev), [ParaDoX](https://github.com/ParaDoX1994), [IronApple](https://github.com/ironapple0915), [Jazzzny](https://github.com/jazzzny), [UHDBits](https://github.com/UHDBits), [DhinakG](https://github.com/dhinakg), [Ausdauersportler](https://github.com/Ausdauersportler) and many more!

Otherwise, it's time to get back to development! Hope everyone's excited for what's to come!


