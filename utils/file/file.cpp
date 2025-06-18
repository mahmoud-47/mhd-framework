#include "file.hpp"

namespace fs = std::filesystem;

// Helper functions 
static std::string generateUniqueFilename(const std::string& originalFilename) {
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

static bool saveUploadedFile(const std::string& content, const std::string& filepath) {
    try {
        // Create directory if it doesn't exist
        fs::path filePath(filepath);
        fs::create_directories(filePath.parent_path());
        
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

// MhdFile implementation
MhdFile::MhdFile(const std::string& filename, const std::string& filecontent, const std::string& filecontenttype)
    : filename(filename), filecontent(filecontent), filecontenttype(filecontenttype) {}

std::string MhdFile::saveTo(const std::string path, bool with_original_name) {
    std::string final_filename;
    
    if (with_original_name) {
        final_filename = filename;
    } else {
        final_filename = generateUniqueFilename(filename);
    }

    fs::path full_path = fs::path(path) / final_filename;
    
    if(saveUploadedFile(filecontent, full_path.string()))
        return full_path.string();
    return "";
}

std::string MhdFile::get_extension() const {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos && dot_pos + 1 < filename.length()) {
        return filename.substr(dot_pos + 1);
    }
    return "";
}

std::string MhdFile::get_name_without_extension() const {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        return filename.substr(0, dot_pos);
    }
    return filename;
}