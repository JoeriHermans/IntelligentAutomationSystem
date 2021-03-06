/**
 * A class which describes the properties and actions of an SSL socket.
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
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <poll.h>
#include <netdb.h>
#include <errno.h>

// Application dependencies.
#include <ias/io/reader/network/ssl/ssl_reader.h>
#include <ias/io/writer/network/ssl/ssl_writer.h>
#include <ias/network/posix/ssl/posix_ssl_socket.h>
#include <ias/network/util.h>

// END Includes. /////////////////////////////////////////////////////

inline void PosixSslSocket::initialize( void ) {
    mSsl = nullptr;
    mReader = nullptr;
    mWriter = nullptr;
    mSslContext = nullptr;
}

void PosixSslSocket::setSslEnvironment( SSL * ssl ) {
    // Checking the precondition.
    assert( ssl != nullptr );

    if( mSsl != nullptr )
        SSL_free(mSsl);
    mSsl = ssl;
    delete mReader; mReader = nullptr;
    delete mWriter; mWriter = nullptr;
    mReader = new SslReader(this,ssl);
    mWriter = new SslWriter(this,ssl);
}

void PosixSslSocket::setSslContext( SSL_CTX * sslContext ) {
    // Checking the precondition.
    assert( sslContext != nullptr );

    mSslContext = sslContext;
}

bool PosixSslSocket::initializeConnection( const std::string & address,
                                           const std::size_t port ) {
    bool connected;
    SSL * ssl;
    int fd;

    delete mReader; mReader = nullptr;
    delete mWriter; mWriter = nullptr;
    connected = false;
    fd = connect(address,port);
    if( fd >= 0 ) {
        connected = true;
        ssl = SSL_new(mSslContext);
        SSL_set_fd(ssl,fd);
        if( SSL_connect(ssl) <= 0 ) {
            SSL_free(mSsl);
            mSsl = nullptr;
            SSL_free(ssl);
            close(fd);
        } else {
            connected = true;
            setSslEnvironment(ssl);
        }
    }

    return ( connected );
}

void PosixSslSocket::pollSocket( void ) const {
    struct pollfd pfd;
    int fd;

    if( mSsl != nullptr ) {
        fd = SSL_get_fd(mSsl);
        if( fd >= 0 ) {
            pfd.fd = fd;
            #if defined(__linux__)
            pfd.events = POLLNVAL | POLLHUP | POLLRDHUP;
            #else
            pfd.events = POLLNVAL | POLLHUP;
            #endif
            pfd.revents = 0;
            if( poll(&pfd,1,0) >= 1 ) {
                shutdownConnection();
            }
        }
    }
}

void PosixSslSocket::shutdownConnection( void ) const {
    int fd;

    fd = SSL_get_fd(mSsl);
    SSL_shutdown(mSsl);
    if( fd >= 0 )
        close(fd);
    SSL_set_fd(mSsl,-1);
}

PosixSslSocket::PosixSslSocket( SSL_CTX * sslContext ) {
    initialize();
    setSslContext(sslContext);
}

PosixSslSocket::PosixSslSocket( SSL * ssl ) {
    initialize();
    setSslEnvironment(ssl);
}

PosixSslSocket::~PosixSslSocket( void ) {
    closeConnection();
    if( mSsl != nullptr )
        SSL_free(mSsl);
    mSsl = nullptr;
    delete mReader; mReader = nullptr;
    delete mWriter; mWriter = nullptr;
}

void PosixSslSocket::closeConnection( void ) {
    if( isConnected() ) {
        shutdownConnection();
    }
}

bool PosixSslSocket::createConnection( const std::string & address,
                                       const std::size_t port ) {
    // Checking the precondition.
    assert( mSslContext != nullptr );

    return ( initializeConnection(address,port) );
}

bool PosixSslSocket::isConnected( void ) const {
    pollSocket();

    return ( mSsl != nullptr && SSL_get_fd(mSsl) >= 0 );
}

Reader * PosixSslSocket::getReader( void ) const {
    return ( mReader );
}

Writer * PosixSslSocket::getWriter( void ) const {
    return ( mWriter );
}

void PosixSslSocket::setSendTimeout( const struct timeval & tv ) {
    if( mSsl != nullptr ) {
        int fd;

        fd = SSL_get_fd(mSsl);
        setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof tv);
    }
}

void PosixSslSocket::setReceiveTimeout( const struct timeval & tv ) {
    if( mSsl != nullptr ) {
        int fd;

        fd = SSL_get_fd(mSsl);
        setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof tv);
    }
}
