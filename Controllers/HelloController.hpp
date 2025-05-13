#ifndef __Hello_Controller__
#define __Hello_Controller__

    
    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"

    class HelloController{
        public:
            // Hello World controller
            static void Hello(Request request){
                std::string templatename = "hello.html";
                renderHtml(request, templatename);
            }

            // Documentation Controller
            static void Documentation(Request request){
                std::string templatename = "documentation.html";
                renderHtml(request, templatename);
            }

            // Controller that returns a string
            static void HelloString(Request request){
                std::string id = request.getGETParameterByParameterName("id");
                std::string message = "Hello herer is a string message with some <h1>HTML</h1>";
                message += "<br> Id = " + id;
                message += "<br>Query parameter key1 = \"" + request.getQueryParameterByParameterName("key1") + "\"";
                message += "<br>Query parameter key2 = \"" + request.getQueryParameterByParameterName("key2") + "\"";
                message += "<br>Query parameter key3 = \"" + request.getQueryParameterByParameterName("key3") + "\"";
                message += "<br>Query parameter key4 = \"" + request.getQueryParameterByParameterName("key4") + "\"";
                
                if(request.getMethod() == "POST"){
                    message += "<br> This is a POST request";
                    message += "<br>POST parameter fkey1 = \"" + request.getFormDataParameterByParameterName("fkey1") + "\"";
                    message += "<br>POST parameter fkey2 = \"" + request.getFormDataParameterByParameterName("fkey2") + "\"";
                    message += "<br>POST parameter fkey3 = \"" + request.getFormDataParameterByParameterName("fkey3") + "\"";
                    message += "<br>POST parameter fkey4 = \"" + request.getFormDataParameterByParameterName("fkey4") + "\"";
                }
                renderText(request, message);
            }

    };

#endif