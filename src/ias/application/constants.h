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

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// BEGIN Includes. ///////////////////////////////////////////////////
// END Includes. /////////////////////////////////////////////////////

extern const char kFlagController[];
extern const char kFlagServer[];
extern const char kFlagConfig[];
extern const char kFlagClient[];
extern const char kFlagAddress[];
extern const char kFlagPort[];

extern const char kTrimCharacters[];

extern const char kVersion[];
extern const char kOnline[];
extern const char kOffline[];

extern const char kConfigDatabaseDriver[];
extern const char kConfigDatabaseUser[];
extern const char kConfigDatabasePassword[];
extern const char kConfigDatabaseSchema[];
extern const char kConfigDatabaseHost[];
extern const char kConfigBindPort[];
extern const char kConfigHost[];
extern const char kConfigHostPort[];
extern const char kConfigControllerIdentifier[];
extern const char kConfigControllerSecurityCode[];
extern const char kConfigDeviceList[];
extern const char kConfigBindUserPort[];
extern const char kConfigPreSalt[];
extern const char kConfigPostSalt[];

extern const char kProtocolAck[];
extern const char kProtocolNack[];

extern const char kDefaultServerAddress[];
extern const unsigned int kDefaultControllerServerPort;
extern const unsigned int kDefaultDeviceServerPort;
extern const unsigned int kDefaultUserServerPort;

#endif /* CONSTANTS_H_ */