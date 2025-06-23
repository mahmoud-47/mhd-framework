#ifndef __MHD_FILTER_
#define __MHD_FILTER_

    #include <functional>
    #include "../request/Request.hpp"
    #include "../render/HttpRender.hpp"
    #include "../../Models/user.hpp"

    /**
     * Controller for authenticated users only
     * Takes the controller function pointer and the login_url 
     * By default the login url is /login
     */
    std::function<void(Request&)> AuthenticatedUsersOnly(std::function<void(Request&)> functionController, std::string login_url = "/login"){
        return [=](Request &request){
            // check if the user is authenticated
            User* user = User::getAuthenticatedUser(request);
            if(user == nullptr){
                redirect(request, login_url);
                return;
            }
            // free memory
            delete user;
            // call the controller
            functionController(request);
        };
    }

    /**
     * Controller for unauthenticated users only
     * Takes the controller function pointer and the home_url 
     * By default the login url is /home
     */
    std::function<void(Request&)> UnAuthenticatedUsersOnly(std::function<void(Request&)> functionController, std::string home_url = "/home"){
        return [=](Request &request){
            // check if the user is authenticated
            User* user = User::getAuthenticatedUser(request);
            if(user != nullptr){
                delete user;
                redirect(request, home_url);
                return;
            }
            
            // call the controller
            functionController(request);
        };
    }
#endif