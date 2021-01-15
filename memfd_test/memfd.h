// Credits: https://github.com/a-darwish/memfd-examples/
// This code is adapted from https://github.com/a-darwish/memfd-examples/
//-----------------------------------------------------------------------------
/*
 * SPDX-License-Identifier: Unlicense
 *
 * No glibc wrappers exist for memfd_create(2), so provide our own.
 *
 * Also define memfd fcntl sealing macros. While they are already
 * defined in the kernel header file <linux/fcntl.h>, that file as
 * a whole conflicts with the original glibc header <fnctl.h>.
 */
#ifndef _MEMFD_H
#define _MEMFD_H

#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


inline void memfd_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int new_memfd_region(const char *text_data, const char *name);

#endif
