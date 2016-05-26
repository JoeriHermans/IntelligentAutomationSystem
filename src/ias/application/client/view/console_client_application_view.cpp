/**
 * A class which describes the properties and actions of a console
 * or terminal IAS client application.
 *
 * @date                    25 05 2016
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

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <cassert>
#include <iostream>
#include <sstream>
#include <termios.h>
#include <unistd.h>

// Application dependencies.
#include <ias/application/constants.h>
#include <ias/application/client/view/console_client_application_view.h>
#include <ias/util/observer.h>
#include <ias/util/view.h>
#include <ias/util/util.h>

// END Includes. /////////////////////////////////////////////////////

namespace ias {

    // BEGIN Constants. //////////////////////////////////////////////

    const char ias::console_client_application_view::kLabelPassword[] = "Password: ";
    const char ias::console_client_application_view::kLabelUsername[] = "Username: ";
    const char ias::console_client_application_view::kMessageConnected[] = "Connected to the server.";
    const char ias::console_client_application_view::kMessageConnecting[] = "Connecting...";
    const char ias::console_client_application_view::kMessageDisconnected[] = "Disconnected.";
    const char ias::console_client_application_view::kMessageDisconnecting[] = "Disconnecting...";
    const char ias::console_client_application_view::kMessageLoggedIn[] = "Authentication successfully.";
    const char ias::console_client_application_view::kMessageLoggingIn[] = "Authenticating with server...";
    const char ias::console_client_application_view::kMessageLoginFailed[] = "Authentication failed.";
    const char ias::console_client_application_view::kMessageNoCredentials[] = "Please specify a username and a password.";
    const char ias::console_client_application_view::kMessageNotConnected[] = "Could not connect to the server.";
    const char ias::console_client_application_view::kMessageShell[] = "IAS > ";
    const char ias::console_client_application_view::kMessageSpecifyCredentials[] = "Please specify your user credentials.";

    // END Constants. ////////////////////////////////////////////////

    inline void console_client_application_view::initialize(void) {
        mModel = nullptr;
        mHost = kDefaultServerAddress;
        mPort = kDefaultUserServerPort;
        mSocksPort = 0;
    }

    inline void console_client_application_view::set_model(ias::client_application_model * model) {
        // Checking the precondition.
        assert(model != nullptr);

        mModel = model;
    }

    inline void console_client_application_view::stopping(void) {
        print_message(kMessageDisconnecting);
        mModel->close_connection();
        print_message(kMessageDisconnected);
    }

    void console_client_application_view::print(const std::string & message) const {
        std::cout << message << std::flush;
    }

    void console_client_application_view::print_message(const std::string & message) const {
        std::cout << "> " << message << std::endl << std::flush;
    }

    void console_client_application_view::disable_terminal_echo(void) const {
        struct termios tty;

        tcgetattr(STDIN_FILENO, &tty);
        tty.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    }

    void console_client_application_view::enable_terminal_echo(void) const {
        struct termios tty;

        tcgetattr(STDIN_FILENO, &tty);
        tty.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    }

    void console_client_application_view::execute_commands(void) {
        std::string command;

        // Checking the precondition.
        assert(mModel->is_connected());

        print(kMessageShell);
        while(mModel->is_connected()) {
            std::getline(std::cin, command);
            if(mModel->is_connected() && !command.empty()) {
                if(command != ias::client_application_model::kCommandQuit)
                    mModel->execute(command);
                else
                    break;
            }
        }
    }

    void console_client_application_view::login(void) {
        std::string username;
        std::string password;

        // Checking the precondition.
        assert(mModel->is_connected());

        // Retry until user specifies decent credentials.
        while(username.empty() || password.empty()) {
            print_message(kMessageSpecifyCredentials);
            print(kLabelUsername);
            // Retrieve the username from stdin.
            std::cin >> username;
            print(kLabelPassword);
            disable_terminal_echo();
            // Retrieve the password from stdin.
            std::cin >> password;
            enable_terminal_echo();
            std::cout << std::endl << std::flush;
            // Check if the specified credentials are empty.
            if(username.empty() || password.empty())
                print_message(kMessageNoCredentials);
        }
        print_message(kMessageLoggingIn);
        mModel->authorize(username, password);
    }

    void console_client_application_view::connect(void) {
        // Check if a SOCKS connection is requested.
        if(mSocksPort != 0)
            mModel->create_connection(mHost, mPort, mSocksHost, mSocksPort);
        else
            mModel->create_connection(mHost, mPort);
    }

    void console_client_application_view::set_host(const int argc, const char ** argv) {
        // Checking the precondition.
        assert(argc > 1 && argv != nullptr);

        int index = ias::flag_index(argc, argv, kFlagAddress);
        if(index != -1 && (index + 1) < argc)
            mHost = argv[index + 1];
    }

    void console_client_application_view::set_port(const int argc, const char ** argv) {
        // Checking the precondition.
        assert(argc > 1 && argv != nullptr);

        int index = ias::flag_index(argc, argv, kFlagPort);
        if(index != -1 && (index + 1) < argc)
            mPort = static_cast<std::size_t>(std::stoi(argv[index + 1]));
    }

    void console_client_application_view::set_ssl(const int argc, const char ** argv) {
        // Checking the precondition.
        assert(argc > 1 && argv != nullptr);

        int index = ias::flag_index(argc, argv, kFlagSsl);
        if(index != -1) {
            mModel->request_ssl(true);
            mPort = kDefaultUserServerPortSsl;
        }
    }

    void console_client_application_view::set_proxy(const int argc, const char ** argv) {
        // Checking the precondition.
        assert(argc > 1 && argv != nullptr);

        int index = ias::flag_index(argc, argv, kFlagSocks);
        if(index == -1 || index == argc)
            return;

        std::istringstream iss(argv[index + 1]);
        std::string address;
        std::string strPort;
        std::size_t port;

        std::getline(iss, address, ':');
        std::getline(iss, strPort);
        if(!address.empty() && !strPort.empty()) {
            port = static_cast<std::size_t>(std::stoi(strPort));
            // Check if a valid port is specified.
            if(port > 0) {
                mSocksHost = address;
                mSocksPort = port;
            }
        }
    }

    console_client_application_view::console_client_application_view(const int argc, const char ** argv,
                                                                     ias::client_application_model * model) {
        initialize();
        set_model(model);
        set_ssl(argc, argv);
        set_host(argc, argv);
        set_port(argc, argv);
        set_proxy(argc, argv);
    }

    void console_client_application_view::paint(void) {
        print_message(kMessageConnecting);
        connect();
        if(mModel->is_connected()) {
            print_message(kMessageConnected);
            login();
            if(mModel->is_logged_in()) {
                print_message(kMessageLoggedIn);
                execute_commands();
            }
            stopping();
        } else {
            print_message(kMessageNotConnected);
        }
    }

    void console_client_application_view::update(void) {
        // TODO Implement.
    }

    void console_client_application_view::update(void * argument) {
        const char * message = static_cast<const char *>(argument);

        std::cout << message << std::endl << std::endl << std::flush;
        std::cout << kMessageShell << std::flush;

    }

};