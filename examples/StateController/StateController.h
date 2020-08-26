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

#pragma once

#include "Module.h"
#include <interfaces/IWebDriver.h>

namespace WPEFramework {
namespace Plugin {

    class StateController
        : public PluginHost::IPlugin {

    public:
        StateController(const StateController&) = delete;
        StateController& operator=(const StateController&) = delete;

        #ifdef __WINDOWS__
        #pragma warning(disable : 4355)
        #endif
        StateController()
            : _adminLock()
            , _skipURL(0)
            , _service(nullptr)
            , _stateController(nullptr)
            , _connectionId(0)
        {
        }
        #ifdef __WINDOWS__
        #pragma warning(default : 4355)
        #endif

        ~StateController() override
        {
        }

    public:
        BEGIN_INTERFACE_MAP(StateController)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_AGGREGATE(Exchange::IWebDriver, _stateController)
        END_INTERFACE_MAP

    public:
        //  IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        // First time initialization. Whenever a plugin is loaded, it is offered a Service object with relevant
        // information and services for this particular plugin. The Service object contains configuration information that
        // can be used to initialize the plugin correctly. If Initialization succeeds, return nothing (empty string)
        // If there is an error, return a string describing the issue why the initialisation failed.
        // The Service object is *NOT* reference counted, lifetime ends if the plugin is deactivated.
        // The lifetime of the Service object is guaranteed till the deinitialize method is called.
        const string Initialize(PluginHost::IShell* service) override;

        // The plugin is unloaded from WPEFramework. This is call allows the module to notify clients
        // or to persist information if needed. After this call the plugin will unlink from the service path
        // and be deactivated. The Service object is the same as passed in during the Initialize.
        // After theis call, the lifetime of the Service object ends.
        void Deinitialize(PluginHost::IShell* service) override;

        // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
        // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
        string Information() const override;

    private:

        Core::CriticalSection _adminLock;
        uint32_t _skipURL;
        PluginHost::IShell* _service;
        Exchange::IWebDriver* _stateController;
        uint32_t _connectionId;
    };
} //namespace Plugin
} //namespace WPEFramework
