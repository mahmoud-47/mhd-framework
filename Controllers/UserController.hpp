#ifndef __MHD_USER_CONTROLLER_
#define __MHD_USER_CONTROLLER_

    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"
    #include "../Models/user.hpp"
    #include "utils/Exception/exception.hpp"

    class UserController{
        public:

            static void createUser(Request request){
                User mhd(1, "Mohamed", "Gaye");
                User csroot(2, "CS", "Root");
                try{
                    mhd.save();
                    csroot.save();
                    csroot.save(); // problem coz duplicate id
                }catch(SQLException e){
                    std::cout << "Exception caught while inserting " << e.getMessage() << std::endl;
                }
                
                renderText(request, "Users saved successfully !!");
            }

            static void getUser(Request request){
                std::string userId = request.getGETParameterByParameterName("userId");
                User userquery;
                User* userPtr = dynamic_cast<User*>(userquery.find_by_id(userId));
                
                std::string message = "User name = " + userPtr->getName() + " Email = " + userPtr->getEmail() + 
                " Id = " + std::to_string(userPtr->getId());
                if(userPtr == nullptr) message += "Null";
                renderText(request, message);
            }
    };

#endif