---
layout: post
title: "OpenCore Legacy Patcher Retrospective & Next Chapter"
date: 2025-06-20 7:00:00 -0600
categories: macos
---

Seeing that this year will be OpenCore Legacy Patcher's 5th anniversary and that there's some bitter-sweet news to share ([more at the end](#finally-some-bittersweet-news)), I thought it'd be a good time to reflect on my time with OpenCore Legacy Patcher.

--------------

* [Background](#background)
* [Notable Milestones](#notable-milestones)
* [Bittersweet news](#finally-some-bittersweet-news)


## Background

For those who are unfamiliar (impressed you found this blog...), [OpenCore Legacy Patcher](https://github.com/dortania/OpenCore-Legacy-Patcher){:target="_blank"} was a project I co-authored with my good friend [DhinakG](https://github.com/dhinakg){:target="_blank"} back in 2020. The general idea was this: How do you get macOS Big Sur running on unsupported Macs?

The reason this question even came up was that we saw the legendary [dosdude1](https://github.com/dosdude1){:target="_blank"} was taking a step back from macOS patcher development with Big Sur ([later confirming such](https://forums.macrumors.com/threads/macos-11-big-sur-on-unsupported-macs-thread.2242172/page-250?post=29236680#post-29236680){:target="_blank"}). Thus a decent hole was left where several patchers took their place: [Barry KN’s micropatcher](https://github.com/barrykn){:target="_blank"}, [Todd Bruss' Big Mac Patcher](https://github.com/SuperBox64/bigmac){:target="_blank"} and [Ben Sova's Patched Sur](https://github.com/Ben216k/Patched-Sur){:target="_blank"} to name a few.

With Dhinak and I, we were heavily based in the [Hackintosh world](https://dortania.github.io/OpenCore-Install-Guide/){:target="_blank"} however saw an opportunity where Acidanthera's [OpenCorePkg](https://github.com/acidanthera/OpenCorePkg){:target="_blank"}{:target="_blank"} could step in and provide a more dynamic solution through in-memory patching. While originally a proof of concept to prove someone wrong online (as all good projects do 😛), we kept building on it as it was able to handle edge cases other patchers couldn't (native software updates, model spoofing, ACPI patching, etc).

Through that, more amazing developers and community members jumped in including ASentientBot, EduCovas, Jazzzny, Flagers, Ausdauersportler, IronApple, UHDBit, thatstella7922, crystall1nedev, Socamx, Paradox94, ASentientHedgehog, Mr.Macintosh, and so many more! (And we can't forget the amazing work from the Acidanthera team, which without Lilu, OpenCorePkg, etc, we wouldn't have been able to build OpenCore Legacy Patcher!)

A little tidbit of history: OpenCore Legacy Patcher was originally never even written on a genuine Mac. Up to this point, I avoided Apple hardware. Instead, the original proof of concept was written between 2 Hackintoshes, an Intel X299 desktop inside a PowerMac G5 case and an HP Elite X2 G1 tablet running Big Sur's beta:

| Desktop | Tablet |
| --- | --- |
| ![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-Dev-Desktop.png) | ![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-Dev-Tablet.png) |


## Notable milestones

### 2020/12/01 - v0.0.1 - Text based interface

The very first release of OpenCore Legacy Patcher! All TUI-based, and requiring Python 2 (which used to ship with macOS 🥲). This build "supported" the following models for macOS Big Sur:

* There were ofc many pitfalls that were later addressed.

| Model Identifier | Year|
|---|---|
| MacBook5,1 - MacBook7,1 | 2008 - 2010 MacBook |
| MacBookAir2,1 - MacBookAir5,x | 2009 - 2012 MacBook Air |
| MacBookPro4,1 - MacBookPro10,x | 2008 - 2012 MacBook Pro |
| Macmini3,1 - Macmini6,x | 2009 - 2012 Mac mini |
| iMac7,1 - iMac14,x (excluding 14,4) | 2007 - 2013 iMac |
| MacPro3,1 - MacPro5,1 | 2008 - 2012 Mac Pro |
| Xserve3,1 | 2009 Xserve |

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.0.1.png)


### 2021/03/01 - v0.0.11 - Python 3 rewrite and stand-alone binary

Thanks to some massive reworking from [DhinakG](https://github.com/dhinakg){:target="_blank"}, OpenCore Legacy Patcher's codebase now supports Python 3. And with it, integration with PyInstaller allowing for stand alone binaries without the need for external dependancies!

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.0.11.png)


### 2021/03/27 - v0.0.19 - Code signing and notarization

Somehow got Apple to sign and notarize OpenCore Legacy Patcher! Fun fact: You can bypass a bunch of Apple's checks by encrypting your resources 😛

* And yes, the DMG's password has in fact always been ["password"](https://github.com/dortania/PatcherSupportPkg/blob/1.9.2/Generate-DMG.command#L16){:target="_blank"}.

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.0.19.png)


### 2021/04/26 - v0.1.1 - macOS Big Sur non-Metal graphics support

What seemed like out of left field, [ASentientBot](https://github.com/asentientbot){:target="_blank"} was able to get non-Metal graphics working! Much of the community thought it was a dead end with macOS Big Sur.

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.1.1.png)


### 2021/06/14 - v0.1.7 - macOS Monterey and TeraScale 2 graphics support

2 big changes in one release! Regarding TeraScale 2 support, we were the first patcher to get it working on anything newer than macOS 10.13, High Sierra! However all the credit goes to [ASentientBot](https://github.com/asentientbot){:target="_blank"}.

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.1.7.png)

### 2021/07/11 - v0.2.3 - dosdude1 GUI

dosdude1's return to macOS patching through OpenCore Legacy Patcher! This time with a CLI wrapper written in Objective-C:

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.2.3.png)


### 2021/09/23 - v0.2.5 - macOS Monterey non-Metal graphics support

Miracles can happen twice! Of course, the credit goes to [ASentientBot](https://github.com/asentientbot){:target="_blank"} and EduCovas for their amazing work!

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.2.5.png)


### 2021/12/13 - v0.3.2 - Restoring the 5k iMac's Dual DisplayPort Stream

Really fun challenge to work on, and super thankful for [@turbomacs](https://github.com/turbomacs){:target="_blank"} for donating a 5k iMac for research! Even got a fun blog post out of it:

* [Blogpost: 5K iMac and UEFI: Fixing the dreaded output limitation](https://khronokernel.com/macos/2021/12/08/5K-UEFI.html){:target="_blank"}

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.3.2.png)

### 2022/01/21 - v0.4.0 - Native wxPython GUI

After much work, we built a GUI that was able to interoperate with OpenCore Legacy Patcher's core much closer through wxPython. Though if I had my way, we'd always be a TUI-based patcher 😅

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.4.0.png)


### 2022/05/02 - v0.4.4 - Automatic OS patching

Thanks to [DhinakG](https://github.com/dhinakg){:target="_blank"}'s research, we were able to add automatic root volume patching during OS installation, and detection of missing patches post-update!

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.4.4.png)


### 2022/06/11 - v0.4.6 - NVIDIA Web Drivers support

Everyone thought Web Drivers were dead after macOS 10.13 , High Sierra ([including myself](https://imgur.com/zNdTy3j){:target="_blank"}). Well with some black magic, [ASentientBot](https://github.com/asentientbot){:target="_blank"} and [Flagers](https://github.com/flagersgit){:target="_blank"} were able to do the impossible:

* [Twitter: In today’s installment of questionable projects, got #Nvidia Web Drivers working in #macOS #Monterey!](https://twitter.com/khronokernel/status/1529583832663437312){:target="_blank"}

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.4.6.png)


### 2022/10/25 - v0.5.0 - macOS Ventura support

This release has a fun little backstory, specifically that it was the first macOS unveil where [Mr.Macintosh](https://mrmacintosh.com){:target="_blank"} and I were at Apple Park! With live macOS patching shenanigans.

And a big thank you to those who funded my GoFundMe to get there. I was originally quite hesitant to ask for any support, but Mr.Macintosh encouraged me to do so. Honestly a once in a lifetime opportunity!

| Tim and Craig! | 2012 MacBook Pro installing Ventura beta 1! |
|---|---|
| ![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.5.0-Apple-Park-Tim-and-Craig.png) | ![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.5.0-Apple-Park-Patching.png) |

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.5.0.png)


### 2022/10/27 - v0.5.1 - Restoring the Trash Can Mac Pro's firmware support

Similar to the 5k iMac, [JohnD](https://forums.macrumors.com/members/johnd.53633/){:target="_blank"} graciously donated a 2013 Mac Pro for research as it was unable to boot macOS Ventura. After much fighting, we were finally able to get it to boot!

* [Twitter: For the first time outside of Cupertino Headquarters, a 2013 Trash Can Mac Pro booted macOS Ventura!](https://twitter.com/khronokernel/status/1585470323998457856){:target="_blank"}


![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.5.1.png)


### 2023/01/23 - v0.6.0 - macOS Ventura Rapid Security Response and non-Metal graphics support

Another 2 for one, this time fighting my entire winter break from school on Apple's shiny new Rapid Security Response. As usual, huge shout out to EduCovas, [ASentientBot](https://github.com/asentientbot){:target="_blank"}, [Flagers](https://github.com/flagersgit){:target="_blank"} and [DhinakG](https://github.com/dhinakg){:target="_blank"} for their amazing work on this release!

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.6.0.png)


### 2023/05/22 - v0.6.6 - Overhauled wxPython GUI

Now with more icons! My personal favorite being the [support icon](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.6.6-Support-Icon.png){:target="_blank"}.

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.6.6.png)


### 2023/06/02 - v0.6.7 - Launching our OpenCollective

After many requests, we finally launched our [OpenCollective](https://opencollective.com/opencore-legacy-patcher){:target="_blank"}! With it, the community helped fund research and fixes through hardware and software purchases. This was especially important for our developers in less fortunate countries. Thank you to everyone who donated!

And a big thank you to [Mr.Macintosh](https://mrmacintosh.com){:target="_blank"} for pushing us to do this!

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-0.6.7.png)


### 2023/10/02 - v1.0.0 - macOS Sonoma support

Our 4th OS release, now with saner [semantic versioning](https://www.geeksforgeeks.org/introduction-semantic-versioning/){:target="_blank"} 🎉 (Though throwing darts for versioning was kinda fun while it lasted)

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-1.0.0.png)


### 2023/11/10 - v1.2.0 - Background caching of patcher assets

After many conversations with [Mr.Macintosh](https://mrmacintosh.com){:target="_blank"} on how to improve the patching experience, we were able to implement OS update detection for asset caching with items requiring network connections (ie. [KdkSupportPkg](https://github.com/dortania/KdkSupportPkg){:target="_blank"})!

And around this same time, I also spoke a bit about the internals of making a macOS patcher and what it means to be a "tier 2 citizen" at BSides Calgary (my very first talk!): [BSides Calgary 2023 Talk: Legacy Macs, Modern Solutions. A Hacker's Approach to Mac Sustainability.
](https://khronokernel.com/macos/2023/11/16/BSIDES.html){:target="_blank"}

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-1.2.0.png)


### 2024/05/31 - v1.5.0 - Privileged Helper Tool and PKG-based distribution

After having spent the last year working as a MacAdmin, I grew quite an interest in PKG deployments. So of course, I converted OpenCore Legacy Patcher's installation system into a streamlined PKG system.

And with it, a new privileged helper tool to drop the annoying password requirement for many of the patcher's functions.


| Installer | Uninstaller |
| --- | --- |
| ![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-1.5.0-Installer.png) | ![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-1.5.0-Uninstaller.png) |



### 2024/09/14 - v2.0.0 - macOS Sequoia support

Supporting our 5th OS release 🦾, and still, not a single machine we previously supported was removed. Though unfortunately we've still been unable to get the 2018 and 2019 MacBook Airs supported due to [firmware shenanigans](https://github.com/dortania/OpenCore-Legacy-Patcher/issues/1136){:target="_blank"} 🫠

![](/images/posts/2025-06-20-OCLP-RETROSPECTIVE/OCLP-2.0.0.png)


## Finally some bittersweet news

After some genuinely amazing years of working on OpenCore Legacy Patcher, I believe it's time to pivot my work on breaking Apple platforms in a new direction. Specifically towards a small, Cupertino-based startup.

Beginning next week, I'll be joining Apple's Bug Bounty team out in Seattle (I'll miss you Canada 🫎). It's quite the shift, but excited for the challenges to come!

> What does this mean for OpenCore Legacy Patcher?

Hopefully very little, as there are still going to be a ton of brilliant members working on the patcher. The project will be in good hands.

Otherwise thank you to all the people who've helped build OpenCore Legacy Patcher, as well as the community that supported us throughout. I wish the best of luck with the future of macOS 🫡
