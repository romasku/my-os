cmake_minimum_required(VERSION 3.14.0)
project(MYOS)

# Configuration
set(ARCH i386)
set(COMPILER_ARCH i686)
set(PLATFORM pc)

# CMAKE setup

enable_language(ASM_NASM)

# Build setup

set(CMAKE_C_COMPILER ${COMPILER_ARCH}-elf-gcc)
set(CMAKE_C_FLAGS "-O2 -flto -Wall -Wextra -ffreestanding -isystem ${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -felf32 -o <OBJECT> <SOURCE>")


# Load files to variables
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/${PLATFORM}/linker.ld)

file(GLOB_RECURSE KERNEL_SRCS kernel/*.c)
file(GLOB_RECURSE ARCH_SRCS arch/${ARCH}/${PLATFORM}/*.c)
file(GLOB_RECURSE ARCH_ASM_SRCS arch/${ARCH}/${PLATFORM}/*.asm)

# Prepare crt* objects properly
# Explaination for "What the hack is going on here":
# C/C++ runtime (de)initialization (honestly speaking not so required for c-only project)
# is done in crtbegin.o, crtend.o files, provided by gcc. But we have to call them manually,
# as in userspace world it is done in crt0.o crti.o, crtn.o files provided by c library (and
# we do not have one in kernel). Moreover, for crtbegin.o contains code not in symbol, but in
# directly in section .init. It is responsibility of crti.o, crtn.o files to wrap that code
# into normal function call. That leads to fact that we have to link in special order:
# crti.o crtbegin.o crtend.o crtn.o, as linker simply copy code in section in order files were
# provided to it.
# As crtbegin.o and crtend.o is provided as object files, the cmake treats them as libraries
# and moves to the end of linking list. To control linkin order, we have to do ctri and ctrn
# also object libraries.

set(CRTI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/${PLATFORM}/crt/crti.asm)
set(CRTN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/${PLATFORM}/crt/crtn.asm)

# Remove crti.asm and crtn.asm from all platform libraries so they do not link twice
list(REMOVE_ITEM ARCH_ASM_SRCS ${CRTI_SRC})
list(REMOVE_ITEM ARCH_ASM_SRCS ${CRTN_SRC})

# Grab crtbegin.o crtend.o file locations and strip trailing '\n'
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN)
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND)
string(STRIP ${CRTBEGIN} CRTBEGIN)
string(STRIP ${CRTEND} CRTEND)

# Setup ctri.o crtn.o as object libraries
add_library(crti OBJECT ${CRTI_SRC})
add_library(crtn OBJECT ${CRTN_SRC})

# Add target. In second line here we see proper order of c runtime ctr* objects.
add_executable(kernel.bin ${KERNEL_SRCS} ${ARCH_SRCS} ${ARCH_ASM_SRCS} /
               $<TARGET_OBJECTS:crti> ${CRTBEGIN} ${CRTEND} $<TARGET_OBJECTS:crtn>)

set_target_properties(kernel.bin PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT} -nostdlib")