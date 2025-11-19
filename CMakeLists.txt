cmake_minimum_required(VERSION 3.16)
project(FileDuper)

# Deaktiviere Warnungen-als-Fehler für deprecated OpenSSL 3.0 MD5 Funktionen
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations")
string(REGEX REPLACE " -Werror" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Performance optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# Link-Time Optimization (LTO) for Release builds
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "✅ Link-Time Optimization (LTO) ENABLED")
    else()
        message(STATUS "⚠️ LTO not supported: ${ipo_error}")
    endif()
endif()

# Compatibility flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")

# AVX2 Optimization for modern CPUs (Intel Arrow Lake, AMD Zen, etc.)
# Detect and enable AVX2 for significant hash performance boost
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
    message(STATUS "✅ AVX2 optimization ENABLED - Hash speed boost activated!")
else()
    message(STATUS "⚠️ AVX2 not supported by compiler")
endif()

# NO QT - using ImGui instead
message(STATUS "✅ ImGui-based build - NO Qt dependencies")

# OpenSSL (required for hashing)
find_package(OpenSSL REQUIRED)
message(STATUS "✅ OpenSSL gefunden: ${OPENSSL_VERSION}")

# Optional OpenCL (disable if not available)
find_package(OpenCL QUIET)
if(OpenCL_FOUND)
    message(STATUS "✅ OpenCL gefunden - GPU acceleration ENABLED")
    add_definitions(-DWITH_OPENCL)
    set(OPENCL_LIBS OpenCL::OpenCL)
else()
    message(STATUS "⚠️ OpenCL nicht gefunden - GPU acceleration DISABLED")
    add_definitions(-UWITH_OPENCL)
    set(OPENCL_LIBS "")
endif()

# libcurl (required for FTP)
find_package(CURL REQUIRED)
if(CURL_FOUND)
    message(STATUS "✅ libcurl gefunden: ${CURL_VERSION_STRING}")
    include_directories(${CURL_INCLUDE_DIRS})
endif()

# Optional curl via pkg-config als Fallback
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CURL_PC QUIET libcurl)
    # Optional libnfs
    pkg_check_modules(LIBNFS QUIET libnfs)
    if(LIBNFS_FOUND)
        message(STATUS "✅ libnfs pkg-config found: ${LIBNFS_VERSION}")
        # Check for header file with common paths
        include(CheckIncludeFile)
        check_include_file("libnfs.h" HAVE_LIBNFS_HEADER1)
        check_include_file("nfsc/libnfs.h" HAVE_LIBNFS_HEADER2)
        if(HAVE_LIBNFS_HEADER1 OR HAVE_LIBNFS_HEADER2)
            set(HAVE_LIBNFS_HEADER TRUE)
        endif()
        if(HAVE_LIBNFS_HEADER)
            message(STATUS "✅ libnfs header found")
            add_definitions(-DHAVE_LIBNFS)
            # Make include dirs available to helper library
            set(LIBNFS_EXTRA_INCLUDES ${LIBNFS_INCLUDE_DIRS})
        else()
            message(WARNING "libnfs library found but headers not available; libnfs will be disabled.")
            unset(LIBNFS_FOUND)
        endif()
    endif()
    if(LIBNFS_FOUND)
        message(STATUS "✅ libnfs gefunden: ${LIBNFS_VERSION}")
        add_definitions(-DHAVE_LIBNFS)
        include_directories(${LIBNFS_INCLUDE_DIRS})
        link_directories(${LIBNFS_LIBRARY_DIRS})
    else()
        message(STATUS "⚠️ libnfs nicht gefunden - remote NFS listing via libnfs deaktiviert")
    endif()

# ULTRA-SPEED: liburing für async I/O
pkg_check_modules(LIBURING QUIET liburing)
if(LIBURING_FOUND)
    message(STATUS "✅ liburing gefunden - async I/O acceleration ENABLED")
    add_definitions(-DWITH_LIBURING)
    set(LIBURING_LIBS ${LIBURING_LIBRARIES})
    link_directories(${LIBURING_LIBRARY_DIRS})
else()
    message(STATUS "⚠️ liburing nicht gefunden - async I/O disabled")
    set(LIBURING_LIBS "")
endif()
endif()

if(CURL_PC_FOUND)
    message(STATUS "✅ libcurl (pkg-config) gefunden")
    set(CURL_LIBS ${CURL_PC_LIBRARIES})
    link_directories(${CURL_PC_LIBRARY_DIRS})
elseif(CURL_FOUND)
    set(CURL_LIBS ${CURL_LIBRARIES})
endif()

# Source files (ImGui version - no Qt)
# Platform-specific network scanner implementation - pick source before SOURCES definition
if(WIN32)
    set(NETWORKSCANNER_SRC src/networkscanner_windows.cpp)
else()
    set(NETWORKSCANNER_SRC src/networkscanner.cpp)
endif()

