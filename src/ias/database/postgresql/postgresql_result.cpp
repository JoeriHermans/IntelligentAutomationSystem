/**
 * A class which describes the properties and actions of a PostgreSQL
 * database result.
 *
 * @date                    August 21, 2014
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

// System dependencies.
#include <cassert>
#include <cstdio>

// Application dependencies.
#include <ias/database/postgresql/postgresql_result.h>

// END Includes. /////////////////////////////////////////////////////

PostgresqlResult::PostgresqlResult( void ) {
    // TODO Implement.
}

PostgresqlResult::~PostgresqlResult( void ) {
    // TODO Implement.
}

bool PostgresqlResult::hasNext( void ) {
    // TODO Implement.

    return ( false );
}

DatabaseResultRow * PostgresqlResult::next( void ) {
    // TODO Implement.

    return ( nullptr );
}
