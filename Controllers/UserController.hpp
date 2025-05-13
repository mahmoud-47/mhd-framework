#ifndef __MHD_USER_CONTROLLER_
#define __MHD_USER_CONTROLLER_

    #include<vector>
    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"
    #include "../Models/user.hpp"
    #include "utils/Exception/exception.hpp"

    class UserController{
        public:

            static void createUser(Request request){
                User mhd(1, "Mohamed", "Gaye");
                User csroot(2, "CS", "Root");
                User Jule(3, "Jul", "Le");
                User akuna(4, "Akuna", "Matat");
                User Ndarei(5, "Khady", "Ndarei");
                try{
                    mhd.save();
                    csroot.save();
                    Jule.save(); 
                    akuna.save();
                    Ndarei.save();
                }catch(SQLException e){
                    std::cout << "Exception caught while inserting " << e.getMessage() << std::endl;
                }
                
                renderText(request, "Users saved successfully !!");
            }

            static void getUser(Request request){
                std::string userId = request.getGETParameterByParameterName("userId");
                User userquery;
                User* userPtr = dynamic_cast<User*>(userquery.find_by_id(userId));
                std::string message = "";
                if(userPtr)
                message += "User name = " + userPtr->name + " Email = " + userPtr->email + 
                " Id = " + std::to_string(userPtr->id);
                if(userPtr == nullptr) message += "Null";
                renderText(request, message);
            }

            static void getAllUsers(Request request) {
                User userquery;
                std::vector<ORMModel*> allusers = userquery.find_all();
                std::cout << "Total users found: " << allusers.size() << std::endl;
                
                std::stringstream message;
                
                if (allusers.empty()) {
                    message << "No users found in the database.";
                } else {
                    for (size_t i = 0; i < allusers.size(); ++i) {
                        User* converted_user = dynamic_cast<User*>(allusers[i]);
                        if (converted_user) {
                            // Add a separator between users
                            if (i > 0) {
                                message << "\n----------------------------------------<br>";
                            }
                            
                            message << "Id = " << converted_user->id;
                            message << "\nName = " << converted_user->name;
                            message << "\nEmail = " << converted_user->email << "\n";
                        } else {
                            std::cerr << "Warning: Failed to cast ORMModel* to User* at index " << i << ".\n";
                        }
                    }
                }
                
                // Don't forget to clean up memory
                for (auto* model : allusers) {
                    delete model;
                }
                
                renderText(request, message.str());
            }


            static void deluser(Request request){
                std::string userId = request.getGETParameterByParameterName("userId");
                User userquery;
                User *user = dynamic_cast<User*>(userquery.find_by_id(userId));
                if(user){
                    bool deleted = user->remove(std::to_string(user->id));
                    if(deleted)
                        renderText(request, "User Deleted !");
                    else
                        renderText(request, "User Not Deleted !");
                }else
                    renderText(request, "User NULL !");
            }


            static void updateUser(Request request){
                std::string userId = request.getGETParameterByParameterName("userId");
                User userquery;
                User *user = dynamic_cast<User*>(userquery.find_by_id(userId));
                if(user){
                    user->name = "New name";
                    user->email = "New Email";
                    bool updated = user->update(std::to_string(user->id));
                    if(updated)
                        renderText(request, "User Updated !");
                    else
                        renderText(request, "User Not Updated !");
                }else
                    renderText(request, "User NULL !");
            }
    };

#endif