#ifndef __MHD_FRAM_REQUEST__
#define __MHD_FRAM_REQUEST__
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "../file/file.hpp"
#include "../../Models/user.hpp"
#include "../../Models/session.hpp"

struct FormField {
    std::string value;
    std::string filename;  // For file uploads
    std::string contentType;
};

/**
 * HTTP request class
 */
class Request{
    private:
        /**
         * Char[] buffer containing informations about the request
         */
        std::string buffer;
        /**
         * URL on run time
         */
        std::string url;
        /**
         * Method
         */
        std::string method;
        /**
         * User Agent
         */
        std::string userAgent;
        /**
         * Hostname
         */
        std::string hostname;
        /**
         * Format of the url
         */
        std::string urlFormat;
        /**
         * Socket
         */
        int socket;

        /**
         * Cookies ****
         */

        // session_id to save
        std::string session_id = "";


        /**********  Computations */

        /**
         * Set the url from the buffer
         */
        void setUrlFromBuffer();
        /**
         * Set the method from the buffer
         */
        void setMethodFromBuffer();
        /**
         * Set Address from the buffer
         */
        void setUserAgentFromBuffer();
        /**
         * Set the Hostname from the buffer
         */
        void setHostanameFromBuffer();

        // static std::unordered_map<std::string, std::string> parseUrlEncodedBody(const std::string& body);

        // static std::string urlDecode(const std::string& str);

        std::unordered_map<std::string, FormField> multipartFields;
        bool multipartParsed = false;
        
        void parseMultipartData();
        void parseMultipartPart(const std::string& part);
        void cleanMultipartPart(std::string& part);
        void trim(std::string& str);
        

    public:
        Request() = default;
        /**
         * Pass the buffer to the class
         */
        Request(const std::string buffer);
        /**
         * Get the url in runtime, in string format ex : /mypath/1
         */
        std::string getUrl() const;
        /**
         * Get the Method as string exemple : GET / POST / DELETE / PUT
         */
        std::string getMethod() const;
        /**
         * Get UserAgent
         */
        std::string getUserAgent() const;
        /**
         * Get Hostname
         */
        std::string getHostName() const;
        /**
         * Get Socket
         */
        int getSocket() const;
        /**
         * Get Header value
         */
        std::string getHeaderValue(const std::string& headerName);
        /**
         * Set the url pattern
         */
        void setUrlFormat(std::string urlFormat);
        /**
         * Set Socket
         */
        void setSocket(int socket);
        /**
         * Client wants HTML
         */
        bool clientWantsHtml() const;
        /**
         * Client wants Anything
         */
        bool clientWantAnything() const;

        /*---------------- GET METHOD*/

        /**
         * Get parameter by parameter name as string
         */
        std::string getGETParameterByParameterName(const std::string parameterName);

        /**
         * Get Query parameter by name (?var=val), returns "" if not found
         */
        std::string getQueryParameterByParameterName(const std::string parameterName);

        /*---------------- POST METHOD*/
        /**
         * Get Form data parameter by name 
         */
        std::string getFormDataParameterByParameterName(const std::string parameterName);
        /**
         * Check if this parameter name has an uploaded file
         */
        bool isFileUpload(const std::string& parameterName);
        std::string getFileName(const std::string& parameterName);
        std::string getFileContentType(const std::string& parameterName);
        /**
         * Get file from post, returns the pointer to the file if the file is uploaded, otherwise returns NULL
         */
        MhdFile* getFileFromPostByName(const std::string parameterName);

        /**
         * Cookies
         */

        // get cookie by name
        std::string get_cookie(const std::string &name) const;

        // set session_id
        void set_session_id(const std::string &session_id){this->session_id = session_id;}
        // get session_id
        std::string get_session_id() const{return session_id;}

        /**
         * Get user pointer if authenticated, returns null if not
         */
        User* getUser();

};

#endif // __MHD_FRAM_REQUEST__