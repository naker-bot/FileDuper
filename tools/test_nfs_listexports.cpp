// Small CLI test to verify that listNfsExports works.
#include <iostream>
#include <vector>
#include <string>
extern std::vector<std::string> listNfsExports(const std::string& server);

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Skipping test_nfs_listexports: no server provided (skipping)" << std::endl;
        return 0; // treat as skip, not failure
    }

    std::string server = argv[1];
    auto exports = listNfsExports(server);
    if (exports.empty()) {
        std::cout << "No exports found (or showmount not available) for: " << server << std::endl;
        return 2;
    }

    std::cout << "Found exports for " << server << ":\n";
    for (auto& e : exports) {
        std::cout << "  " << e << std::endl;
    }

    return 0;
}
