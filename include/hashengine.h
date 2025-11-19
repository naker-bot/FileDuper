#ifndef HASHENGINE_H
#define HASHENGINE_H

#include <string>

class HashEngine {
public:
    HashEngine();
    ~HashEngine();
    
    std::string hashFile(const std::string& filePath);
};

#endif // HASHENGINE_H
