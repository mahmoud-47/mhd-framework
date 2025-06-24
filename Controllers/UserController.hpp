#ifndef __MHD_USER_CONTROLLER_
#define __MHD_USER_CONTROLLER_

    #include <vector>
    #include "../utils/request/Request.hpp"
    #include "../utils/render/HttpRender.hpp"
    #include "../Models/user.hpp"
    #include "../utils/Exception/exception.hpp"
    #include "../utils/mail/send_mail.hpp"

    class UserController{

        public:

            static void regirer(Request request){
                std::string template_name = "user/register.html";
                Context context;
                User userQuery;

                if(request.getMethod() == "POST"){
                    std::string firstname = request.getFormDataParameterByParameterName("firstname");
                    std::string lastname = request.getFormDataParameterByParameterName("lastname");
                    std::string email = request.getFormDataParameterByParameterName("email");
                    std::string username = request.getFormDataParameterByParameterName("username");
                    std::string password1 = request.getFormDataParameterByParameterName("password1");
                    std::string password2 = request.getFormDataParameterByParameterName("password2");

                    if(password1 != password2){
                        context["error"] = ContextValue("true");
                        context["message"] = ContextValue("The passwords are not the same");
                        renderHtml(request, template_name, context);
                        return;
                    }

                    if(password1.length() < 4){
                        context["error"] = ContextValue("true");
                        context["message"] = ContextValue("The passwords should contain at least 4 characters");
                        renderHtml(request, template_name, context);
                        return;
                    }

                    if(userQuery.find_by("username", username).size() > 0){
                        context["error"] = ContextValue("true");
                        context["message"] = ContextValue("This username elready exists");
                        renderHtml(request, template_name, context);
                        return;
                    }

                    try{
                        User* user = User::create_user(firstname, lastname, email, username, password1);
                        if(user == nullptr){
                            context["error"] = ContextValue("true");
                            context["message"] = ContextValue("User not created !");
                            renderHtml(request, template_name, context);
                            return;
                        }
                        // sucess message
                        context["success"] = ContextValue("true");
                        context["message"] = ContextValue("User Created successfuly ! Check your Email !!!");

                        // Send a Thank you email
                        try{
                            std::string sender_name = "MHD++";
                            std::string subject = "Tnak you for creating an account !";
                            std::string receiver_email = email;
                            // creating mail object
                            MhdSendMail sender(sender_name, receiver_email, subject);
                            // add context
                            Context context;
                            context["firstname"] = ContextValue(firstname);
                            context["lastname"] = ContextValue(lastname);
                            // link the html file
                            sender.AttachHtmlMessage("mail/thankyou.html", context);
                            // send
                            sender.send();
                        }catch(MailException e){
                            std::cout << "--------- Did not send email " << e.getMessage() << "\n";
                        }

                        delete user;
                        renderHtml(request, template_name, context);
                        return;
                    }catch(SQLException e){
                        context["error"] = ContextValue("true");
                        context["message"] = ContextValue(e.getMessage());
                        renderHtml(request, template_name, context);
                        return;
                    }
                }

                renderHtml(request, template_name, context);
            }

            static void login(Request request){
                std::string template_name = "user/login.html";
                Context context;

                if(request.getMethod() == "POST"){
                    std::string username = request.getFormDataParameterByParameterName("username");
                    std::string password = request.getFormDataParameterByParameterName("password");

                    User *user = User::authenticate(request, username, password);
                    if(user){
                        delete user;
                        // redirect to home page
                        redirect(request, "/clients/all");
                        return;
                    }else{
                        context["error"] = ContextValue("true");
                        context["message"] = ContextValue("Username or password not correct");
                    }
                }

                renderHtml(request, template_name, context);
            }

            static void logout_user(Request request){
                // check if user is authenticated and redirect to login page if not
                User* user = User::getAuthenticatedUser(request);
                if(user != nullptr){
                    User::logout(request);
                    delete user; // free allocated memory
                }
                redirect(request, "/user/login");
                return;
            }

    };

#endif