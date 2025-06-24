#ifndef __MHD_FILTER_
#define __MHD_FILTER_

    #include <functional>
    #include "../request/Request.hpp"
    #include "../render/HttpRender.hpp"
    #include "../../Models/user.hpp"

    /**
     * @brief Controller wrapper for authenticated users only.
     *
     * This function wraps a controller and checks if the user is authenticated.
     * If not, it redirects to the login page.
     *
     * @param functionController The actual controller function to call if authenticated.
     * @param login_url The URL to redirect to if the user is not logged in (default: "/login").
     * @return A wrapped controller function that performs authentication check before calling the controller.
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
     * @brief Controller for unauthenticated users only.
     *
     * This function wraps a controller to ensure that only unauthenticated users can access it.
     * If the user is already authenticated, it redirects them to the home page.
     *
     * @param functionController The controller function to execute if the user is not authenticated.
     * @param home_url The URL to redirect authenticated users to (default: "/home").
     * @return A wrapped controller function that prevents access to authenticated users.
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