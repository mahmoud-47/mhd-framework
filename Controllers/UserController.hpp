#ifndef __MHD_USER_CONTROLLER_
#define __MHD_USER_CONTROLLER_

    #include <vector>
    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"
    #include "../Models/user.hpp"
    #include "utils/Exception/exception.hpp"

    class UserController{

        public:

            static void regirer(Request request){
                std::string template_name = "user/register.html";
                Context contect;
                renderHtml(request, template_name, contect);
            }

            static void login(Request request){
                std::string template_name = "user/login.html";
                Context contect;
                renderHtml(request, template_name, contect);
            }

    };

#endif