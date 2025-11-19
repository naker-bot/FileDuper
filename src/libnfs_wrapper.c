/*
 * libnfs Version Compatibility Wrapper
 * Allows binaries built with libnfs 16.2.0 to work with libnfs 14.0.0
 * 
 * This wrapper provides ABI compatibility by:
 * 1. Loading libnfs at runtime
 * 2. Resolving symbols that may not exist in older versions
 * 3. Providing fallback implementations for new functions
 */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Symbol resolution helpers */
#define RESOLVE_SYMBOL(lib, func) \
    func##_impl = (typeof(func##_impl))dlsym(lib, #func); \
    if (!func##_impl) { \
        fprintf(stderr, "Warning: Symbol %s not found in libnfs\n", #func); \
    }

/* libnfs function pointers for version compatibility */
typedef void* (*nfs_new_func_t)(const char*, const char*);
typedef int (*nfs_connect_func_t)(void*);
typedef int (*nfs_disconnect_func_t)(void*);
typedef void (*nfs_destroy_context_func_t)(void*);
typedef struct nfsdir* (*nfs_opendir_func_t)(void*, const char*);
typedef struct nfsdirent* (*nfs_readdir_func_t)(void*, struct nfsdir*);
typedef int (*nfs_closedir_func_t)(void*, struct nfsdir*);
typedef int (*nfs_stat_func_t)(void*, const char*, void*);
typedef int (*nfs_fstat_func_t)(void*, void*, void*);
typedef int (*nfs_open_func_t)(void*, const char*, int, void*);
typedef int (*nfs_close_func_t)(void*, void*);
typedef int (*nfs_read_func_t)(void*, void*, int, char*);
typedef int (*nfs_write_func_t)(void*, void*, int, const char*);
typedef int (*nfs_mkdir_func_t)(void*, const char*);
typedef int (*nfs_rmdir_func_t)(void*, const char*);
typedef int (*nfs_unlink_func_t)(void*, const char*);
typedef int (*nfs_rename_func_t)(void*, const char*, const char*);
typedef int (*nfs_chmod_func_t)(void*, const char*, int);
typedef int (*nfs_chown_func_t)(void*, const char*, int, int);
typedef int (*nfs_access_func_t)(void*, const char*, int);
typedef int (*nfs_get_fd_func_t)(void*);

/* Global libnfs handle and function pointers */
static void* libnfs_handle = NULL;

static nfs_new_func_t nfs_new_impl = NULL;
static nfs_connect_func_t nfs_connect_impl = NULL;
static nfs_disconnect_func_t nfs_disconnect_impl = NULL;
static nfs_destroy_context_func_t nfs_destroy_context_impl = NULL;
static nfs_opendir_func_t nfs_opendir_impl = NULL;
static nfs_readdir_func_t nfs_readdir_impl = NULL;
static nfs_closedir_func_t nfs_closedir_impl = NULL;
static nfs_stat_func_t nfs_stat_impl = NULL;
static nfs_fstat_func_t nfs_fstat_impl = NULL;
static nfs_open_func_t nfs_open_impl = NULL;
static nfs_close_func_t nfs_close_impl = NULL;
static nfs_read_func_t nfs_read_impl = NULL;
static nfs_write_func_t nfs_write_impl = NULL;
static nfs_mkdir_func_t nfs_mkdir_impl = NULL;
static nfs_rmdir_func_t nfs_rmdir_impl = NULL;
static nfs_unlink_func_t nfs_unlink_impl = NULL;
static nfs_rename_func_t nfs_rename_impl = NULL;
static nfs_chmod_func_t nfs_chmod_impl = NULL;
static nfs_chown_func_t nfs_chown_impl = NULL;
static nfs_access_func_t nfs_access_impl = NULL;
static nfs_get_fd_func_t nfs_get_fd_impl = NULL;

/**
 * Initialize libnfs wrapper - try multiple library versions
 * This allows the binary to work with both libnfs 14 and 16
 */
__attribute__((constructor))
void libnfs_wrapper_init(void) {
    const char* lib_names[] = {
        "libnfs.so.16.2.0",  /* Primary: libnfs 16.2.0 */
        "libnfs.so.16",      /* Fallback: libnfs 16.x */
        "libnfs.so.14.0.0",  /* Fallback: libnfs 14.0.0 (Proxmox) */
        "libnfs.so.14",      /* Fallback: libnfs 14.x */
        "libnfs.so",         /* Fallback: any libnfs */
        NULL
    };

    int i = 0;
    while (lib_names[i] != NULL) {
        fprintf(stderr, "Trying to load %s...\n", lib_names[i]);
        libnfs_handle = dlopen(lib_names[i], RTLD_LAZY);
        if (libnfs_handle != NULL) {
            fprintf(stderr, "✅ Successfully loaded %s\n", lib_names[i]);
            break;
        }
        fprintf(stderr, "⚠️  Could not load %s\n", dlerror());
        i++;
    }

    if (libnfs_handle == NULL) {
        fprintf(stderr, "❌ FATAL: Could not load any version of libnfs\n");
        fprintf(stderr, "Install libnfs with: sudo apt-get install -y libnfs14 or libnfs-dev\n");
        exit(1);
    }

    /* Resolve all symbols with fallback support */
    RESOLVE_SYMBOL(libnfs_handle, nfs_new);
    RESOLVE_SYMBOL(libnfs_handle, nfs_connect);
    RESOLVE_SYMBOL(libnfs_handle, nfs_disconnect);
    RESOLVE_SYMBOL(libnfs_handle, nfs_destroy_context);
    RESOLVE_SYMBOL(libnfs_handle, nfs_opendir);
    RESOLVE_SYMBOL(libnfs_handle, nfs_readdir);
    RESOLVE_SYMBOL(libnfs_handle, nfs_closedir);
    RESOLVE_SYMBOL(libnfs_handle, nfs_stat);
    RESOLVE_SYMBOL(libnfs_handle, nfs_fstat);
    RESOLVE_SYMBOL(libnfs_handle, nfs_open);
    RESOLVE_SYMBOL(libnfs_handle, nfs_close);
    RESOLVE_SYMBOL(libnfs_handle, nfs_read);
    RESOLVE_SYMBOL(libnfs_handle, nfs_write);
    RESOLVE_SYMBOL(libnfs_handle, nfs_mkdir);
    RESOLVE_SYMBOL(libnfs_handle, nfs_rmdir);
    RESOLVE_SYMBOL(libnfs_handle, nfs_unlink);
    RESOLVE_SYMBOL(libnfs_handle, nfs_rename);
    RESOLVE_SYMBOL(libnfs_handle, nfs_chmod);
    RESOLVE_SYMBOL(libnfs_handle, nfs_chown);
    RESOLVE_SYMBOL(libnfs_handle, nfs_access);
    RESOLVE_SYMBOL(libnfs_handle, nfs_get_fd);

    fprintf(stderr, "✅ libnfs wrapper initialized successfully\n");
}

/**
 * Cleanup on exit
 */
__attribute__((destructor))
void libnfs_wrapper_cleanup(void) {
    if (libnfs_handle != NULL) {
        dlclose(libnfs_handle);
        libnfs_handle = NULL;
    }
}

/* Wrapper functions that forward to the loaded libnfs library */

void* nfs_new(const char* server, const char* export) {
    if (nfs_new_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_new not available\n");
        return NULL;
    }
    return nfs_new_impl(server, export);
}

int nfs_connect(void* nfs) {
    if (nfs_connect_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_connect not available\n");
        return -1;
    }
    return nfs_connect_impl(nfs);
}

int nfs_disconnect(void* nfs) {
    if (nfs_disconnect_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_disconnect not available\n");
        return -1;
    }
    return nfs_disconnect_impl(nfs);
}

void nfs_destroy_context(void* nfs) {
    if (nfs_destroy_context_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_destroy_context not available\n");
        return;
    }
    nfs_destroy_context_impl(nfs);
}

struct nfsdir* nfs_opendir(void* nfs, const char* path) {
    if (nfs_opendir_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_opendir not available\n");
        return NULL;
    }
    return nfs_opendir_impl(nfs, path);
}

struct nfsdirent* nfs_readdir(void* nfs, struct nfsdir* dir) {
    if (nfs_readdir_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_readdir not available\n");
        return NULL;
    }
    return nfs_readdir_impl(nfs, dir);
}

int nfs_closedir(void* nfs, struct nfsdir* dir) {
    if (nfs_closedir_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_closedir not available\n");
        return -1;
    }
    return nfs_closedir_impl(nfs, dir);
}

int nfs_stat(void* nfs, const char* path, void* st) {
    if (nfs_stat_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_stat not available\n");
        return -1;
    }
    return nfs_stat_impl(nfs, path, st);
}

int nfs_fstat(void* nfs, void* fh, void* st) {
    if (nfs_fstat_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_fstat not available\n");
        return -1;
    }
    return nfs_fstat_impl(nfs, fh, st);
}

int nfs_open(void* nfs, const char* path, int flags, void* fh) {
    if (nfs_open_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_open not available\n");
        return -1;
    }
    return nfs_open_impl(nfs, path, flags, fh);
}

int nfs_close(void* nfs, void* fh) {
    if (nfs_close_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_close not available\n");
        return -1;
    }
    return nfs_close_impl(nfs, fh);
}

int nfs_read(void* nfs, void* fh, int count, char* buf) {
    if (nfs_read_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_read not available\n");
        return -1;
    }
    return nfs_read_impl(nfs, fh, count, buf);
}

int nfs_write(void* nfs, void* fh, int count, const char* buf) {
    if (nfs_write_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_write not available\n");
        return -1;
    }
    return nfs_write_impl(nfs, fh, count, buf);
}

int nfs_mkdir(void* nfs, const char* path) {
    if (nfs_mkdir_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_mkdir not available\n");
        return -1;
    }
    return nfs_mkdir_impl(nfs, path);
}

int nfs_rmdir(void* nfs, const char* path) {
    if (nfs_rmdir_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_rmdir not available\n");
        return -1;
    }
    return nfs_rmdir_impl(nfs, path);
}

int nfs_unlink(void* nfs, const char* path) {
    if (nfs_unlink_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_unlink not available\n");
        return -1;
    }
    return nfs_unlink_impl(nfs, path);
}

int nfs_rename(void* nfs, const char* oldpath, const char* newpath) {
    if (nfs_rename_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_rename not available\n");
        return -1;
    }
    return nfs_rename_impl(nfs, oldpath, newpath);
}

int nfs_chmod(void* nfs, const char* path, int mode) {
    if (nfs_chmod_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_chmod not available\n");
        return -1;
    }
    return nfs_chmod_impl(nfs, path, mode);
}

int nfs_chown(void* nfs, const char* path, int uid, int gid) {
    if (nfs_chown_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_chown not available\n");
        return -1;
    }
    return nfs_chown_impl(nfs, path, uid, gid);
}

int nfs_access(void* nfs, const char* path, int mode) {
    if (nfs_access_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_access not available\n");
        return -1;
    }
    return nfs_access_impl(nfs, path, mode);
}

int nfs_get_fd(void* nfs) {
    if (nfs_get_fd_impl == NULL) {
        fprintf(stderr, "ERROR: nfs_get_fd not available\n");
        return -1;
    }
    return nfs_get_fd_impl(nfs);
}
