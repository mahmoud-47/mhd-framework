#ifndef __Hello_Controller__
#define __Hello_Controller__

    
    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"

    class HelloController{
        public:
            // Hello World controller
            static void Hello(Request request){
                std::string templatename = "hello.html";
                Context context;
                renderHtml(request, templatename, context);
            }

            // Documentation Controller
            static void Documentation(Request request){
                std::string templatename = "documentation-v1.1.html";
                Context context;
                renderHtml(request, templatename, context);
            }

    };

#endif