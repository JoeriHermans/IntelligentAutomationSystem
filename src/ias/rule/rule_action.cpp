/**
 * A class which describes the properties and actions of a rule action.
 *
 * @date                    Jul 31, 2014
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

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <cassert>

// Application dependencies.
#include <ias/rule/rule_action.h>

// END Includes. /////////////////////////////////////////////////////

void RuleAction::setDevice( Device * device ) {
    // Checking the precondition.
    assert( device != nullptr );
    
    mDevice = device;
}

void RuleAction::setAction( Action * action ) {
    // Checking the precondition.
    assert( action != nullptr );
    
    mAction = action;
}

RuleAction::RuleAction( Device * device , Action * action ) {
    setDevice(device);
    setAction(action);
}

RuleAction::~RuleAction( void ) {
    delete mAction; mAction = nullptr;
}

void RuleAction::execute( void ) const {
    mDevice->execute(*mAction);
}
