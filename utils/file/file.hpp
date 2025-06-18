#ifndef __MHD_FILE__
#define __MHD_FILE__
    #include <fstream>
    #include <filesystem>
    #include <random>
    #include <chrono>
    #include <iostream>
    #include<string>

    // File class
    class MhdFile{
        private:
            std::string filename;
            std::string filecontent;
            std::string filecontenttype;
        public:
            MhdFile(const std::string& filename, const std::string& filecontent, const std::string& filecontenttype);
            // save file to a path, generates a unique filename if with_original_name is false, returns the full path or ""
            std::string saveTo(const std::string path, bool with_original_name = false);
            // get the extension of the file
            std::string get_extension() const;
            // get the name of the file without the extension
            std::string get_name_without_extension() const;

            // getters
            std::string get_filename() const{return filename;}
            std::string get_filecontent() const{return filecontent;}
            std::string get_filecontenttype() const{return filecontenttype;}

            // method is_img, is_... need to be implemented here
    };

#endif