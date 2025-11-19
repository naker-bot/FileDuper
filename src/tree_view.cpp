// Modern C++ implementation for directory tree rendering with POSIX APIs
#include "tree_view.h"
#include <imgui.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fnmatch.h>
#include <cstring>
#include <set>
#include <functional>
#include <memory>

// RAII wrapper for DIR* to ensure proper cleanup
class DirectoryHandle {
public:
    explicit DirectoryHandle(const std::string& path) : dir_(opendir(path.c_str())) {}
    ~DirectoryHandle() { if (dir_) closedir(dir_); }
    
    DirectoryHandle(const DirectoryHandle&) = delete;
    DirectoryHandle& operator=(const DirectoryHandle&) = delete;
    
    explicit operator bool() const noexcept { return dir_ != nullptr; }
    DIR* get() const noexcept { return dir_; }
    
private:
    DIR* dir_;
};

static bool matchesPattern(const std::string& name, const std::vector<std::string>& patterns) noexcept {
    if (patterns.empty()) return false;
    return std::any_of(patterns.begin(), patterns.end(), [&name](const auto& pattern) {
        return fnmatch(pattern.c_str(), name.c_str(), 0) == 0;
    });
}

static bool shouldSkip(const std::string& name, const std::vector<std::string>& notLike,
                       const std::vector<std::string>& like) noexcept {
    if (!notLike.empty() && matchesPattern(name, notLike)) return true;
    if (!like.empty() && !matchesPattern(name, like)) return true;
    return false;
}

static std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

static bool pathMatchesSearchTerms(const std::string& path, const std::vector<std::string>& searchTerms) {
    if (searchTerms.empty()) return true;
    const auto lowerPath = toLower(path);
    return std::any_of(searchTerms.begin(), searchTerms.end(), [&lowerPath](const auto& term) {
        return lowerPath.find(term) != std::string::npos;
    });
}

// Recursive directory tree renderer with TRUE LAZY LOADING (on-demand)
// ONLY reads directory when TreeNode is actually opened!
void renderDirectoryTreeRecursive(const std::string& root, int depth, int maxDepth,
                                    const std::vector<std::string>& notLike,
                                    const std::vector<std::string>& like,
                                    std::string& selectedPath,
                                    std::set<std::string>* selectedPaths,
                                    const std::vector<std::string>& searchTerms) {
    if (depth > maxDepth) return;

    // CRITICAL: Only read directory contents for THIS level
    // Do NOT recurse into children unless TreeNode is expanded!
    
    DirectoryHandle dir(root);
    if (!dir) return;

    std::vector<std::pair<std::string, bool>> entries;
    struct dirent* ent;
    while ((ent = readdir(dir.get())) != nullptr) {
        if (ent->d_name[0] == '.') continue; // skip hidden
        std::string name = ent->d_name;
        if (shouldSkip(name, notLike, like)) continue;

        const std::string full = root + "/" + name;
        struct stat st;
        bool isDir = false;
        if (stat(full.c_str(), &st) == 0) {
            isDir = S_ISDIR(st.st_mode);
        }
        entries.emplace_back(std::move(name), isDir);
    }

    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        // Directories first, then lexical order
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    for (const auto& [name, isDir] : entries) {
        const std::string full = root + "/" + name;
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (!isDir) flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        // No search functionality - just show all directories
        // User must navigate manually to avoid NFS performance issues

        // Multi-selection checkbox BEFORE TreeNode (left side, unchecked by default)
        bool selected = false;
        if (selectedPaths) {
            selected = selectedPaths->count(full) > 0;
            
            // Create unique widget ID (replace slashes to avoid confusion)
            std::string checkboxId = "##sel_" + full;
            std::replace(checkboxId.begin(), checkboxId.end(), '/', '#');
            
            if (ImGui::Checkbox(checkboxId.c_str(), &selected)) {
                if (selected) {
                    // Remove redundant child selections when parent is selected
                    const std::string prefix = full + "/";
                    std::vector<std::string> toRemove;
                    for (const auto& path : *selectedPaths) {
                        if (path.find(prefix) == 0) {
                            toRemove.push_back(path);
                        }
                    }
                    for (const auto& path : toRemove) {
                        selectedPaths->erase(path);
                    }
                    selectedPaths->insert(full);
                } else {
                    selectedPaths->erase(full);
                }
            }
            
            // Show tooltip if already covered by parent selection
            if (!selected) {
                for (const auto& parentPath : *selectedPaths) {
                    if (parentPath != full && full.find(parentPath + "/") == 0) {
                        ImGui::SameLine();
                        ImGui::TextDisabled("(Bereits durch Eltern ausgewählt)");
                        break;
                    }
                }
            }
            
            ImGui::SameLine(); // Checkbox and TreeNode on same line
        }

        // TreeNode AFTER checkbox
        const bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

        // Double-click disabled to prevent UI freeze on large NFS shares
        // TODO: Implement async directory selection instead
        #if 0
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            selectedPath = full;
        }
        #endif

        if (opened && isDir) {
            renderDirectoryTreeRecursive(full, depth + 1, maxDepth, notLike, like, 
                                       selectedPath, selectedPaths, searchTerms);
            ImGui::TreePop();
        }
    }
}

bool renderDirectoryTreeUI(const std::string& root,
                           int maxDepth,
                           bool showRootName,
                           const std::vector<std::string>& notLike,
                           const std::vector<std::string>& like,
                           std::string& selectedPath,
                           std::set<std::string>* selectedPaths,
                           const std::vector<std::string>& searchTerms)
{
    if (showRootName) {
        ImGui::Text("%s", root.c_str());
    }
    ImGui::BeginChild((std::string("Tree-") + root).c_str(), ImVec2(0, -20), true);
    renderDirectoryTreeRecursive(root, 0, maxDepth, notLike, like, selectedPath, selectedPaths, searchTerms);
    ImGui::EndChild();
    return !selectedPath.empty();
}
