#ifndef __MHD_MIGRATIONS__
#define __MHD_MIGRATIONS__

    /**
     * Add your model pointers in the models array
     */

    #include <vector>
    #include <functional>

    // Include the tables here
    #include "../user.hpp"
    #include "../session.hpp"

    void initialize_tables() {
        // Create Model objects

        // *** Add your classes here (new Model)
        ORMModel* models[] = {
            new User,
            new Session
        };

        if(dropRecreateTables){
            for(int i = 0; i < sizeof(models)/sizeof(ORMModel*); i++)
                models[i]->dropTable();
        }
        
        // Call create table method
        for(int i = 0; i < sizeof(models)/sizeof(ORMModel*); i++)
            models[i]->createTable();

        // Free allcated memory
        for(int i = 0; i < sizeof(models)/sizeof(ORMModel*); i++)
            delete models[i];

    }


#endif