#!/bin/bash
PATH_TO_KERNEL="../cmake-build-debug/kernel.bin"
if ! test -f "$PATH_TO_KERNEL"; then
    PATH_TO_KERNEL="cmake-build-debug/kernel.bin"
fi
if ! test -f "$PATH_TO_KERNEL"; then
    echo "Cannot find 'kernel.bin' file."
    echo "Please check that kernel build was successfull."
    echo "Also, note that you can run this script only from project root directory or ./tools directory."
    exit 1
fi
qemu-system-x86_64 -monitor stdio -kernel $PATH_TO_KERNEL


