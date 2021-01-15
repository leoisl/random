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
#include "memfd.h"

int new_memfd_region(const char *text_data, const char *name) {
    char *shm;
    const int shm_size = strlen(text_data)+1;
    int fd, ret;

    fd = memfd_create(name, MFD_ALLOW_SEALING);
    if (fd == -1)
        memfd_error("memfd_create()");

    ret = ftruncate(fd, shm_size);
    if (ret == -1)
        memfd_error("ftruncate()");

    ret = fcntl(fd, F_ADD_SEALS, F_SEAL_SHRINK);
    if (ret == -1)
        memfd_error("fcntl(F_SEAL_SHRINK)");

    shm = (char*)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED)
        memfd_error("mmap()");

    /* Write the data to memfd region*/
    sprintf(shm, "%s", text_data);

    /* Seal writes too, but unmap our shared mappings beforehand */
    ret = munmap(shm, shm_size);
    if (ret == -1)
        memfd_error("munmap()");
    ret = fcntl(fd, F_ADD_SEALS, F_SEAL_WRITE);
    if (ret == -1)
        memfd_error("fcntl(F_SEAL_WRITE)");

    ret = fcntl(fd, F_ADD_SEALS, F_SEAL_SEAL);
    if (ret == -1)
        memfd_error("fcntl(F_SEAL_SEAL)");

    return fd;
}
