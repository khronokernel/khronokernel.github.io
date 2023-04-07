---
layout: post
title: "Thunderbolt and PCIe Ethernet in macOS"
date: 2021-11-22 7:00:00 -0600
categories: macOS
---

So with the purchase of a 14" MacBook Pro, I've been in the market to buy a Thunderbolt dock to live to 1 cable life style. Unfortunately it's been a bit difficult to find much information on the parts used in many docks, thus I've decided to document my research on Ethernet controllers built into many Thunderbolt docks, with the goal of easily finding docks that use PCIe Ethernet.

* [Why PCIe over USB](#why-pcie-over-usb)
* [PCIe Driver Support](#pcie-driver-support)
* [Model Notes](#model-notes)
* [Dock Recommendations](#dock-recommendations)
  * [Best Bang for Buck](#best-bang-for-buck)
  * [Best Overall](#best-overall)
  * [Best for eGPU](#best-for-egpu)
* [Thunderbolt Ethernet Table](#thunderbolt-ethernet-table)
  * [USB](#usb)
  * [PCIe](#pcie)

## Why PCIe over USB

With macOS, Apple generally prioritizes PCIe devices over USB for better driver support as drivers are written explicitly for them (see next section). Whereas with USB, Apple writes semi-generic drivers based off USB CDC (Controller Device Class). As of Monterey, the following are supported:

* ACM: Abstract Control Model
* DMM: Device Management Model
* ECM: RNDIS, Ethernet Control Model
* EEM: Ethernet Emulation Model
* NCM: Network Control Model
* WCM: (WebCaM) Device Management Model

Additionally PCIe NICs generally offload much of the compute task from the CPU onto the on-board controller, giving your system some headroom during bandwidth intensive tasks. Compare this to USB, much of the compute task is still on the CPU side. 

* Note 1: Not all USB Ethernet devices are supported in macOS (namely AX88179) however the vast majority are thanks to this generic driver struture.
* Note 2: Apple may sometimes have additional support for certain USB Ethernet controllers via add-on drivers such as AppleUSBRealtek8153Patcher.kext

Finally the most noticeable issue with USB NICs is that they share bandwidth with the rest of your USB devices on the same controller. USB 3.0 for example has a shared bandwidth of 5Gbps, and when you have a dock with multiple devices plugged in, that takes up precious resources:

| Device | Bandwidth | Notes |
| :--- | :--- | :--- |
| USB to SATA HDD	| 3Gbps	| Lower end HDD (SATA 2 speeds) |
| USB to SATA SSD	| 5Gbps	| SATA 3 SSD (capped at USB 3.0) |
| USB Ethernet NIC	| 1Gbps	| Standard 1Gbe |
| USB Capture Card  | 5Gbps | |
| TOTAL	| 14Gbps	| |

As you can see, devices will be strained for bandwidth as we're exceeding the 5Gbps bandwidth allowed under USB 3.0. Removing devices from this chain can greatly alleviate contrains, allowing your NIC to run without compromise as well as allowing other USB devices to run faster.


## PCIe Driver Support

Currently in macOS Ventura, there are 3 PCIe Ethernet Vendors natively supported. All drivers in macOS natively support both Intel and Apple Silicon machines:

* Drivers can be found under `/System/Library/Extensions/IONetworkingFamily.kext/Contents/PlugIns/`

| Vendor   | Driver                           | Supported Architectures | Hardware                              |
| :-----   | :------------------------------- | :---------------------- | :------------------------------------ |
| Intel    | AppleIntelI210Ethernet.kext      | x86_64, arm64e          | i210, i225                            |
| Broadcom | AppleBCM5701Ethernet.kext        | x86_64, arm64e          | 5764M, 57761, 57762, 57765, 57766     |
| Aquantia | AppleEthernetAquantiaAqtion.kext | x86_64, arm64e          | AQC107, AQC113                        |

* As reported by Raje Punsalan, macOS Ventura adds 10GBe support via DriverKit for many Intel-based NICs. Currently they were able to confirm the X520-DA2 as functional, however this likely implies other NICs such as the X540 should be supported as well. Do proceed with caution however, as Apple may not have added support for all X5xx series NICs.

### Ventura Warning

With macOS Ventura, Apple removed support for the AppleIntel8254XEthernet and Intel82574L-based devices. You can try to re-add support by installing the older extension, however we'd recommend users avoid these NICs if possible. These Ethernet Controllers are natively supported in Monterey and older:

| Vendor   | Driver                           | Supported Architectures | Hardware                              |
| :-----   | :------------------------------- | :---------------------- | :------------------------------------ |
| Intel    | AppleIntel8254XEthernet.kext     | x86_64                  | 80003ES2LAN, 82545EM, 82571EB/82571GB |
| Intel    | Intel82574L.kext                 | x86_64, arm64e          | 82574L, 82566DC                       |


## Model Notes

With the above 5 drivers, currently Apple only uses 2 of them in their products:

* Aquantia is used on all Macs with 10Gbe
  * ie. 2017 iMac Pro, 2019 Mac Pro, 2018 Mac mini
* Broadcom is used on all 2011+ Macs with 1Gbe
  * ie. 2011-2020 iMacs, 2010-2020 Mac minis, 2013 Mac Pro

Apple previously used Intel with their 2006-2012 Mac Pro's:

* MacPro1,1-3,1 (2006-8): 8254X
* MacPro4,1-5,1 (2009-12): 82574L

However as of Ventura, Apple has removed these drivers. Additionally Apple maintains the i210 kexts solely for Thunderbolt docks, and i225 support was added into AppleIntelI210Ethernet.kext with macOS Catalina.

## Dock Recommendations

### Best Bang for Buck

The best bang for buck for the majority of users would be the Belkin Thunderbolt 3 Express Dock HD solely for the reason of being insanely cheap on eBay while still including essential features:

* Fresco Logic USB 3.0 Controller
* Intel i210 PCIe Ethernet Controller (1Gbe)
* 85w USB-PD Charging
* Single DisplayPort 1.2
* Thunderbolt 3 passthrough
  * can be used as additional DisplayPort output

It's a very bare bones dock, but if all you need is some USB ports, PCIe Ethernet and DisplayPort, this dock is a great value! Ensure you look on used markets such as eBay to get better pricing, as these docks usually sit under $100 CAD without PSUs or just over with them.

### Best Overall

From my research, I've found that the best dock overall in the field seems to be CalDigit's TS4 dock. The main reasons why:

* Includes 2 dedicated USB Controllers
  * Dual Fresco Logic USB 3.2 Controllers
* Intel i225 PCIe Ethernet Controller (2.5Gbe)
* 98w USB-PD Charging
* Single DisplayPort 1.4
* Dual Thunderbolt 4 passthrough
  * can be used as additional DisplayPort output
* SD 4.0 and UHS-II reader

If price is no issue, the TS4 surpasses every dock out there currently

### Best for eGPU

If you're an eGPU user, you've really only got 2 realistic options:

* TUL mini eGFX prototype
* Visiontek Expansion Chassis TB3 Mini eGFX

Both of these enclosures include an Intel i210 PCIe Ethernet Controller while still giving you the flexibility of a PCIe slot for eGPUs. However due to the 15w charging, you will need to supply the laptop with an additional cable.

## Thunderbolt Ethernet Table

### USB

| OEM          | Model                             | Power | Ethernet Controller | Connection | Notes                                          |
| :----------- | :-------------------------------- | :---- | :------------------ | :--------- | :--------------------------------------------- |
| Belkin       | Thunderbolt 3 Dock Pro            | 85w   | Realtek RTL8153     | USB        |                                                |
| CalDigit     | USB-C Pro Dock                    | 85w   | Realtek RTL8153     | USB        |                                                |
| ThinkPad     | Universal Thunderbolt 4 Dock      | 100w  | Realtek RTL8153     | USB        | DisplayPort MST, no multi-monitor in macOS     |
| OWC          | Thunderbolt 4 Dock                | 90w   | Realtek RTL8153     | USB        |                                                |
| Sonnet       | Echo 11 Thunderbolt 4             | 90w   | Realtek RTL8153     | USB        |                                                |
| Satechi      | Thunderbolt 4 Dock                | 96w   | Realtek RTL8153     | USB        |                                                |
| WAVLINK      | Thunderdock SP 3/Thunderdock SP 5 | 85w   | Realtek RTL8153     | USB        |                                                |
| Dell         | Thunderbolt Dock TB16             | 130w  | Realtek RTL8153     | USB        | 130w limited to Dell systems                   |
| Plugable     | Thunderbolt 3 USB-C Dual Display  | 96w   | Realtek RTL8153     | USB        |                                                |
| HP           | Thunderbolt Dock 120W G2          | 120w? | Realtek RTL8153     | USB        |                                                |
| Razer        | Thunderbolt 4 Dock Chroma         | 90w   | Realtek RTL8153     | USB        |                                                |
| Corsair      | TBT100 Thunderbolt 3 Dock         | 85w   | Realtek RTL8153     | USB        |                                                |
| Seagate      | Firecuda Gaming Dock              | N/A   | Realtek RTL8153     | USB        |                                                |
| Mantiz       | Saturn Pro                        | 97w   | Realtek RTL8153     | USB        |                                                |
| HP           | Omen Accelerator                  | 60w   | Realtek RTL8153     | USB        |                                                |
| WAVLINK      | Thunderdock SE/Thunderdock SE III | N/A   | Asix AX88179        | USB        | Requires 3rd party drivers in macOS            |
| Razer        | Core X Chroma                     | 100w  | Asix AX88179        | USB        | Requires 3rd party drivers in macOS            |

### PCIe

| OEM          | Model                             | Power | Ethernet Controller | Connection | Notes                                          |
| :----------- | :-------------------------------- | :---- | :------------------ | :--------- | :--------------------------------------------- |
| Apple        | Thunderbolt Display               | N/A   | Broadcom BCM57761   | PCIe       |                                                |
| Apple        | Thunderbolt 1 to Ethernet adapter | N/A   | Broadcom BCM57762   | PCIe       | Same NIC as 2020 M1 iMac                       |
| Belkin       | Thunderbolt 3 Express Dock HD     | 85w   | Intel i210          | PCIe       |                                                |
| Plugable     | Thunderbolt 3 Dock + 60W          | 60w   | Intel i210          | PCIe       |                                                |
| i-tec        | Thunderbolt 3 Dual 4K Dock + 60w  | 60w   | Intel i210          | PCIe       |                                                |
| CableMatters | Thunderbolt 3 Dock + 60w          | 60w   | Intel i210          | PCIe       |                                                |
| StarTech     | Thunderbolt 3 Dual-4K             | 15w?  | Intel i210          | PCIe       |                                                |
| StarTech     | Dual 4K Mini Thunderbolt 3 Dock   | N/A   | Intel i210          | PCIe       |                                                |
| StarTech     | Thunderbolt 3 Dock - Dual 4K 60Hz | 85w   | Intel i210          | PCIe       |                                                |
| Sonnet       | Echo 11 Thunderbolt 3             | 87w   | Intel i210          | PCIe       |                                                |
| OWC          | Thunderbolt 3 Dock (14 port)      | 85w   | Intel i210          | PCIe       |                                                |
| Targus       | Thunderbolt 3 DOCK220             | 85w   | Intel i210          | PCIe       |                                                |
| Mantiz       | Titan Thunderbolt 3 dock          | 85w   | Intel i210          | PCIe       |                                                |
| TUL          | mini eGFX prototype               | 15w   | Intel i210          | PCIe       |                                                |
| Visiontek    | Expansion Chassis TB3 Mini eGFX   | 15w   | Intel i210          | PCIe       |                                                |
| CalDigit     | TS3 Plus                          | 87w   | Intel i210          | PCIe       |                                                |
| CalDigit     | TS4                               | 98w   | Intel i225          | PCIe       | 2.5Gbe                                         |
| Goodway      | DBD1330                           | 96w   | Intel i225          | PCIe       | 2.5Gbe, must buy in bulk                       |
| ThinkPad     | Universal Thunderbolt 4 Dock      | 100w  | Intel i225          | PCIe       | 2.5Gbe, not officially available. DisplayPort MST |
| OWC          | Thunderbolt 3 Pro Dock            | 60w   | Aquantia AQC107     | PCIe       | 10Gbe, same NIC as 2017 iMac Pro/ 2019 Mac Pro |
| Akitio       | Thunder3 Dock Pro                 | 60w   | Aquantia AQC107     | PCIe       | 10Gbe, same NIC as 2017 iMac Pro/ 2019 Mac Pro |
| Sonnet       | Twin 10G SFP+                     | N/A   | Intel X540          | PCIe       | Apple Silicon support in Ventura (expected)    |
| HP           | Elite Thunderbolt 3 Dock          | 65w   | Broadcom BCM57786   | PCIe       | **Not supported in macOS**                     |

Editor's Note: As pointed out by a reader, Liam, the HP Elite Thunderbolt 3 Dock seems to have another variant with a Broadcom BCM57762-A1 which is natively supported in macOS. If you're able to verify the device before purchase, this may be another great option.
* Additionally it seems some HP docks are also mislabeled, and can be fixed with a firmware flash: [Fixing Ethernet on the HP Thunderbolt 3 Dock](https://zhongfu.li/posts/2020/01/fix-ethernet-on-hp-tb3-dock/)
* However other units may [report a BCM57786 chipset](https://h30434.www3.hp.com/t5/Notebook-Software-and-How-To-Questions/Thunderbolt-3-new-dock-station-no-Ethernet-controller-driver/td-p/6906807) which macOS does not support.