set(SOURCES
    src/main.cpp
    src/scanner.cpp
    src/hardwarebenchmark.cpp
    src/hashengine.cpp
    # network scanner implementation is platform specific; we pick a native source below
    ${NETWORKSCANNER_SRC}
    src/networkscanner_adapter.cpp
    src/ftpclient.cpp
    src/ultraspeedengine.cpp
    src/ftplistworker.cpp
    src/ftpdeleteworker.cpp
    src/nfsclient.cpp
    src/nfs_helpers.cpp
    src/tree_view.cpp
    src/sftpclient.cpp
    src/smbclient.cpp
)

# Platform-specific network scanner implementation
if(WIN32)
    set(NETWORKSCANNER_SRC src/networkscanner_windows.cpp)
else()
    set(NETWORKSCANNER_SRC src/networkscanner.cpp)
endif()

# ImGui library
add_subdirectory(lib/imgui)

# Headers
set(HEADERS
    include/scanner.h
    include/hashengine.h
    include/hardwarebenchmark.h
    include/networkscanner.h
    include/presetmanager.h
    include/ftpclient.h
    include/ftplistworker.h
    include/ftpdeleteworker.h
    include/sftpclient.h
    include/smbclient.h
    include/nfsclient.h
    include/tree_view.h
    include/ultraspeedengine.h
)

# Include directories
include_directories(include)
include_directories(lib/imgui)
include_directories(lib/imgui/backends)

# Find GLFW3 for ImGui
find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
    message(STATUS "✅ GLFW3 gefunden")
endif()

# Find OpenGL for ImGui
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message(STATUS "✅ OpenGL gefunden")
endif()

# Find X11 for native window management
find_package(X11 REQUIRED)
if(X11_FOUND)
    message(STATUS "✅ X11 gefunden")
endif()

# Create executable
add_executable(FileDuper ${SOURCES} ${HEADERS})

# Link libraries (NO Qt!)
target_link_libraries(FileDuper 
    imgui
    glfw
    OpenGL::GL
    ${X11_LIBRARIES}
    ${OPENCL_LIBS}
    ${CURL_LIBS}
    ${LIBURING_LIBS}
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
    ${LIBNFS_LIBRARIES}
    net_utils
)

if(WIN32)
    # Link Winsock library for Windows network APIs
    target_link_libraries(FileDuper PRIVATE ws2_32)
endif()

# Optional libssh support for remote commands
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(LIBSSH QUIET libssh)
endif()
if(LIBSSH_FOUND)
    message(STATUS "✅ libssh found - enabling libssh remote start")
    add_definitions(-DWITH_LIBSSH)
    set(LIBSSH_LIBS ${LIBSSH_LIBRARIES})
else()
    message(STATUS "⚠️ libssh not found - fallback to system ssh")
    set(LIBSSH_LIBS "")
endif()

# Small helper library for tools and tests
add_library(fileduper_helpers STATIC src/nfs_helpers.cpp)
if(LIBNFS_FOUND)
    target_include_directories(fileduper_helpers PRIVATE ${LIBNFS_EXTRA_INCLUDES})
    target_link_libraries(fileduper_helpers PRIVATE ${LIBNFS_LIBRARIES})
endif()
target_include_directories(fileduper_helpers PRIVATE include)
target_link_libraries(fileduper_helpers PRIVATE ${CURL_LIBS} OpenSSL::SSL OpenSSL::Crypto pthread)

# Small net utils for CIDR & subnet normalization
add_library(net_utils STATIC src/net_utils.cpp)
target_include_directories(net_utils PRIVATE include)
target_link_libraries(net_utils PRIVATE pthread)

# Install target
install(TARGETS FileDuper DESTINATION bin)

# Small test to verify remote NFS exports listing
add_executable(test_nfs_listexports tools/test_nfs_listexports.cpp)
target_link_libraries(test_nfs_listexports PRIVATE fileduper_helpers)
install(TARGETS test_nfs_listexports RUNTIME DESTINATION bin)

# Small test to verify local /etc/exports parsing
add_executable(test_parse_local_exports tools/test_parse_local_exports.cpp)
target_link_libraries(test_parse_local_exports PRIVATE fileduper_helpers)
install(TARGETS test_parse_local_exports RUNTIME DESTINATION bin)

# Basic test for the non-Qt NetworkScannerAdapter
add_executable(test_networkscanner_adapter tools/test_networkscanner_adapter.cpp src/networkscanner_adapter.cpp ${NETWORKSCANNER_SRC})
install(TARGETS test_networkscanner_adapter RUNTIME DESTINATION bin)

    # Basic test for the non-Qt NetworkScannerAdapter
    add_executable(test_networkscanner_cancel tools/test_networkscanner_cancel.cpp src/networkscanner_adapter.cpp ${NETWORKSCANNER_SRC})
    add_executable(test_net_utils tools/test_net_utils.cpp)
    target_include_directories(test_net_utils PRIVATE include)
    target_link_libraries(test_net_utils PRIVATE pthread net_utils)
    install(TARGETS test_net_utils RUNTIME DESTINATION bin)
    install(TARGETS test_networkscanner_cancel RUNTIME DESTINATION bin)

    if(WIN32)
        target_link_libraries(test_networkscanner_adapter PRIVATE ws2_32)
        target_link_libraries(test_networkscanner_cancel PRIVATE ws2_32)
    endif()
    

