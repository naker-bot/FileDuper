#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/**
 * 🧠 STANDALONE INTELLIGENT PROCESSING DEMONSTRATION
 * 
 * Zeigt das automatische Umschalten zwischen NPU (Bilder/Videos) 
 * und GPU-Hash (Audio/Dokumente/Archive/Daten) basierend auf Dateikategorien
 * 
 * User Request: "wenn du andere dateitypen von eine anderen kattergorie siehst 
 * switche automatisch zu hash gpu dann wieder zu npu u wenn der Dateityp stimmt 
 * und vergleich später dann bilder mit bilder Musik mit Musik Video mit Video usw"
 */

enum ProcessingCategory {
    IMAGES = 0,      // .jpg, .png, .bmp, .tiff → NPU processing
    VIDEOS = 1,      // .mp4, .avi, .mkv, .mov → NPU processing  
    AUDIO = 2,       // .mp3, .wav, .flac, .ogg → GPU-Hash processing
    DOCUMENTS = 3,   // .pdf, .doc, .txt, .rtf → GPU-Hash processing
    ARCHIVES = 4,    // .zip, .rar, .7z, .tar → GPU-Hash processing
    DATA = 5,        // .dat, .db, .json, .xml → GPU-Hash processing
    UNKNOWN = 6      // Other file types → Auto-select processing
};

enum ProcessingUnit {
    AUTO_SELECT = 0,      // Automatic best available
    CPU_ALL_CORES = 1,    // Always available fallback
    GPU_OPENCL = 2,       // Generic OpenCL acceleration
    INTEL_GPU_OPENCL = 3, // Intel GPU optimized (Arc, Xe, UHD)
    NPU_LEVEL_ZERO = 4    // Intel NPU via Level Zero API
};

ProcessingCategory categorizeFile(const std::string &fileName) {
    std::string extension;
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != std::string::npos) {
        extension = fileName.substr(dotPos + 1);
        // Convert to lowercase
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    }
    
    // Images → NPU processing
    if (extension == "jpg" || extension == "jpeg" || extension == "png" || 
        extension == "bmp" || extension == "tiff" || extension == "tif" ||
        extension == "gif" || extension == "webp" || extension == "svg") {
        return IMAGES;
    }
    
    // Videos → NPU processing
    if (extension == "mp4" || extension == "avi" || extension == "mkv" || 
        extension == "mov" || extension == "wmv" || extension == "flv" ||
        extension == "webm" || extension == "m4v" || extension == "3gp") {
        return VIDEOS;
    }
    
    // Audio → GPU-Hash processing
    if (extension == "mp3" || extension == "wav" || extension == "flac" || 
        extension == "ogg" || extension == "aac" || extension == "wma" ||
        extension == "m4a" || extension == "opus") {
        return AUDIO;
    }
    
    // Documents → GPU-Hash processing
    if (extension == "pdf" || extension == "doc" || extension == "docx" || 
        extension == "txt" || extension == "rtf" || extension == "odt" ||
        extension == "xls" || extension == "xlsx" || extension == "ppt" || extension == "pptx") {
        return DOCUMENTS;
    }
    
    // Archives → GPU-Hash processing
    if (extension == "zip" || extension == "rar" || extension == "7z" || 
        extension == "tar" || extension == "gz" || extension == "bz2" ||
        extension == "xz" || extension == "iso" || extension == "dmg") {
        return ARCHIVES;
    }
    
    // Data files → GPU-Hash processing
    if (extension == "dat" || extension == "db" || extension == "sqlite" || 
        extension == "json" || extension == "xml" || extension == "csv" ||
        extension == "log" || extension == "config" || extension == "ini") {
        return DATA;
    }
    
    return UNKNOWN; // Unknown file types
}

ProcessingUnit getOptimalUnitForCategory(ProcessingCategory category) {
    switch (category) {
        case IMAGES:
        case VIDEOS:
            // Images and Videos → NPU if available, Intel GPU as fallback
            return NPU_LEVEL_ZERO;  // Assume NPU available for demo
            
        case AUDIO:
        case DOCUMENTS:
        case ARCHIVES:
        case DATA:
            // All other categories → GPU-Hash for fast content hashing
            return INTEL_GPU_OPENCL;  // Intel GPU optimized
            
        case UNKNOWN:
        default:
            // Unknown types → Auto-select best available
            return AUTO_SELECT;
    }
}

