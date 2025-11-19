// Simple directory tree rendering helpers for FileDuper
#pragma once

#include <string>
#include <vector>
#include <set>

// Render an interactive directory tree starting at `root`.
// - maxDepth: maximum recursion depth
// - showRootName: when true the root name will be printed above the tree
// - notLike: vector of wildcard patterns to skip
// - like: vector of wildcard patterns to allow only matching names; if empty, all are allowed
// Returns true if node was selected (double-click) and sets selectedPath with full path
// - selectedPaths: optional pointer to a set used for multi-selection (checkboxes). When provided,
//   checkboxes will appear next to each directory and selections inserted/removed from the set.
// - searchTerms: optional list of lowercased substrings; when non-empty the tree will be filtered
//   to only show nodes that match at least one term (or whose descendant matches), and matching nodes
//   will be highlighted. Multiple terms are ORed together.
bool renderDirectoryTreeUI(const std::string& root, int maxDepth, bool showRootName,
                             const std::vector<std::string>& notLike,
                             const std::vector<std::string>& like,
                             std::string& selectedPath,
                             std::set<std::string>* selectedPaths = nullptr,
                             const std::vector<std::string>& searchTerms = std::vector<std::string>());

// Helper: non-UI recursive rendering (for other uses)
void renderDirectoryTreeRecursive(const std::string& root, int depth, int maxDepth,
                                  const std::vector<std::string>& notLike,
                                  const std::vector<std::string>& like,
                                  std::string& selectedPath);
