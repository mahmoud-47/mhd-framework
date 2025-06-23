#ifndef __MHD_EXCEPTION_
#define __MHD_EXCEPTION_

    #include <string>

    // Exception in General
    class Exception : public std::exception{
        protected:
            std::string message;
        public:
            Exception(const std::string& message_) : message(message_){}

            virtual const char* what() const noexcept override {
                return message.c_str();
            }

            std::string getMessage() const {return message;}
    };

    // SQL exception
    class SQLException : public Exception{
        public:
            SQLException(const std::string& message_) : Exception(message_){}
    };

    // Custom exception class for datetime-related errors
    class DateTimeException : public Exception{
        public:
            DateTimeException(const std::string& msg) : Exception(msg) {}
            
    };

    // Custom exception class for datetime-related errors
    class EmailException : public Exception{
        public:
            EmailException(const std::string& msg) : Exception(msg) {}
            
    };

#endif