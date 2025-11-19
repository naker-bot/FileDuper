/*
 * Unified Directory Browser UI Renderer
 * ImGui-based interface for browsing and comparing directories
 */

#pragma once

#include "unified_directory_browser.h"
#include <imgui.h>

class UnifiedDirectoryBrowserUI {
public:
    // Render the browser window
    void render(UnifiedDirectoryBrowser& browser, bool& isOpen);
    
    // Render directory tree with selection
    void renderDirectoryTree(UnifiedDirectoryBrowser& browser);
    
    // Render search bar and filters
    void renderSearchBar(UnifiedDirectoryBrowser& browser);
    
    // Render file list
    void renderFileList(UnifiedDirectoryBrowser& browser);
    
    // Render comparison view
    void renderComparisonView(UnifiedDirectoryBrowser& browser);
    
    // Render sync dialog
    void renderSyncDialog(UnifiedDirectoryBrowser& browser);
    
private:
    // UI state
    int selectedIndex = -1;
    ImGuiTextFilter filter;
    
    // Helpers
    const char* getSourceIcon(const std::string& source);
    ImVec4 getSourceColor(const std::string& source);
};
