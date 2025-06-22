#ifndef __MHD_SESSION__
#define __MHD_SESSION__
    #include <string>
    #include "../request/Request.hpp"

    // Session class 
    class Session{
        private:
            // session id
            std::string session_id;
            // serialized data (key=val;key=val)
            std::string content;
            // Helper function to generate a session_id
            std::string generateSessionID(size_t length = 32);
            // Generate unique 
            std::string generateUniqueSessionID();
        public:
            // Session constructor
            Session(Request& request);
            // Get the value of a key, returns "" if not found
            std::string get_value(const std::string key); // looks into the content to get the value or returns ""
            // Set value (key, value)
            void set_value(const std::string key, const std::string value); // updates the database 
    };

#endif