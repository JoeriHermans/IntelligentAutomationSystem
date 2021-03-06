/**
 * An operator which checks if a specified string is a substring of
 * an argument.
 *
 * @date                    December 13, 2014
 * @author                  Joeri HERMANS
 * @version                 0.1
 *
 * Copyright 2014 Joeri HERMANS
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
#include <ias/operator/operator_contains.h>

// END Includes. /////////////////////////////////////////////////////

// BEGIN Constants. //////////////////////////////////////////////////

const char OperatorContains::kIdentifier[] = "in";

// END Constants. ////////////////////////////////////////////////////

OperatorContains::OperatorContains( void ) :
    Operator(kIdentifier) {
    // Nothing to do here.
}

bool OperatorContains::evaluate( const std::string & leftExpression,
                                 const std::string & rightExpression ) const {
    return ( leftExpression.find(rightExpression) != std::string::npos );
}
