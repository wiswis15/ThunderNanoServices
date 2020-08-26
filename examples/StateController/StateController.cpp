/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "StateController.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(StateController, 1, 0);

    /* virtual */ const string StateController::Initialize(PluginHost::IShell* service)
    {
        string message;
        ASSERT(service != nullptr);
        _stateController = service->Root<Exchange::IWebDriver>(_connectionId, Core::infinite, _T("StateControllerImplementation"));

        if (_stateController == nullptr) {
            RPC::IRemoteConnection* connection(service->RemoteConnection(_connectionId));
            if (connection != nullptr) {
                connection->Terminate();
                connection->Release();
            }
            message = _T("StateControllerPlugin could not be instantiated.");

        } else {
            fprintf(stderr, "\n------->> [%d]\033[1m\033[31m Configuring StateController \033[0m<<-------\n", ::getpid());
            _stateController->Configure(service);
            fprintf(stderr, "\n------->> [%d]\033[1m\033[31m Configuring StateController - done \033[0m<<-------\n", ::getpid());
        }

        return message;
    }

    /* virtual */ void StateController::Deinitialize(PluginHost::IShell* service)
    {
        if (_stateController != nullptr) {
            if (_stateController->Release() != Core::ERROR_DESTRUCTION_SUCCEEDED) {

                ASSERT(_connectionId != 0);

                RPC::IRemoteConnection* connection(service->RemoteConnection(_connectionId));
                if (connection != nullptr) {
                    connection->Terminate();
                    connection->Release();
                }
            }
        }

        _service = nullptr;
    }

    /* virtual */ string StateController::Information() const
    {
        // No additional info to report.
        return (string());
    }

} //namespace Plugin
} // namespace WPEFramework
