#ifndef __MHD_GET_ITEMS__
#define __MHD_GET_ITEMS__

    #include<string>

    // Get the method from the buffer
    std::string get_method_from_buffer(char* buffer);

    // Get route
    std::string get_route_from_buffer(char* buffer);

#endif