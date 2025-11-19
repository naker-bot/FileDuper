/*
 * Unified Directory Browser UI Implementation
 */

#include "unified_directory_browser_ui.h"
#include "export_discovery.h"
#include <imgui.h>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdio>

const char* UnifiedDirectoryBrowserUI::getSourceIcon(const std::string& source) {
    if (source == "Local") return "📁";
    if (source == "FTP") return "📡";
    if (source == "NFS") return "🌐";
    if (source == "NFS Server") return "🖥️";
    if (source == "SMB") return "🗂️";
    if (source == "WebDAV") return "☁️";
    return "❓";
}

ImVec4 UnifiedDirectoryBrowserUI::getSourceColor(const std::string& source) {
    if (source == "Local") return ImVec4(0.2f, 0.8f, 0.2f, 1.0f);  // Green
    if (source == "FTP") return ImVec4(0.2f, 0.6f, 1.0f, 1.0f);     // Blue
    if (source == "NFS") return ImVec4(1.0f, 0.7f, 0.2f, 1.0f);     // Orange
    if (source == "NFS Server") return ImVec4(1.0f, 0.5f, 0.2f, 1.0f); // Dark Orange
    if (source == "SMB") return ImVec4(0.8f, 0.2f, 0.8f, 1.0f);     // Purple
    if (source == "WebDAV") return ImVec4(0.2f, 0.8f, 0.8f, 1.0f);  // Cyan
    return ImVec4(0.7f, 0.7f, 0.7f, 1.0f);                          // Gray
}

void UnifiedDirectoryBrowserUI::render(UnifiedDirectoryBrowser& browser, bool& isOpen) {
    if (!isOpen) return;
    
    auto& state = browser.getState();
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowSize(ImVec2(1200, 700), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("🔍 Unified Directory Browser", &isOpen, flags)) {
        // Top bar with source selector and navigation
        if (ImGui::BeginChild("##TopBar", ImVec2(0, 80), true)) {
            ImGui::Text("📍 Current Location: ");
            ImGui::SameLine();
            ImGui::TextColored(getSourceColor(state.currentSource), "%s %s",
                             getSourceIcon(state.currentSource), state.currentSource.c_str());
            
            if (!state.currentSourceHost.empty()) {
                ImGui::SameLine();
                ImGui::Text("@ %s", state.currentSourceHost.c_str());
            }
            
            ImGui::SameLine();
            ImGui::Text(" → %s", state.currentPath.c_str());
            
            ImGui::Separator();
            
            // Source selector buttons
            ImGui::Text("Quick Access: ");
            
            static const std::vector<std::string> sources = {"Local", "FTP", "NFS", "SMB", "WebDAV"};
            for (const auto& src : sources) {
                if (ImGui::SmallButton((getSourceIcon(src) + std::string(" ") + src).c_str())) {
                    // Would open source selection dialog
                }
                ImGui::SameLine();
            }
            
            ImGui::EndChild();
        }
        
        // Main content area
        if (ImGui::BeginChild("##MainContent", ImVec2(0, 0), true)) {
            ImGui::Columns(3, "##BrowserColumns", true);
            
            // Left: Directory tree
            ImGui::SetColumnWidth(-1, 300);
            renderDirectoryTree(browser);
            
            ImGui::NextColumn();
            
            // Middle: File list
            ImGui::SetColumnWidth(-1, 400);
            renderSearchBar(browser);
            renderFileList(browser);
            
            ImGui::NextColumn();
            
            // Right: Comparison/Details
            if (state.comparisonMode) {
                ImGui::Text("📊 Comparison Results");
                ImGui::Separator();
                renderComparisonView(browser);
            } else if (state.syncMode) {
                ImGui::Text("🔄 Sync Status");
                ImGui::Separator();
                renderSyncDialog(browser);
            } else {
                ImGui::Text("ℹ️ Information");
                ImGui::Separator();
                
                if (selectedIndex >= 0 && selectedIndex < (int)state.filteredEntries.size()) {
                    const auto& entry = state.filteredEntries[selectedIndex];
                    ImGui::Text("Name: %s", entry.name.c_str());
                    ImGui::Text("Size: %llu bytes", entry.size);
                    ImGui::Text("Type: %s", entry.isDirectory ? "Directory" : "File");
                    ImGui::Text("Modified: %s", std::ctime(&entry.modified));
                    
                    if (ImGui::Button("📋 Copy Path")) {
                        ImGui::SetClipboardText(entry.fullPath.c_str());
                    }
                }
            }
            
            ImGui::EndChild();
        }
    }
    ImGui::End();
}