void simulateIntelligentProcessing() {
    std::cout << "🧠 INTELLIGENT PROCESSING UNIT SWITCHING DEMONSTRATION\n";
    std::cout << "=======================================================\n\n";
    
    const char* categoryNames[] = {
        "IMAGES", "VIDEOS", "AUDIO", "DOCUMENTS", 
        "ARCHIVES", "DATA", "UNKNOWN"
    };
    
    const char* unitNames[] = {
        "AUTO_SELECT", "CPU_ALL_CORES", "GPU_OPENCL", 
        "INTEL_GPU_OPENCL", "NPU_LEVEL_ZERO"
    };
    
    // Test file categorization
    std::cout << "📂 File Categorization Test:\n";
    std::cout << "----------------------------\n";
    
    std::vector<std::string> testFiles = {
        "vacation_photo.jpg", "family_video.mp4", "favorite_song.mp3", 
        "business_report.pdf", "backup_files.zip", "settings.json", "unknown_file.xyz"
    };
    
    for (const auto &fileName : testFiles) {
        ProcessingCategory category = categorizeFile(fileName);
        ProcessingUnit optimalUnit = getOptimalUnitForCategory(category);
        
        std::cout << "📁 " << fileName 
                  << " → " << categoryNames[category] 
                  << " → " << unitNames[optimalUnit] << "\n";
    }
    
    // Test intelligent switching
    std::cout << "\n🔄 Intelligent Unit Switching Simulation:\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<std::string> processingSequence = {
        "image1.jpg",       // IMAGES → NPU
        "video1.mp4",       // VIDEOS → NPU (stay)
        "audio1.mp3",       // AUDIO → GPU-Hash (switch)
        "document1.pdf",    // DOCUMENTS → GPU-Hash (stay)
        "archive1.zip",     // ARCHIVES → GPU-Hash (stay)
        "data1.json",       // DATA → GPU-Hash (stay)
        "image2.png",       // IMAGES → NPU (switch back)
        "video2.avi"        // VIDEOS → NPU (stay)
    };
    
    ProcessingUnit currentUnit = CPU_ALL_CORES;
    
    for (const auto &fileName : processingSequence) {
        ProcessingCategory category = categorizeFile(fileName);
        ProcessingUnit optimalUnit = getOptimalUnitForCategory(category);
        
        if (optimalUnit != currentUnit) {
            std::cout << "🔄 SWITCH: " << unitNames[currentUnit] 
                      << " → " << unitNames[optimalUnit] 
                      << " (for " << categoryNames[category] << " processing)\n";
            currentUnit = optimalUnit;
        } else {
            std::cout << "✅ STAY:   " << unitNames[currentUnit] 
                      << " (for " << categoryNames[category] << " processing)\n";
        }
        
        std::cout << "   📁 Processing: " << fileName << " (" << categoryNames[category] << ")\n";
        
        // Simulate processing type
        if (category == IMAGES || category == VIDEOS) {
            std::cout << "   🧠 NPU feature extraction for similarity comparison\n";
        } else {
            std::cout << "   🔢 GPU-Hash calculation for content-based duplicate detection\n";
        }
        std::cout << "\n";
    }
    
    // Summary
    std::cout << "🎯 INTELLIGENT PROCESSING SUMMARY:\n";
    std::cout << "==================================\n";
    std::cout << "✅ Images/Videos → NPU processing for AI-based similarity\n";
    std::cout << "✅ Audio/Documents/Archives/Data → GPU-Hash for fast duplicate detection\n";
    std::cout << "✅ Automatic unit switching based on file category\n";
    std::cout << "✅ Category-based comparison: 'bilder mit bilder, Musik mit Musik, Video mit Video'\n";
    std::cout << "✅ Optimal performance through intelligent hardware utilization\n";
}

int main() {
    simulateIntelligentProcessing();
    return 0;
}
