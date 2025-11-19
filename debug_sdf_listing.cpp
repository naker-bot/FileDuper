#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>

// Simple callback to capture FTP listing output
size_t writeToString(void* ptr, size_t size, size_t nmemb, void* stream) {
    std::string* str = static_cast<std::string*>(stream);
    str->append(static_cast<const char*>(ptr), size * nmemb);
    return size * nmemb;
}

// Test FTP listing for sdf and sdg directories
void testFtpListing(const std::string& directory) {
// Test FTP listing for sdf and sdg directories
void testFtpListing(const std::string& directory) {
    std::cout << "\n🔍 Testing FTP listing for: " << directory << std::endl;
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cout << "❌ curl_easy_init failed!" << std::endl;
        return;
    }
    
    std::string url = "ftp://192.168.1.224:21" + directory;
    std::cout << "📡 URL: " << url << std::endl;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, "jan");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "");  // Will prompt for password
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    
    std::string listing;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &listing);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        std::cout << "❌ curl_easy_perform failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "✅ FTP LIST successful!" << std::endl;
        std::cout << "📋 Listing content:" << std::endl;
        std::cout << listing << std::endl;
        
        // Count directories and files
        std::istringstream iss(listing);
        std::string line;
        int dirCount = 0, fileCount = 0;
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                if (line[0] == 'd') dirCount++;
                else if (line[0] == '-' || line[0] == 'l') fileCount++;
            }
        }
        std::cout << "📊 Found: " << dirCount << " directories, " << fileCount << " files" << std::endl;
    }
    
    curl_easy_cleanup(curl);
}

int main(int argc, char *argv[]) {
    std::cout << "🚀 Debug FTP listing for sdf and sdg directories" << std::endl;
    
    // Test each directory individually
    testFtpListing("/sdf/");
    testFtpListing("/sdg/");
    
    // Also test root to compare
    testFtpListing("/");
    
    return 0;
}