void UnifiedDirectoryBrowserUI::renderDirectoryTree(UnifiedDirectoryBrowser& browser) {
    ImGui::Text("📂 Locations");
    ImGui::Separator();
    
    auto& state = browser.getState();
    
    // Local directories
    static bool expandLocal = true;
    if (ImGui::TreeNodeEx("📁 Local", expandLocal ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
        if (ImGui::Selectable("/home", false)) {
            browser.loadDirectory("Local", "", "/home");
        }
        if (ImGui::Selectable("/mnt", false)) {
            browser.loadDirectory("Local", "", "/mnt");
        }
        if (ImGui::Selectable("/media", false)) {
            browser.loadDirectory("Local", "", "/media");
        }
        ImGui::TreePop();
    }
    
    ImGui::Spacing();
    
    // NFS Mounts (local mounted NFS)
    static bool expandNFSMounts = true;
    if (ImGui::TreeNodeEx("🌐 NFS Mounts", expandNFSMounts ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
        // Auto-discover NFS mounts from /etc/mtab or /proc/mounts
        std::vector<std::string> nfsMounts;
        FILE* f = fopen("/proc/mounts", "r");
        if (f) {
            char line[512];
            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, "nfs") && !strstr(line, "sunrpc")) {
                    char device[256], mountpoint[256], fstype[64];
                    if (sscanf(line, "%s %s %s", device, mountpoint, fstype) == 3) {
                        nfsMounts.push_back(mountpoint);
                    }
                }
            }
            fclose(f);
        }
        
        if (nfsMounts.empty()) {
            ImGui::TextDisabled("No NFS mounts found");
        } else {
            for (const auto& mount : nfsMounts) {
                ImGui::PushID(mount.c_str());
                if (ImGui::Selectable(mount.c_str(), false)) {
                    browser.loadDirectory("NFS", "", mount);
                }
                ImGui::PopID();
            }
        }
        ImGui::TreePop();
    }
    
    ImGui::Spacing();
    
    // NFS Servers (remote servers)
    static bool expandNFSServers = true;
    if (ImGui::TreeNodeEx("🖥️ NFS Servers", expandNFSServers ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
        static char nfsServerInput[256] = "";
        ImGui::InputTextWithHint("##NFSServer", "192.168.1.100", nfsServerInput, sizeof(nfsServerInput));
        ImGui::SameLine();
        
        if (ImGui::Button("➕ Add", ImVec2(60, 0))) {
            if (strlen(nfsServerInput) > 0) {
                std::cout << "🔍 Querying NFS server: " << nfsServerInput << std::endl;
                // Will trigger NFS export discovery
            }
        }
        
        // Display discovered NFS servers (placeholder for dynamic content)
        ImGui::TextDisabled("Enter NFS server IP to discover exports");
        ImGui::TreePop();
    }
    
    ImGui::Spacing();
    
    // SMB Shares
    static bool expandSMB = true;
    if (ImGui::TreeNodeEx("🗂️ SMB Shares", expandSMB ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
        static char smbServerInput[256] = "";
        ImGui::InputTextWithHint("##SMBServer", "192.168.1.50", smbServerInput, sizeof(smbServerInput));
        ImGui::SameLine();
        
        if (ImGui::Button("➕ Add##SMB", ImVec2(60, 0))) {
            if (strlen(smbServerInput) > 0) {
                std::cout << "🔍 Querying SMB server: " << smbServerInput << std::endl;
                // Will trigger SMB share discovery
            }
        }
        
        ImGui::TextDisabled("Enter SMB server IP to discover shares");
        ImGui::TreePop();
    }
}

void UnifiedDirectoryBrowserUI::renderSearchBar(UnifiedDirectoryBrowser& browser) {
    auto& state = browser.getState();
    
    ImGui::Text("🔍 Search");
    ImGui::Separator();
    
    if (ImGui::InputText("##SearchFilter", state.searchFilter, 
                         sizeof(state.searchFilter), 
                         ImGuiInputTextFlags_CallbackAlways)) {
        browser.searchEntries(state.searchFilter);
    }
    
    ImGui::SameLine();
    if (ImGui::SmallButton("Clear")) {
        state.searchFilter[0] = '\0';
        browser.searchEntries("");
    }
    
    ImGui::Checkbox("Case Sensitive", &state.searchCaseSensitive);
    ImGui::Checkbox("Show Hidden Files", &state.showHiddenFiles);
    
    // Sort options
    if (ImGui::RadioButton("Sort by Name", state.sortBy == 0)) {
        browser.getState().sortBy = 0;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Sort by Size", state.sortBy == 1)) {
        browser.getState().sortBy = 1;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Sort by Date", state.sortBy == 2)) {
        browser.getState().sortBy = 2;
    }
}

void UnifiedDirectoryBrowserUI::renderFileList(UnifiedDirectoryBrowser& browser) {
    auto& state = browser.getState();
    
    ImGui::Text("📄 Files (%zu)", state.filteredEntries.size());
    ImGui::Separator();
    
    if (ImGui::BeginTable("##FileList", 4, 
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable)) {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Size");
        ImGui::TableSetupColumn("Modified");
        ImGui::TableHeadersRow();
        
        for (int i = 0; i < (int)state.filteredEntries.size(); i++) {
            const auto& entry = state.filteredEntries[i];
            
            ImGui::TableNextRow();
            
            // Name column
            ImGui::TableSetColumnIndex(0);
            ImGuiSelectableFlags flags = ImGuiSelectableFlags_SpanAllColumns;
            if (ImGui::Selectable((getSourceIcon(entry.source) + std::string(" ") + entry.name).c_str(),
                                 selectedIndex == i, flags)) {
                selectedIndex = i;
            }
            
            // Type column
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", entry.isDirectory ? "Dir" : "File");
            
            // Size column
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%llu B", entry.size);
            
            // Modified column
            ImGui::TableSetColumnIndex(3);
            char timeBuf[32];
            std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d", std::localtime(&entry.modified));
            ImGui::Text("%s", timeBuf);
        }
        
        ImGui::EndTable();
    }
}

void UnifiedDirectoryBrowserUI::renderComparisonView(UnifiedDirectoryBrowser& browser) {
    auto& state = browser.getState();
    
    ImGui::Text("Path 1: %s", state.comparePath1.c_str());
    ImGui::Text("Path 2: %s", state.comparePath2.c_str());
    ImGui::Separator();
    
    ImGui::Text("Differences found: %zu", state.comparisonResult.size());
    
    if (ImGui::BeginTable("##ComparisonResults", 3)) {
        ImGui::TableSetupColumn("Item");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Action");
        ImGui::TableHeadersRow();
        
        for (const auto& item : state.comparisonResult) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", item.name.c_str());
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Different");
            
            ImGui::TableSetColumnIndex(2);
            if (ImGui::SmallButton(("Sync##" + item.name).c_str())) {
                // Trigger sync
            }
        }
        
        ImGui::EndTable();
    }
}

