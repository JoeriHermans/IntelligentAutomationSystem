/**
 * A class which is responsible for writing bytes to a POSIX TCP socket.
 *
 * @date                    July 6, 2014
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

#ifndef POSIX_TCP_SOCKET_WRITER_H_
#define POSIX_TCP_SOCKET_WRITER_H_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <mutex>

// Application dependencies.
#include <ias/io/writer/writer.h>

// Forward declarations.
class PosixTcpSocket;

// END Includes. /////////////////////////////////////////////////////

class PosixTcpSocketWriter : public Writer {

    public:

    // BEGIN Class constants. ////////////////////////////////////////
    // END Class constants. //////////////////////////////////////////

    private:

    // BEGIN Private members. ////////////////////////////////////////

    /**
     * Contains the socket to which this writer will write to.
     */
    PosixTcpSocket * mSocket;

    /**
     * Contains the mutex which is responsible for synchronous I/O.
     */
    std::mutex mMutex;

    // END Private members. //////////////////////////////////////////

    // BEGIN Private methods. ////////////////////////////////////////

    void setSocket( PosixTcpSocket * socket );

    // END Private methods. //////////////////////////////////////////

    protected:

    // BEGIN Protected methods. //////////////////////////////////////
    // END Protected methods. ////////////////////////////////////////

    public:

    // BEGIN Constructors. ///////////////////////////////////////////

    PosixTcpSocketWriter( PosixTcpSocket * socket );

    // END Constructors. /////////////////////////////////////////////

    // BEGIN Destructor. /////////////////////////////////////////////

    virtual ~PosixTcpSocketWriter( void ) = default;

    // END Destructor. ///////////////////////////////////////////////

    // BEGIN Public methods. /////////////////////////////////////////

    virtual void closeWriter( void );

    virtual std::size_t writeByte( const char byte );

    virtual std::size_t writeBytes( const char * buffer,
                                    const std::size_t bufferSize );

    // END Public methods. ///////////////////////////////////////////

    // BEGIN Static methods. /////////////////////////////////////////
    // END Static methods. ///////////////////////////////////////////

};

#endif /* POSIX_TCP_SOCKET_WRITER_H_ */
