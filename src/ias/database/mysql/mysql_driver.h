/**
 * A class which describes the properties of a MySQL database driver.
 *
 * @date                    Jul 23, 2013
 * @author                  Joeri HERMANS
 * @version                 0.1
 *
 * Copyright 2013 Joeri HERMANS
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

#ifndef MYSQL_DRIVER_H_
#define MYSQL_DRIVER_H_

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <ias/database/interface/database_driver.h>

// END Includes. /////////////////////////////////////////////////////

class MySqlDriver : public DatabaseDriver {

    public:

    // BEGIN Class constants. ////////////////////////////////////////

    /**
     * Contains the identifier of the MySQL database driver.
     */
    static const char kMySqlIdentifier[];

    // END Class constants. //////////////////////////////////////////

    private:

    // BEGIN Private members. ////////////////////////////////////////
    // END Private members. //////////////////////////////////////////

    // BEGIN Private methods. ////////////////////////////////////////
    // END Private methods. //////////////////////////////////////////

    protected:

    // BEGIN Protected methods. //////////////////////////////////////
    // END Protected methods. ////////////////////////////////////////

    public:

    // BEGIN Constructors. ///////////////////////////////////////////

    MySqlDriver( void );

    // END Constructors. /////////////////////////////////////////////

    // BEGIN Destructor. /////////////////////////////////////////////

    virtual ~MySqlDriver( void );

    // END Destructor. ///////////////////////////////////////////////

    // BEGIN Public methods. /////////////////////////////////////////

    virtual DatabaseConnection * initializeConnection( 
        const std::string & username, const std::string & password,
        const std::string & schema, const std::string & host );

    // END Public methods. ///////////////////////////////////////////

    // BEGIN Static methods. /////////////////////////////////////////
    // END Static methods. ///////////////////////////////////////////

};

#endif /* MYSQL_DRIVER_H_ */
