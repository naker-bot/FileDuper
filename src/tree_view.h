#pragma once

#include <set>
#include <string>
#include <vector>

// Render a directory tree rooted at `root` with optional filters and multi-select support.
bool renderDirectoryTreeUI(const std::string& root,
                           int maxDepth,
                           bool showRootName,
                           const std::vector<std::string>& notLike,
                           const std::vector<std::string>& like,
                           std::string& selectedPath,
                           std::set<std::string>* selectedPaths = nullptr,
                           const std::vector<std::string>& searchTerms = {});

// Query whether the directory tree for a path is currently being scanned
bool isTreeScanning(const std::string &path);

// Query whether any tree scan is currently active
bool isAnyTreeScanning();

// Ensure that a path is scheduled for background scanning (non-blocking)
void ensureTreeScanned(const std::string &path);

// Return cached children of a directory (just names, not full paths) - empty if not yet scanned
std::vector<std::string> getCachedChildren(const std::string &path);

// Returns true if a background scan for the given path is currently running
bool isTreeScanning(const std::string& root);

// After rendering, query how many matches were rendered (useful for progressive UI)
int getLastSearchMatchesCount();
