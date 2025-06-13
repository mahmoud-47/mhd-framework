#ifndef __MHD_FRAM_REQUEST_
#define __MHD_FRAM_REQUEST_
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>


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
         * Get Query parameter by name (?var=val)
         */
        std::string getQueryParameterByParameterName(const std::string parameterName);

        /*---------------- POST METHOD*/
        /**
         * Get Form data parameter by name 
         */
        std::string getFormDataParameterByParameterName(const std::string parameterName);
};

#endif