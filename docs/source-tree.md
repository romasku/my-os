# Source tree structure

Root directories:
 - kernel - main entry point and glues all subsystems
 - include - headers directory. #include <...> looks to this directory.
 - drivers - controllers for generic devices
 - arch - contains cpu and platform specific code.
 - tools - scripts for running/deploying this kernel
 - docs - all documentation about this kernel

## Kernel

Contains all platfrom-independend code. Here placed:
 - main.c - Main entry point (kernel_main should be called from arch boot routine after some boot initialization)
 - panic.c - Main error handler. Just hangs out system.
 - mm - Kernel Memory Manager. Controls paging and physical memory allocation.
 - lib - library code. Here placed some function from Standard C library.
 - tty - generic code for terminal I/O

## Arch

Code placed in dirs named %ARCH_NAME/%PLATFORM_NAME
Folders should contain:
 - linker.ld - linker script for platform
 - boot - entry point of the system.
 - crt - C runtime initialiazation code
All other folder will mimic contents of kernel folder.

## Include

Under kernel subfolder all kernel headers provided.