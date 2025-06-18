#ifndef __MHD_Functions_
#define __MHD_Functions_

#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <random>


std::string generateUniqueFilename(const std::string& originalFilename) {
    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    // Generate random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    int randomNum = dis(gen);
    
    // Extract file extension
    std::string extension;
    size_t dotPos = originalFilename.find_last_of('.');
    if (dotPos != std::string::npos) {
        extension = originalFilename.substr(dotPos);
    }
    
    // Create unique filename: timestamp_random_originalname
    std::string baseName = originalFilename.substr(0, dotPos);
    return std::to_string(timestamp) + "_" + std::to_string(randomNum) + "_" + baseName + extension;
}

bool saveUploadedFile(const std::string& content, const std::string& filepath) {
    try {
        // Create directory if it doesn't exist
        std::filesystem::path filePath(filepath);
        std::filesystem::create_directories(filePath.parent_path());
        
        // Write binary file
        std::ofstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filepath << std::endl;
            return false;
        }
        
        file.write(content.data(), content.size());
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
        return false;
    }
}

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string getMimeType(const std::string& path) {
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
    if (endsWith(path, ".png")) return "image/png";
    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
    if (endsWith(path, ".gif")) return "image/gif";
    if (endsWith(path, ".svg")) return "image/svg+xml";
    if (endsWith(path, ".ico")) return "image/x-icon";
    return "application/octet-stream"; // fallback
}

#endif