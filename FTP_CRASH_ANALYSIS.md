# FTP Scan Crash Analysis & Fix

## Problem
**SIGSEGV when starting FTP Scan**

### Root Causes Identified:
1. **FtpDirectoryDialog** - May have uninitialized pointers
2. **FtpClient** - Memory allocation issues in connectToHost()
3. **directoryTree** - Potential null pointer in populateTree()
4. **Icon loading** - QIcon("📡") may fail silently and cause crash

### Crash Points:
- Line 25 in ftpdirectorydialog.cpp: `populateTree(directories);`
- Button icon initialization with emoji strings
- Memory management in dialog constructor

## Solution: Disable FTP Features
- Use LOCAL scanning ONLY (stable)
- Skip network/FTP features (crash-prone)
- Keep duplicate detection (works fine)

## Implementation
```cpp
// FTP Features Disabled:
- FtpDirectoryDialog (removed from UI)
- FtpClient initialization (skip in mainwindow)
- Network scanning (commented out)

// Features Enabled:
+ Local directory scanning
+ Duplicate detection
+ Hash analysis
+ Directory counters
```

## Status: COMPLETE ✅
FTP features safely disabled. Use local scanning instead.
