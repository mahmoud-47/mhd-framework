#ifndef __CLIENT_Controller__
#define __CLIENT_Controller__

    
    #include "utils/request/Request.hpp"
    #include "utils/render/HttpRender.hpp"
    #include "../Models/client.hpp"

    class ClientController{
        public:
            // Display all clients
            static void DisplayAll(Request request){
                // mister query
                Client clientQuery;

                // get all non converted clients
                std::vector<ORMModel*> clients = clientQuery.find_all();

                // add all converted clients to a list
                ContextArray clientContextArray;
                ContextObject clientContextObject;
                for(int i = 0; i < clients.size(); i++){
                    Client* converted_client = static_cast<Client*> (clients[i]);
                    clientContextObject["id"] = ContextValue(std::to_string(converted_client->id));
                    clientContextObject["name"] = ContextValue(converted_client->name);
                    clientContextObject["phone"] = ContextValue(converted_client->phone);
                    clientContextObject["email"] = ContextValue(converted_client->email);
                    clientContextObject["company"] = ContextValue(converted_client->company);
                    clientContextObject["notes"] = ContextValue(converted_client->notes);
                    clientContextArray.push_back(clientContextObject);
                }

                std::string templatename = "clients/list.html";
                Context context;
                context["clients"] = clientContextArray;

                // if from redirected url from update, display the message
                std::string updated = request.getQueryParameterByParameterName("updated");
                std::string clientUpdatedName = request.getQueryParameterByParameterName("clientUpdatedName");
                if(updated == "true"){
                    context["updated"] = ContextString("true");
                    context["clientUpdatedName"] = ContextString(clientUpdatedName);
                }

                // if from redirection from deletion
                std::string deletion = request.getQueryParameterByParameterName("deletion");
                std::string deletion_id = request.getQueryParameterByParameterName("del-id");
                std::string deletion_name = request.getQueryParameterByParameterName("del-name");
                if(deletion == "true"){
                    context["ok-deletion"] = ContextString("true");
                    context["del-name"] = ContextString(deletion_name);
                }
                if(deletion == "false"){
                    context["ko-deletion"] = ContextString("true");
                    context["del-id"] = ContextString(deletion_id);
                }

                renderHtml(request, templatename, context);
            }

            // create a client
            static void Create(Request request){
                std::string templatename = "clients/create.html";
                Context context;

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

                    try{
                        Client client(0, name, email, phone, company, notes, MhdDateTime());
                        client.save();
                        context["save"] = ContextValue("true");
                    }catch(SQLException e){
                        context["fail"] = ContextValue("true");
                    }
                }
                
                renderHtml(request, templatename, context);
            }

            // create a client
            static void Update(Request request){
                std::string templatename = "clients/edit.html";
                Context context;

                if(request.getMethod() == "POST"){
                    std::string id = request.getGETParameterByParameterName("id");
                    std::string name = request.getFormDataParameterByParameterName("name");
                    std::string email = request.getFormDataParameterByParameterName("email");
                    std::string phone = request.getFormDataParameterByParameterName("phone");
                    std::string company = request.getFormDataParameterByParameterName("company");
                    std::string notes = request.getFormDataParameterByParameterName("notes");

                    // Update the client
                    try{
                        Client client = Client(std::stoi(id), name, email, phone, company, notes, MhdDateTime()); // need to fetch the old time
                        client.update(id);
                        // context["updated"] = ContextValue("true");
                        redirect(request, "/clients/all?updated=true&clientUpdatedName="+name);
                        return;
                    }catch(SQLException &e){
                        context["failed"] = ContextValue("true");
                        context["message"] = ContextValue(e.getMessage());
                    }

                }
                
                // fetch the client
                Client clientQuery;
                Client* client = nullptr;
                try{
                    client = static_cast<Client*>(clientQuery.find_by_id(request.getGETParameterByParameterName("id")));
                    if(client == nullptr){
                        renderText(request, "No client found with this id");
                        return;
                    }
                    
                }catch(SQLException& e){
                    renderText(request, e.getMessage());
                    return;
                }

                
                context["id"] = ContextValue(std::to_string(client->id));
                context["name"] = ContextValue(client->name);
                context["email"] = ContextValue(client->email);
                context["phone"] = ContextValue(client->phone);
                context["company"] = ContextValue(client->company);
                context["notes"] = ContextValue(client->notes);

                renderHtml(request, templatename, context);
            }

            // Details of a client
            static void Details(Request request){
                std::string templatename = "clients/details.html";

                Client clientQuery;
                Client* client = nullptr;
                try{
                    client = static_cast<Client*>(clientQuery.find_by_id(request.getGETParameterByParameterName("id")));
                    if(client == nullptr){
                        renderText(request, "No client found with this id");
                        return;
                    }
                    
                }catch(SQLException& e){
                    renderText(request, e.getMessage());
                    return;
                }

                Context context;
                context["id"] = ContextValue(std::to_string(client->id));
                context["name"] = ContextValue(client->name);
                context["email"] = ContextValue(client->email);
                context["phone"] = ContextValue(client->phone);
                context["company"] = ContextValue(client->company);
                context["notes"] = ContextValue(client->notes);
                context["created-at"] = ContextValue(client->created_at.get_day_name() + " " + client->created_at.get_month_name() + " " + std::to_string(client->created_at.get_year()));

                renderHtml(request, templatename, context);
            }

            // Details of a client
            static void Delete(Request request){
                std::string id = request.getGETParameterByParameterName("id");
                std::string clientname;
                try{
                    Client clientQuery;
                    Client* client = static_cast<Client*>(clientQuery.find_by_id(id)); 
                    // if client not found, meaage
                    if(client == nullptr){
                        redirect(request, "/clients/all?deletion=false&del-id="+id);
                        return;
                    }
                    clientname = client->name;
                    clientQuery.remove(id);
                }catch(SQLException& e){
                    redirect(request, "/clients/all?deletion=false&del-id="+id);
                    return;
                }
                redirect(request, "/clients/all?deletion=true&del-name="+ clientname);
            }
    };

#endif