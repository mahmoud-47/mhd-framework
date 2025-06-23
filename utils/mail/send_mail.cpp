#include "send_mail.hpp"

size_t MhdSendMail::read_callback(char *buffer, size_t size, size_t nitems, void *userdata){
    EmailData* email_data = (EmailData*)userdata;
    
    if (email_data->pos >= email_data->size) {
        return 0;  // No more data to send
    }
    
    size_t remaining = email_data->size - email_data->pos;
    size_t to_copy = (remaining < size * nitems) ? remaining : size * nitems;
    
    memcpy(buffer, email_data->data + email_data->pos, to_copy);
    email_data->pos += to_copy;
    
    return to_copy;
}

// Add some text to be sent
void MhdSendMail::AttachTextMessage(const std::string& message){
    content = "\r\n" + message + "\r\n";
}

// Add Html content
void MhdSendMail::AttachHtmlMessage(const std::string& template_path, Context& context){
    std::string html = readHtmlFile(template_path);
    std::string rendered = parseHtmlCode(html, context);
    content = "MIME-Version: 1.0\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "\r\n" + rendered + "\r\n";
}

// send the message or raise an MailException error if fails
void MhdSendMail::send(){
    CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_slist *recipients = nullptr;
        
        // SMTP settings
        curl_easy_setopt(curl, CURLOPT_URL, SMTP_SERVER.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, SMTP_SENDER_EMAIL.c_str());
        
        // Recipients
        const char* receiver_email_c_str = receiver_email.c_str();
        recipients = curl_slist_append(recipients, receiver_email_c_str);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        
        // Email content
        std::string email_content_cpp = "To: " + receiver_email + "\r\n"
            "From: " + SMTP_SENDER_EMAIL + "\r\n"
            "Subject: " + subject + "\r\n"
            "\r\n" + content;

        const char* email_content = email_content_cpp.c_str();
        
        
        // Setup email data structure
        EmailData email_data;
        email_data.data = email_content;
        email_data.size = strlen(email_content);
        email_data.pos = 0;
        
        // Set callback and data
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &email_data);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        
        // Authentication
        curl_easy_setopt(curl, CURLOPT_USERNAME, SMTP_SENDER_EMAIL);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, SMTP_SENDER_PASSWORD);
        
        // Enable verbose output for debugging
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        

        if (res != CURLE_OK) {
            char error[1000];
            sprintf(error, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            MailException e(error);
            throw e;
        }  
        
        // Cleanup
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
}