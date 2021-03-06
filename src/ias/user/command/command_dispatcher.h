/**
 * A class which is responsible for dispatching user command to actions.
 *
 * @date                    Jul 18, 2014
 * @author                    Joeri HERMANS
 * @version                    0.1
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

#ifndef COMMAND_DISPATCHER_H_
#define COMMAND_DISPATCHER_H_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <string>
#include <vector>
#include <map>

// Application dependencies.
#include <ias/user/user.h>
#include <ias/user/command/command.h>

// END Includes. /////////////////////////////////////////////////////

class CommandDispatcher {

    public:

    // BEGIN Class constants. ////////////////////////////////////////
    // END Class constants. //////////////////////////////////////////

    private:

    // BEGIN Private members. ////////////////////////////////////////

    /**
     * A map which holds all commands and unique identifiers.
     */
    std::map<std::string,Command *> mCommands;

    // END Private members. //////////////////////////////////////////

    // BEGIN Private methods. ////////////////////////////////////////
    // END Private methods. //////////////////////////////////////////

    protected:

    // BEGIN Protected methods. //////////////////////////////////////
    // END Protected methods. ////////////////////////////////////////

    public:

    // BEGIN Constructors. ///////////////////////////////////////////

    CommandDispatcher( void );

    // END Constructors. /////////////////////////////////////////////

    // BEGIN Destructor. /////////////////////////////////////////////

    virtual ~CommandDispatcher( void );

    // END Destructor. ///////////////////////////////////////////////

    // BEGIN Public methods. /////////////////////////////////////////

    void registerCommand( const std::string & identifier,
                          Command * command );

    bool registered( const std::string & identifier ) const;

    std::string dispatch( User * user,
                          const std::string & identifier,
                          const std::string & parameter );

    std::vector<Command *> getCommands( void ) const;

    // END Public methods. ///////////////////////////////////////////

    // BEGIN Static methods. /////////////////////////////////////////
    // END Static methods. ///////////////////////////////////////////

};

#endif /* COMMAND_DISPATCHER_H_ */
