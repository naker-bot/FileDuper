#include "nfs_helpers.h"
#include <vector>
#include <string>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <cstring>
#ifdef HAVE_LIBNFS
#include <nfsc/libnfs.h>
#include <nfsc/libnfs-raw-mount.h>
#endif

std::vector<std::string> listNfsExports(const std::string& server) {
    std::vector<std::string> exports;
    if (server.empty()) return exports;

    std::string cmd = "showmount -e " + server + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return exports;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string line(buffer);
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
        if (line.rfind("Export list for", 0) == 0) continue;
        std::istringstream iss(line);
        std::string path;
        if (iss >> path) {
            if (!path.empty()) exports.push_back(path);
        }
    }

    pclose(pipe);
    return exports;
}

bool isLibnfsAvailable() {
#ifdef HAVE_LIBNFS
    return true;
#else
    return false;
#endif
}

std::vector<std::string> listNfsDirectoryWithLibnfs(const std::string& server, const std::string& exportPath) {
    std::vector<std::string> result;
#ifdef HAVE_LIBNFS
    struct nfs_context *nfs = nfs_init_context();
    if (!nfs) return result;

    // Mount the export with libnfs (no local mount)
    if (nfs_mount(nfs, server.c_str(), exportPath.c_str()) != 0) {
        nfs_destroy_context(nfs);
        return result;
    }

    struct nfsdir* dir;
    if (nfs_opendir(nfs, "/", &dir) != 0) {
        nfs_destroy_context(nfs);
        return result;
    }

    struct nfsdirent *entry;
    while ((entry = nfs_readdir(nfs, dir)) != NULL) {
        if (strcmp(entry->name, ".") == 0 || strcmp(entry->name, "..") == 0) continue;
        result.push_back(std::string(entry->name));
    }

    nfs_closedir(nfs, dir);
    nfs_destroy_context(nfs);
#endif
    return result;
}

bool isCommandAvailable(const std::string& cmd) {
    if (cmd.empty()) return false;
    std::string pathCheck = "which " + cmd + " >/dev/null 2>&1";
    int rc = system(pathCheck.c_str());
    return (rc == 0);
}

// Read /etc/exports and return first token from each non-empty, non-comment line
std::vector<std::string> parseLocalExports() {
    std::vector<std::string> exports;
    FILE* f = fopen("/etc/exports", "r");
    if (!f) return exports;

    char buf[2048];
    while (fgets(buf, sizeof(buf), f)) {
        std::string line(buf);
        // Trim carriage returns/newlines
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();

        // Skip comments and empty lines
        size_t i = 0;
        while (i < line.size() && isspace((unsigned char)line[i])) ++i;
        if (i >= line.size() || line[i] == '#') continue;

        // Extract first token (export path)
        std::istringstream iss(line);
        std::string path;
        if (iss >> path) {
            if (!path.empty()) exports.push_back(path);
        }
    }

    fclose(f);
    return exports;
}
