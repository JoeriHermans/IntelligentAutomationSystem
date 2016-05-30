/**
 * A class which represents the properties and actions of a PostgreSQL
 * database connection.
 *
 * @date                    30 05 2016
 * @author                  Joeri HERMANS
 * @version                 0.1
 *
 * Copyright 2016 Joeri HERMANS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef IAS_DATABASE_DRIVER
#if IAS_DATABASE_DRIVER == 'P' || IAS_DATABASE_DRIVER == 'A'

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <ias/database/database_connection.h>

// END Includes. /////////////////////////////////////////////////////

#ifndef IAS_POSTGRESQL_DATABASE_CONNECTION_H_
#define IAS_POSTGRESQL_DATABASE_CONNECTION_H_

namespace ias {

class postgresql_database_connection : public database_connection {

    public:

    // BEGIN Class constants. ////////////////////////////////////////////////

    /**
     * Holds the default for PostgreSQL database connection as a string
     * representation.
     */
    static const char kDefaultPort[];

    /**
     * Holds the unique identifier for the PostgreSQL driver.
     */
    static const char kIdentifier[];

    // END Class constants. //////////////////////////////////////////////////

    private:

    // BEGIN Private members. ////////////////////////////////////////////////
    // END Private members. //////////////////////////////////////////////////

    // BEGIN Private methods. ////////////////////////////////////////////////

    inline void initialize(void);

    // END Private methods. //////////////////////////////////////////////////

    protected:

    // BEGIN Protected methods. //////////////////////////////////////////////
    // END Protected methods. ////////////////////////////////////////////////

    public:

    // BEGIN Constructor. ////////////////////////////////////////////////////

    postgresql_database_connection(const std::string & host,
                                   const std::string & schema,
                                   const std::string & username,
                                   const std::string & password);

    postgresql_database_connection(const std::string & host,
                                   const std::string & port,
                                   const std::string & schema,
                                   const std::string & username,
                                   const std::string & password);

    // END Constructor. //////////////////////////////////////////////////////

    // BEGIN Destructor. /////////////////////////////////////////////////////

    virtual ~postgresql_database_connection(void);

    // END Destructor. ///////////////////////////////////////////////////////

    // BEGIN Public methods. /////////////////////////////////////////////////

    virtual bool close_connection(void);

    virtual bool is_connected(void) const;

    virtual bool open_connection(void);

    virtual void * get_link(void) const;

    // END Public methods. ///////////////////////////////////////////////////

    // BEGIN Public static methods. //////////////////////////////////////////

    static void cleanup(void);

    // END Public static methods. ////////////////////////////////////////////

};

};

#endif
#endif
#endif
