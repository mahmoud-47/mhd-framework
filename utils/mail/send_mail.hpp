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
            // sender name
            std::string sender_name;
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
            /**
             * @brief Constructs an MhdSendMail object for sending an email.
             *
             * This constructor initializes the email sending context with the receiver's name,
             * email address, and the subject of the message.
             *
             * @param sender_name  The name you want the user to see in their email as sender.
             * @param receiver_email The email address of the recipient (e.g., "recipient@gmail.com").
             * @param subject        The subject line of the email.
             */
            MhdSendMail(const std::string& sender_name, const std::string& receiver_email, const std::string& subject) : 
                receiver_email(receiver_email),
                sender_name(sender_name),
                subject(subject){}

            /**
             * @brief Attaches a plain text message to the email body.
             *
             * This method is used to include a plain text version of the email content.
             * It can be used on its own or in combination with HTML content.
             *
             * @param message The plain text message to include in the email.
             */
            void AttachTextMessage(const std::string& message);

            /**
             * @brief Attaches HTML content to the email body using a template.
             *
             * Loads an HTML template from the given file path, renders it using the provided context,
             * and attaches the result to the email as HTML content.
             *
             * @param template_path The file path to the HTML template (e.g., "templates/welcome.html").
             * @param context       The context used to populate dynamic values in the template.
             */
            void AttachHtmlMessage(const std::string& template_path, Context& context);

            /**
             * @brief Sends the constructed email.
             *
             * This method attempts to send the email using the configured SMTP settings.
             * It must be called after setting the recipient, subject, and message body.
             *
             * @throws MailException if the sending process fails.
             */
            void send();
    };

#endif