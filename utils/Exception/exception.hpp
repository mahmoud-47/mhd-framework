#ifndef __MHD_EXCEPTION_
#define __MHD_EXCEPTION_

    #include <string>

    // Exception in General
    class Exception{
        protected:
            std::string message;
        public:
            Exception(const std::string& message_) : message(message_){}
            std::string getMessage() const {return message;}
    };

    // SQL exception
    class SQLException : public Exception{
        public:
            SQLException(const std::string& message_) : Exception(message_){}
    };

#endif