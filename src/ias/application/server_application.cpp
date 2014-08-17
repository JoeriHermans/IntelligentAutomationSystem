/**
 * A class which describes the actions and properties of the IAS
 * server applications.
 *
 * @date                    Jul 2, 2014
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

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

// Application dependencies.
#include <ias/ai/nlp/natural_language_processor.h>
#include <ias/application/server_application.h>
#include <ias/application/constants.h>
#include <ias/building/factory/area_factory_database.h>
#include <ias/building/factory/building_factory_database.h>
#include <ias/device/factory/device_database_factory.h>
#include <ias/technology/factory/value_type_database_factory.h>
#include <ias/technology/factory/member_database_factory.h>
#include <ias/technology/factory/feature_database_factory.h>
#include <ias/technology/factory/technology_database_factory.h>
#include <ias/controller/factory/controller_database_factory.h>
#include <ias/network/posix/posix_tcp_server_socket.h>
#include <ias/util/util.h>
#include <ias/user/command/command_stop.h>
#include <ias/user/command/command_state.h>
#include <ias/user/command/command_list_controllers.h>
#include <ias/user/command/command_execute_feature.h>
#include <ias/user/command/command_say.h>
#include <ias/user/command/command_list_commands.h>
#include <ias/user/command/command_delete_rule.h>
#include <ias/rule/factory/rule_database_factory.h>
#include <ias/user/command/command_list_rules.h>
#include <ias/user/command/command_load_rule.h>

// END Includes. /////////////////////////////////////////////////////

inline void ServerApplication::initialize( void ) {
    mDbConnection = nullptr;
    mServerController = nullptr;
    mServerUser = nullptr;
    mFlagRunning = true;
    mNlp = nullptr;
    mDeviceMonitor = nullptr;
}

void ServerApplication::setup( const int argc , const char ** argv ) {
    int index;

    // Retrieve the configuration file path.
    index = flagIndex(argc,argv,kFlagConfig);
    if( index >= 0 && (index + 1) <= argc && strlen(argv[index + 1]) > 0 ) {
        std::string configurationPath;
        
        configurationPath = argv[index + 1];
        readConfiguration(configurationPath);
        connectToDatabase();
        // Check if a connection with the database is available.
        if( mDbConnection != nullptr ) {
            mDeviceMonitor = new DeviceMonitor(mDbConnection);
            registerOperators();
            initializeSalts();
            fillContainers();
            initializeNlp();
            initializeDispatcher();
            initializeControllerServer();
            initializeUserServer();
        }
    }
}

void ServerApplication::readConfiguration( const std::string & filePath ) {
    std::ifstream file(filePath);
    std::string line;
    std::string key;
    std::string value;
    std::size_t i;

    // Checking the precondition.
    assert( !filePath.empty() );

    while( std::getline(file,line) ) {
        trim(line);
        if( line.empty() || line.at(0) == '#' )
            continue;
        i = line.find('=');
        key = line.substr(0,i);
        value = line.substr(i + 1,line.length());
        trim(key); trim(value);
        if( !key.empty() && !value.empty() )
            mProperties.add(key,value);
    }
    file.close();
}

void ServerApplication::connectToDatabase( void ) {
    if( !mProperties.contains(kConfigDatabaseUser) || 
        !mProperties.contains(kConfigDatabasePassword) ||
        !mProperties.contains(kConfigDatabaseHost) ||
        !mProperties.contains(kConfigDatabaseSchema) ||
        !mProperties.contains(kConfigDatabaseDriver) )
        return;

    const std::string & dbuser = mProperties.get(kConfigDatabaseUser);
    const std::string & dbpass = mProperties.get(kConfigDatabasePassword);
    const std::string & dbschema = mProperties.get(kConfigDatabaseSchema);
    const std::string & dbhost = mProperties.get(kConfigDatabaseHost);
    const std::string & dbdriver = mProperties.get(kConfigDatabaseDriver);

    // Check if the required member have been specified.
    if( !dbuser.empty() && !dbpass.empty() && !dbdriver.empty() &&
        !dbschema.empty() && !dbhost.empty() ) {
        // Check if a valid db driver identifier has been specified.
        if( dbdriver == MySqlDriver::kMySqlIdentifier ) {
            mDbConnection = MySqlDriver().initializeConnection(dbuser,dbpass,
                                                               dbschema,dbhost);
            if( !mDbConnection->connect() ) {
                delete mDbConnection;
                mDbConnection = nullptr;
            }
        }
    }
}

void ServerApplication::fillContainers( void ) {
    fillUsers();
    fillValueTypes();
    fillMembers();
    fillFeatures();
    fillTechnologies();
    fillDevices();
    fillControllers();
    fillAreas();
    fillBuildings();
    fillRules();
}

void ServerApplication::fillUsers( void ) {
    UserDatabaseFactory factory(mDbConnection);
    std::vector<User *> users;
    User * u;
    
    users = factory.fetchAll();
    for( auto it = users.begin() ; it != users.end() ; ++it ) {
        u = (*it);
        mContainerUsers.add((*it));
    }
}

void ServerApplication::fillValueTypes( void ) {
    ValueTypeDatabaseFactory factory(mDbConnection);
    std::vector<ValueType *> types;
    
    types = factory.fetchAll();
    for( auto it = types.begin() ; it != types.end() ; ++it ) {
        mContainerValueTypes.add((*it));
    }
}

void ServerApplication::fillMembers( void ) {
    MemberDatabaseFactory factory(mDbConnection,&mContainerValueTypes);
    std::vector<Member *> members;
    
    members = factory.fetchAll();
    for( auto it = members.begin() ; it != members.end() ; ++it ) {
        mContainerMembers.add((*it));
    }
}

void ServerApplication::fillFeatures( void ) {
    FeatureDatabaseFactory factory(mDbConnection,&mContainerValueTypes);
    std::vector<Feature *> features;
    
    features = factory.fetchAll();
    for( auto it = features.begin() ; it != features.end() ; ++it ) {
        mContainerFeatures.add((*it));
    }
}

void ServerApplication::fillTechnologies( void ) {
    std::vector<Technology *> technologies;
    TechnologyDatabaseFactory factory(mDbConnection,
                                      &mContainerMembers,
                                      &mContainerFeatures);

    technologies = factory.fetchAll();
    for( auto it = technologies.begin() ; it != technologies.end() ; ++it ) {
        mContainerTechnologies.add((*it));
    }
}

void ServerApplication::fillDevices( void ) {
   DeviceDatabaseFactory factory(
       mDbConnection,&mContainerTechnologies,mDeviceMonitor);
   std::vector<Device *> devices;
   
   devices = factory.fetchAll();
   for( auto it = devices.begin() ; it != devices.end() ; ++it ) {
       mContainerDevices.add((*it));
   }
}

void ServerApplication::fillControllers( void ) {
    ControllerDatabaseFactory factory(mDbConnection,&mContainerDevices);
    std::vector<Controller *> controllers;
    
    controllers = factory.fetchAll();
    for( auto it = controllers.begin() ; it != controllers.end() ; ++it ) {
        mContainerControllers.add((*it));
    }
}

void ServerApplication::fillAreas( void ) {
    AreaDatabaseFactory factory(mDbConnection,&mContainerDevices);
    std::vector<Area *> areas;
    
    areas = factory.fetchAll();
    for( auto it = areas.begin() ; it != areas.end() ; ++it ) {
        mContainerAreas.add((*it));
    }
}

void ServerApplication::fillRules( void ) {
    RuleDatabaseFactory factory(mDbConnection,&mContainerDevices,&mOperators);
    std::vector<Rule *> rules;
    
    rules = factory.fetchAll();
    for( auto it = rules.begin() ; it != rules.end() ; ++it ) {
        mContainerRules.add((*it));
    }
}

void ServerApplication::fillBuildings( void ) {
    BuildingDatabaseFactory factory(mDbConnection,&mContainerAreas);
    std::vector<Building *> buildings;
    
    buildings = factory.fetchAll();
    for( auto it = buildings.begin() ; it != buildings.end() ; ++it ) {
        mContainerBuildings.add((*it));
    }
}

void ServerApplication::initializeSalts( void ) {
    std::string preSalt;
    std::string postSalt;
    
    if( mProperties.contains(kConfigPreSalt) )
        preSalt = mProperties.get(kConfigPreSalt);
    if( mProperties.contains(kConfigPostSalt) )
        postSalt = mProperties.get(kConfigPostSalt);
    gSaltPre = preSalt;
    gSaltPost = postSalt;
}

void ServerApplication::initializeNlp( void ) {
    mNlp = new NaturalLanguageProcessor(&mContainerAreas,
                                        &mContainerDevices,
                                        &mContainerTechnologies);
}

void ServerApplication::initializeControllerServer( void ) {
    ServerSocket * serverSocket;
    std::string stringPort;
    unsigned int port;
    
    if( mProperties.contains(kConfigBindUserPort) )
        stringPort = mProperties.get(kConfigBindUserPort);
    if( !stringPort.empty() )
        port = (unsigned int) atol(stringPort.c_str());
    else
        port = kDefaultControllerServerPort;
    serverSocket = new PosixTcpServerSocket(port);
    if( serverSocket->bindToPort() ) {
        mServerController = new ControllerServer(serverSocket,
                                                 &mContainerControllers);
    } else {
        delete serverSocket;
    }
}

void ServerApplication::initializeUserServer( void ) {
    ServerSocket * serverSocket;
    std::string stringPort;
    unsigned int port;
    
    if( mProperties.contains(kConfigBindUserPort) )
        stringPort = mProperties.get(kConfigBindUserPort);
    if( !stringPort.empty() )
        port = (unsigned int) atol(stringPort.c_str());
    else
        port = kDefaultUserServerPort;
    serverSocket = new PosixTcpServerSocket(port);
    if( serverSocket->bindToPort() ) {
        mServerUser = new UserServer(serverSocket,
                                     &mContainerUsers,
                                     &mDispatcher);
    } else {
        delete serverSocket;
    }
}

void ServerApplication::initializeDispatcher( void ) {
    mDispatcher.registerCommand(
        CommandStop::kIdentifier,
        new CommandStop(this)
    );
    mDispatcher.registerCommand(
        CommandState::kIdentifier,
        new CommandState(&mContainerDevices)
    );
    mDispatcher.registerCommand(
        CommandListControllers::kIdentifier,
        new CommandListControllers(&mContainerControllers)
    );
    mDispatcher.registerCommand(
        CommandExecuteFeature::kIdentifier,
        new CommandExecuteFeature(&mContainerDevices)
    );
    mDispatcher.registerCommand(
        CommandSay::kIdentifier,
        new CommandSay(mNlp)
    );
    mDispatcher.registerCommand(
        CommandListCommands::kIdentifier,
        new CommandListCommands(&mDispatcher)
    );
    mDispatcher.registerCommand(
        CommandDeleteRule::kIdentifier,
        new CommandDeleteRule(&mContainerDevices,&mContainerRules)
    );
    mDispatcher.registerCommand(
        CommandListRules::kIdentifier,
        new CommandListRules(&mContainerRules)
    );
    mDispatcher.registerCommand(
        CommandLoadRule::kIdentifier,
        new CommandLoadRule(mDbConnection,&mContainerDevices,
                            &mContainerRules,&mOperators)
    );
}

void ServerApplication::registerOperators( void ) {
    mOperators[OperatorEquals::kIdentifier] = new OperatorEquals();
    mOperators[OperatorGreaterThan::kIdentifier] = new OperatorGreaterThan();
    mOperators[OperatorLessThan::kIdentifier] = new OperatorLessThan();
    mOperators[OperatorNotEquals::kIdentifier] = new OperatorNotEquals();
    mOperators[OperatorLessThanEquals::kIdentifier] = new OperatorLessThanEquals();
    mOperators[OperatorGreaterThanEquals::kIdentifier] = new OperatorGreaterThanEquals();
}

void ServerApplication::cleanupOperators( void ) {
    for( auto it = mOperators.begin() ; it != mOperators.end() ; ++it ) {
        delete it->second;
    }
}

ServerApplication::ServerApplication( const int argc,
                                      const char ** argv ) {
    initialize();
    setup(argc,argv);
}

ServerApplication::~ServerApplication( void ) {
    delete mDbConnection; mDbConnection = nullptr;
    delete mServerController; mServerController = nullptr;
    delete mServerUser; mServerUser = nullptr;
    delete mNlp; mNlp = nullptr;
    delete mDeviceMonitor; mDeviceMonitor = nullptr;
    cleanupOperators();
}

void ServerApplication::run( void ) {
    if( mServerController != nullptr &&
        mServerUser != nullptr ) {
        mServerController->start();
        mServerUser->start();
        mServerController->join();
        mServerUser->join();
    }
}

void ServerApplication::stop( void ) {
    mFlagRunning = false;
    if( mServerController != nullptr )
        mServerController->stop();
    if( mServerUser != nullptr )
        mServerUser->stop();
}
