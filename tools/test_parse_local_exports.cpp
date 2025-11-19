#include <iostream>
#include "nfs_helpers.h"

int main(int argc, char **argv) {
    auto exports = parseLocalExports();
    if (exports.empty()) {
        std::cout << "No exports found (or /etc/exports not accessible)." << std::endl;
        return 0;
    }
    std::cout << "Local exports found:" << std::endl;
    for (auto &e : exports) {
        std::cout << "- " << e << std::endl;
    }
    return 0;
}
