# MY OS
###### (I did not decided how to name it)
This is small project about developing x86 kernel. Currently, I follow guides from https://wiki.osdev.org. I want to make it posix compliant as far as I can. 

## Features working:
- VGA terminal output
- Memory management using paging (on for kernel space, as I don't have any user space). It uses my allocator
 from https://github.com/romasku/lpalloc.

## Building and running
Build system is cmake. I suggest using qemu to run kernel (it is multiboot compliant kernel).
### To Build:
```sh
cmake CMakeLists.txt -B cmake-build-debug
cd cmake-build-debug
make
``` 
### To Run:
```sh
./tools/run-qemu.sh
```

### Dependencies
To be able to build, you'll need a i686-elf-gcc cross-compiler. On arch, you can try to use `i686-elf-gcc` AUR package. 
If you use other distro/platform, please refer to [osdev wiki](https://wiki.osdev.org/GCC_Cross-Compiler), it has 
comprehensive guide how to build cross compiler it.
After installation, please make sure that `i686-elf-gcc` executable is in your PATH. CMake script assume it is there.

To be able to run, please install qemu. On Arch linux, it can be done using `sudo pacman -S qemu`, on ubuntu, you can 
use `sudo apt install qemu`.

### TODO
- Setup interrupts
- Implement keyboard driver
