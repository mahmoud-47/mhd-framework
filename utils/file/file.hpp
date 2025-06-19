#ifndef __MHD_FILE__
#define __MHD_FILE__
    #include <fstream>
    #include <filesystem>
    #include <random>
    #include <chrono>
    #include <iostream>
    #include<string>

    #include "../../settings.hpp"

    // File class
    class MhdFile{
        private:
            std::string filename;
            std::string filecontent;
            std::string filecontenttype;
        public:
            MhdFile(const std::string& filename, const std::string& filecontent, const std::string& filecontenttype);
            /**
             * save file to a path, generates a unique filename if with_original_name is false
             * returns the full path or "" if the upload did not succeed
             * the path is a relative one, it is a subfoler of UPLOADS_DIR 
             * the path has to start with / exemple "/clients"
             */
            std::string saveTo(const std::string path, bool with_original_name = false);
            // get the extension of the file
            std::string get_extension() const;
            // get the name of the file without the extension
            std::string get_name_without_extension() const;
            /**
             * remove uploaded file, this is going to be a subdirectory of UPLOADS_DIR
             * The default value of is_absolute_path is false
             * if you want to give a absolute path, set is_absolute_path to false 
             */
            static bool remove_uploaded_file(const std::string path, bool is_absolute_path = false);

            // getters
            std::string get_filename() const{return filename;}
            std::string get_filecontent() const{return filecontent;}
            std::string get_filecontenttype() const{return filecontenttype;}

            // method is_img, is_... need to be implemented here
    };

#endif