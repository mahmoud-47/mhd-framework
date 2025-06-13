#ifndef __CLIENT_Controller__
#define __CLIENT_Controller__

    
    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"

    class ClientController{
        public:
            // Display all clients
            static void DisplayAll(Request request){
                std::string templatename = "clients/list.html";
                Context context;
                renderHtml(request, templatename, context);
            }

            // create a client
            static void Create(Request request){
                std::string templatename = "clients/create.html";
                Context context;
                renderHtml(request, templatename, context);
            }

            // create a client
            static void Update(Request request){
                std::string templatename = "clients/edit.html";
                Context context;
                renderHtml(request, templatename, context);
            }

            // Details of a client
            static void Details(Request request){
                std::string templatename = "clients/details.html";
                Context context;
                renderHtml(request, templatename, context);
            }
    };

#endif