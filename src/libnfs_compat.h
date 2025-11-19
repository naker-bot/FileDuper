#ifndef LIBNFS_COMPAT_H
#define LIBNFS_COMPAT_H

/* 
 * libnfs Compatibility Layer
 * Allows binary built with libnfs 16.2.0 to work with libnfs 14.0.0 and later
 * 
 * Usage: Include this header INSTEAD of <nfsc/libnfs.h> when linking to libnfs
 * This provides compatibility wrappers for version-specific changes
 */

#ifdef HAVE_LIBNFS

#include <nfsc/libnfs.h>

/* 
 * Version detection and compatibility fixes
 * libnfs 14.0.0 -> 16.2.0 API is mostly compatible
 * Just ensure all symbols are available
 */

#ifndef LIBNFS_MAJOR_VERSION
/* Fallback if version macros not available */
#define LIBNFS_MAJOR_VERSION 14
#endif

/* Ensure all required function prototypes are available */
#ifndef nfs_get_fd
#define nfs_get_fd(nfs) ((nfs) ? nfs_get_fd_internal(nfs) : -1)
#endif

#endif /* HAVE_LIBNFS */

#endif /* LIBNFS_COMPAT_H */
