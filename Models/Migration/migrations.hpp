#ifndef __MHD_MIGRATIONS__
#define __MHD_MIGRATIONS__

#include "run-migrations.hpp"

// Import tables
#include "../user.hpp"
#include "../session.hpp"
#include "../client.hpp"
#include "../userprofile.hpp"


inline void initialize_tables() {

    // Add table here
    ORMModel* models[] = {
        new UserProfile,
        new User,
        new SessionTable,
        new Client
    };

    run_migrations(models, sizeof(models) / sizeof(ORMModel*));
}

#endif
