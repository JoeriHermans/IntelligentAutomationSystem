/**
 * A class which will act as a data access layer through a MySQL
 * database connection.
 *
 * @date                    01 06 2016
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
#if IAS_DATABASE_DRIVER == 'M' || IAS_DATABASE_DRIVER == 'A'

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <cassert>
#include <cstring>
#include <unistd.h>

// Application dependencies.
#include <ias/database/database_connection.h>
#include <ias/technology/data_access/mysql_value_type_data_access.h>
#include <ias/logger/logger.h>

// END Includes. /////////////////////////////////////////////////////

namespace ias {

    // BEGIN Constants. //////////////////////////////////////////////

    const char mysql_value_type_data_access::kStmtGetAll[] =
        "SELECT id, pattern FROM value_types;";

    const char mysql_value_type_data_access::kStmtGetId[] =
        "SELECT id, pattern FROM value_types WHERE id = ?;";

    const char mysql_value_type_data_access::kStmtRemove[] =
        "DELETE FROM value_types WHERE id = ?;";

    // END Constants. ////////////////////////////////////////////////

    inline void mysql_value_type_data_access::initialize(void) {
        mDbConnection = nullptr;
        mStmtGetAll = nullptr;
        mStmtGetId = nullptr;
        mStmtRemove = nullptr;
    }

    void mysql_value_type_data_access::set_database_connection(ias::database_connection * dbConn) {
        // Checking the precondition.
        assert(dbConn != nullptr && dbConn->is_connected());

        mDbConnection = static_cast<MYSQL *>(dbConn->get_link());
    }

    void mysql_value_type_data_access::initialize_statements(void) {
        mStmtGetAll = mysql_stmt_init(mDbConnection);
        prepare_statement_get_all();
        mStmtGetId = mysql_stmt_init(mDbConnection);
        prepare_statement_get_id();
        mStmtRemove = mysql_stmt_init(mDbConnection);
        prepare_statement_remove();
    }

    void mysql_value_type_data_access::prepare_statement_get_all(void) {
        int rc;

        // Checking the precondition.
        assert(mStmtGetAll != nullptr);

        rc = mysql_stmt_prepare(mStmtGetAll, kStmtGetAll,
                                strlen(kStmtGetAll));
        // Check if the statement could be prepared.
        if(rc != 0)
            loge(mysql_stmt_error(mStmtGetAll));
    }

    void mysql_value_type_data_access::prepare_statement_get_id(void) {
        int rc;

        // Checking the precondition.
        assert(mStmtGetId != nullptr);

        rc = mysql_stmt_prepare(mStmtGetId, kStmtGetId,
                                strlen(kStmtGetId));
        // Check if the statement could be prepared.
        if(rc != 0)
            loge(mysql_stmt_error(mStmtGetId));
    }

    void mysql_value_type_data_access::prepare_statement_remove(void) {
        int rc;

        // Checking the precondition.
        assert(mStmtRemove != nullptr);

        rc = mysql_stmt_prepare(mStmtRemove, kStmtRemove,
                                strlen(kStmtRemove));
        // Check if the statement could be prepared.
        if(rc != 0)
            loge(mysql_stmt_error(mStmtRemove));
    }

    void mysql_value_type_data_access::close_statements(void) {
        mysql_stmt_close(mStmtGetAll);
        mStmtGetAll = nullptr;
        mysql_stmt_close(mStmtGetId);
        mStmtGetId = nullptr;
        mysql_stmt_close(mStmtRemove);
        mStmtRemove = nullptr;
    }

    mysql_value_type_data_access::mysql_value_type_data_access(ias::database_connection * dbConn) {
        initialize();
        set_database_connection(dbConn);
        initialize_statements();
    }

    mysql_value_type_data_access::~mysql_value_type_data_access(void) {
        cache_clear();
        close_statements();
    }

    std::vector<ias::value_type *> mysql_value_type_data_access::get_all(void) {
        std::vector<ias::value_type *> types;
        MYSQL_BIND result[2];

        // Buffer variables.
        std::size_t bufferId = 0;
        char bufferPattern[kDefaultStringSize + 1];

        // Clear the parameter structures.
        memset(result, 0, sizeof result);
        // Prepare the result types.
        // id
        result[0].buffer_type     = MYSQL_TYPE_LONG;
        result[0].buffer          = static_cast<void *>(&bufferId);
        result[0].is_unsigned     = 1;
        // pattern
        result[1].buffer_type     = MYSQL_TYPE_VAR_STRING;
        result[1].buffer          = static_cast<void *>(bufferPattern);
        result[1].buffer_length   = kDefaultStringSize;

        // Bind the result buffers.
        mysql_stmt_bind_result(mStmtGetAll, result);
        if(mysql_stmt_execute(mStmtGetAll) == 0) {
            if(mysql_stmt_store_result(mStmtGetAll) == 0) {
                // Retrieve all elements.
                while(!mysql_stmt_fetch(mStmtGetAll)) {
                    ias::value_type * type;

                    // Type conversions to match class constructor.
                    std::size_t id           = bufferId;
                    std::string pattern      = bufferPattern;

                    // Allocate a new value type instance.
                    type = new ias::value_type(id, pattern);
                    // Store the type in the local vector.
                    types.push_back(type);
                }
                // Store all elements in the cache.
                cache_store_fast(types);
            } else {
                loge(mysql_stmt_error(mStmtGetAll));
            }
        } else {
            loge(mysql_stmt_error(mStmtGetAll));
        }

        return types;
    }

    ias::value_type * mysql_value_type_data_access::get(const std::size_t id) {
        ias::value_type * type = nullptr;

        // TODO Implement.

        return type;
    }

    void mysql_value_type_data_access::add(ias::value_type * element) {
        // Checking the precondition.
        assert(element != nullptr);

        // TODO Implement.
    }

    void mysql_value_type_data_access::remove(ias::value_type * element) {
        // Checking the precondition.
        assert(element != nullptr);

        // Check if there is a cache hit.
        if(cache_contains(element))
            // Remove the element.
            remove(element->get_id());
    }

    void mysql_value_type_data_access::remove(const std::size_t id) {
        // TODO Implement.
    }

    void mysql_value_type_data_access::update(ias::value_type * element) {
        // Checking the precondition.
        assert(element != nullptr);

        // TODO Implement.
    }

};

#endif
#endif
