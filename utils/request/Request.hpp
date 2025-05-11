#ifndef __MHD_FRAM_REQUEST_
#define __MHD_FRAM_REQUEST_
#include<string>
#include <sstream>


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
        /**
         * Get parameter by parameter name as string
         */
        std::string getGETParameterByParameterName(const std::string parameterName);

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
         * Set the url pattern
         */
        void setUrlFormat(std::string urlFormat);
};

#endif