# OpenCore VM Images

Here are a few OpenCore images I've made for QEMU:

* [EFI-MODERN](https://github.com/khronokernel/khronokernel.github.io/blob/master/Binaries/EFI-MODERN.img.zip?raw=true)
  * macOS 10.15 and newer supported
  * SMBIOS used: MacPro7,1

* [EFI-LEGACY](https://github.com/khronokernel/khronokernel.github.io/blob/master/Binaries/EFI-LEGACY.img.zip?raw=true)
  * Supports x86_64 kernels
  * Mac OS X 10.6 through 10.14 supported
  * SMBIOS used: MacPro5,1

* [EFI-i386](https://github.com/khronokernel/khronokernel.github.io/blob/master/Binaries/EFI-i386.img.zip?raw=true)
  * Supports i386 kernels
  * Mac OS X 10.4 through 10.7 supported
  * SMBIOS used: MacPro2,1

### Known issues

* OS X 10.8 and older fails to boot with memory allocation errors
* macOS 10.13 will have extra `IOService stalls(240)`
* macOS 11 fails to boot with shutdown.c kernel panic for some users