void UnifiedDirectoryBrowserUI::renderSyncDialog(UnifiedDirectoryBrowser& browser) {
    auto& state = browser.getState();
    
    ImGui::Text("Source: %s", state.comparePath1.c_str());
    ImGui::Text("Destination: %s", state.comparePath2.c_str());
    ImGui::Separator();
    
    ImGui::Checkbox("Preserve Directory Structure", &state.preserveStructure);
    ImGui::Checkbox("Overwrite Existing Files", &state.overwriteExisting);
    
    ImGui::SliderInt("Max Concurrent Transfers", &state.maxConcurrentTransfers, 1, 16);
    
    ImGui::Text("Progress: %.1f%%", state.loadingProgress * 100.0f);
    ImGui::ProgressBar(state.loadingProgress, ImVec2(-1, 0), state.loadingStatus.c_str());
    
    if (ImGui::Button("Start Sync", ImVec2(100, 0))) {
        // Start synchronization
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(100, 0))) {
        state.syncMode = false;
    }
}

void UnifiedDirectoryBrowserUI::renderExportDiscovery(UnifiedDirectoryBrowser& browser) {
    auto& state = browser.getState();
    
    ImGui::SetNextWindowSize(ImVec2(900, 600), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("🔌 NFS/SMB Export Discovery", nullptr)) {
        ImGui::Text("🔍 Discover available NFS exports and SMB shares from servers");
        ImGui::Separator();
        
        // Input for server host
        static char serverInput[256] = "";
        ImGui::InputTextWithHint("##ServerInput", "Server IP or hostname (e.g. 192.168.1.100)", 
                                serverInput, sizeof(serverInput));
        ImGui::SameLine();
        
        // Server type selector
        static int serverType = 0;  // 0=NFS, 1=SMB
        ImGui::RadioButton("NFS##type", &serverType, 0);
        ImGui::SameLine();
        ImGui::RadioButton("SMB##type", &serverType, 1);
        
        if (ImGui::Button("🔎 Discover Exports", ImVec2(200, 0))) {
            if (strlen(serverInput) > 0) {
                std::cout << "🔍 Starting discovery for: " << serverInput << std::endl;
                
                if (serverType == 0) {  // NFS
                    std::cout << "📊 Querying NFS server exports..." << std::endl;
                    // This will be called asynchronously in main application
                } else {  // SMB
                    std::cout << "📊 Querying SMB server shares..." << std::endl;
                    // This will be called asynchronously in main application
                }
            }
        }
        
        ImGui::Separator();
        ImGui::Text("📋 Available Exports/Shares:");
        ImGui::Spacing();
        
        // Table header
        if (ImGui::BeginTable("##ExportTable", 5, 
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
            
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 200);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Access", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Server", ImGuiTableColumnFlags_WidthStretch, 150);
            ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed, 100);
            ImGui::TableHeadersRow();
            
            // Example export entries (will be replaced with real data)
            static std::vector<ExportInfo> exports;
            
            for (const auto& export_info : exports) {
                ImGui::TableNextRow();
                
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(getSourceColor(export_info.type), "%s %s",
                                 getSourceIcon(export_info.type), export_info.name.c_str());
                
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", export_info.type.c_str());
                
                ImGui::TableSetColumnIndex(2);
                ImVec4 accessColor = (export_info.accessLevel == "rw") 
                    ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f)  // Green for rw
                    : ImVec4(1.0f, 0.7f, 0.2f, 1.0f); // Orange for ro
                ImGui::TextColored(accessColor, "%s", export_info.accessLevel.c_str());
                
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%s", export_info.server.c_str());
                
                ImGui::TableSetColumnIndex(4);
                if (ImGui::Button(("Mount##" + export_info.name).c_str(), ImVec2(80, 0))) {
                    std::cout << "✅ Mounting: " << export_info.path << std::endl;
                    // Trigger mount action
                }
            }
            
            ImGui::EndTable();
        }
        
        ImGui::Separator();
        if (ImGui::Button("Close", ImVec2(100, 0))) {
            ImGui::End();
            return;
        }
        
        ImGui::End();
    }
}

