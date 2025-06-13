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

                if(request.getMethod() == "POST"){
                    std::string name = request.getFormDataParameterByParameterName("name");
                    std::string email = request.getFormDataParameterByParameterName("email");
                    std::string phone = request.getFormDataParameterByParameterName("phone");
                    std::string company = request.getFormDataParameterByParameterName("company");
                    std::string notes = request.getFormDataParameterByParameterName("notes");

                    // Debug output
                    std::cout << "----- Form Data Received -----" << std::endl;
                    std::cout << "Name: " << name << std::endl;
                    std::cout << "Email: " << email << std::endl;
                    std::cout << "Phone: " << phone << std::endl;
                    std::cout << "Company: " << company << std::endl;
                    std::cout << "Notes: " << notes << std::endl;
                    std::cout << "-----------------------------" << std::endl;
                }
                
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