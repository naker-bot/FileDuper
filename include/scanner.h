#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>

class Scanner {
public:
    Scanner();
    ~Scanner();
    
    void scanDirectory(const std::string& path);
};

#endif // SCANNER_H
