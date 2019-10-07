#!/bin/bash
PATH_TO_ROOT="../"
PATH_TO_KERNEL="cmake-build-debug/kernel.bin"
PATH_TO_DRIVE="disk.img"
if ! test -f "$PATH_TO_ROOT$PATH_TO_KERNEL"; then
    PATH_TO_ROOT=""
fi
if ! test -f "$PATH_TO_ROOT$PATH_TO_KERNEL"; then
    echo "Cannot find 'kernel.bin' file."
    echo "Please check that kernel build was successfull."
    echo "Also, note that you can run this script only from project root directory or ./tools directory."
    exit 1
fi
if ! test -f "$PATH_TO_ROOT$PATH_TO_DRIVE"; then
    dd if=/dev/zero of=$PATH_TO_ROOT$PATH_TO_DRIVE bs=1k count=50000
fi

qemu-system-x86_64 -monitor stdio -kernel $PATH_TO_ROOT$PATH_TO_KERNEL -drive id=disk,file=$PATH_TO_ROOT$PATH_TO_DRIVE,format=raw,if=ide

