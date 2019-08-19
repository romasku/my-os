# MY OS
###### (I did not decided how to name it)
This is small project about developing x86 kernel. Currently, I follow guides from https://wiki.osdev.org. I want to make it posix compliant as far as I can. 

## Features working:
- VGA terminal output
- Memory managment using paging (on for kernel space, as I don't have any user space). It uses my allocator from https://github.com/romasku/lpalloc.

## Building and running
Build system is cmake. I suggest using qemu to run kernel (it is multiboot compliant kernel).
### To Build:
```bash 
cmake CMakeLists.txt -B cmake-build-debug
cd cmake-build-debug
make
``` 
### To Run:
```
./tools/run-qemu.sh
```

### Dependencies
For building, you will need a i686-elf-gcc cross-compiler. Please refer to osdev wiki for details about installing it. Then you will need to modify CMakeLists.txt as currently it uses global path to it.
For running, please install qemu for you system. On arch linux, it can be done using `sudo pacman -S qemu`

### TODO
- Remove absolute path from build system
- Setup interrupts
- Implement keyboard driver
