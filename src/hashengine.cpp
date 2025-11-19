#include "hashengine.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <fstream>
#include <iostream>
#include <cstring>

HashEngine::HashEngine() {
    std::cout << "[HashEngine] Initialized" << std::endl;
}

HashEngine::~HashEngine() {
}

std::string HashEngine::hashFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return "";
    
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    
    char buffer[8192];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        MD5_Update(&md5Context, buffer, file.gcount());
    }
    
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5Context);
    
    char hexResult[MD5_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&hexResult[i * 2], "%02x", result[i]);
    }
    hexResult[MD5_DIGEST_LENGTH * 2] = '\0';
    
    return std::string(hexResult);
}
