/**
 * Contains a list of all constants being used in IAS.
 *
 * @date					Jul 2, 2014
 * @author					Joeri HERMANS
 * @version					0.1
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

// Application dependencies.
#include <ias/application/constants.h>

// END Includes. /////////////////////////////////////////////////////

const char kFlagController[] = "--controller";
const char kFlagServer[] = "--server";
const char kFlagConfig[] = "--config";
const char kFlagClient[] = "--client";
const char kFlagAddress[] = "--address";
const char kFlagPort[] = "--port";

const char kTrimCharacters[] = " \t\n\r";

const char kVersion[] = "IAS 0.1";
const char kOnline[] = "Online";
const char kOffline[] = "Offline";

const char kConfigDatabaseDriver[] = "database_driver";
const char kConfigDatabaseUser[] = "database_username";
const char kConfigDatabasePassword[] = "database_password";
const char kConfigDatabaseSchema[] = "database_schema";
const char kConfigDatabaseHost[] = "database_host";
const char kConfigBindPort[] = "network_bind_port";
const char kConfigHost[] = "server_address";
const char kConfigHostPort[] = "server_port";
const char kConfigControllerIdentifier[] = "controller_identifier";
const char kConfigControllerSecurityCode[] = "controller_security_code";
const char kConfigDeviceList[] = "devices";
const char kConfigBindUserPort[] = "user_port";
const char kConfigPreSalt[] = "security_presalt";
const char kConfigPostSalt[] = "security_postsalt";

const char kProtocolAck[] = "ACK";
const char kProtocolNack[] = "NACK";

const char kDefaultServerAddress[] = "127.0.0.1";
const unsigned int kDefaultDeviceServerPort = 5001;
const unsigned int kDefaultControllerServerPort = 5000;
const unsigned int kDefaultUserServerPort = 4999;