# CMAKE Integration Checklist

## Files to Add to CMakeLists.txt

### Headers (in `target_sources` or `set(SOURCES ...)`)

```cmake
include/curlmultimanager.h
include/curlmulti_benchmark.h
include/ftpclient.h
```

### Sources (in `target_sources` or `set(SOURCES ...)`)

```cmake
src/curlmultimanager.cpp
src/curlmulti_benchmark.cpp
src/ftpclient.cpp              # ← UPDATED VERSION
```

---

## CMakeLists.txt Example

```cmake
cmake_minimum_required(VERSION 3.16)
project(FileDuper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find packages
find_package(Qt6 COMPONENTS 
    Core Gui Widgets Network Concurrent
    REQUIRED
)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)  # For SFTP/HTTPS if needed

# Add target
add_executable(FileDuper
    # ... existing sources ...
    
    # curl_multi support (NEW)
    include/curlmultimanager.h
    src/curlmultimanager.cpp
    include/curlmulti_benchmark.h
    src/curlmulti_benchmark.cpp
    
    # Updated FTP client
    include/ftpclient.h
    src/ftpclient.cpp
    
    # ... other sources ...
)

# Link libraries
target_link_libraries(FileDuper 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::Network 
    Qt6::Concurrent
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
)

# Include directories
target_include_directories(FileDuper PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CURL_INCLUDE_DIR}
)
```

---

## Expected Compiler Flags

The following flags are automatically set by CMake for curl_multi:

```
-DCURL_STATICLIB           # If using static curl
-I/usr/include/curl        # curl headers
-L/usr/lib/x86_64-linux-gnu  # curl libraries
-lcurl                     # Link libcurl
```

---

## Build Steps

```bash
# 1. Create build directory
mkdir -p build && cd build

# 2. Configure with cmake (includes updated CMakeLists.txt)
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. Build
make -j$(nproc)

# 4. (Optional) Install
sudo make install
```

---

## Verify Build Success

After building, verify the new components are compiled:

```bash
# Check for curl_multi symbols in binary
nm ./FileDuper | grep -i curl

# Expected output should include:
# - CurlMultiManager::*
# - curl_multi_*
# - curl_easy_*
```

---

## Dependencies Check

Ensure all dependencies are installed:

```bash
# Ubuntu/Debian
sudo apt-get install -y \
    libcurl4-openssl-dev \
    qt6-base-dev \
    qt6-networking-dev \
    libssl-dev

# Fedora/RHEL
sudo dnf install -y \
    libcurl-devel \
    qt6-qtbase-devel \
    openssl-devel
```

---

## Troubleshooting Build Issues

### "libcurl.so not found"

```bash
# Find where curl is installed
pkg-config --cflags --libs libcurl

# Add to CMakeLists.txt if needed:
# list(APPEND CMAKE_PREFIX_PATH /usr/lib/x86_64-linux-gnu)
```

### "CURL/curl.h not found"

```bash
# Check if curl headers are installed
find /usr -name "curl.h" 2>/dev/null

# Install if missing:
sudo apt-get install libcurl4-openssl-dev
```

### "Qt6 not found"

```bash
# Set Qt6_DIR manually
cmake .. -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6
```

---

## Runtime Verification

After building, test the curl_multi functionality:

```bash
# Test parallel FTP transfers
./FileDuper --test-curl-multi

# Expected output:
# [CurlMultiManager] ✅ curl_multi initialized with max 4 parallel connections
# [FtpClient] ✅ curl_multi backend initialized
```

---

## Performance Notes

Building with curl_multi support:
- **Compile time**: +5-10 seconds (for curlmultimanager.cpp)
- **Binary size**: +50-100 KB (libcurl linked)
- **Runtime overhead**: Minimal (Qt signal overhead ~1-2ms per callback)

---

## Optimization Flags

For production builds, use:

```cmake
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
```

This enables:
- `-O3`: Aggressive optimization
- `-march=native`: CPU-specific optimizations
- `-DNDEBUG`: Disable assertions (faster)

Result: ~10-15% performance improvement for parallel transfers.
