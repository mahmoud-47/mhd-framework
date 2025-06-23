#ifndef __MHD_SEND_MAIL__
#define __MHD_SEND_MAIL__

    #include <string>
    #include <curl/curl.h>
    #include <string>
    #include "../../settings.hpp"
    #include "../render/HttpRender.hpp"

    class MhdSendMail{
        private:
            struct EmailData {
                const char* data;
                size_t size;
                size_t pos;
            };

            static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata);

        public:
            MhdSendMail(const std::string& to_email, const std::string& subject);
            void sendTextMessage(const std::string& message); // raise error 
            void sendHtmlMessage(const std::string& template_path, Context& context); // raise error
    };

#endif