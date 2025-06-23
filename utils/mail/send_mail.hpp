#ifndef __MHD_SEND_MAIL__
#define __MHD_SEND_MAIL__

    #include <string>
    #include <curl/curl.h>
    #include <string>
    #include "../../settings.hpp"
    #include "../render/HttpRender.hpp"
    #include "../Exception/exception.hpp"

    // Send Email class
    class MhdSendMail{
        private:
            // string content
            std::string content;
            // receiver email
            std::string receiver_email;
            // subject
            std::string subject;

            struct EmailData {
                const char* data;
                size_t size;
                size_t pos;
            };

            static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata);
            
        public:
            // receiver email, subject of the mail
            MhdSendMail(const std::string& receiver_email, const std::string& subject) : 
                receiver_email(receiver_email),
                subject(subject){}
            // Add some text to be sent
            void AttachTextMessage(const std::string& message); 
            // Add Html content
            void AttachHtmlMessage(const std::string& template_path, Context& context); 
            // send the message or raise an MailException error if fails
            void send(); // raise error
    };

#endif