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

#include "DIALServer.h"
#include "interfaces/IAmazonPrime.h"

namespace WPEFramework {
namespace DIALHandlers {

    class AmazonPrime : public Plugin::DIALServer::Default {
    private:
        AmazonPrime() = delete;
        AmazonPrime(const AmazonPrime&) = delete;
        AmazonPrime& operator=(const AmazonPrime&) = delete;

    public:
        AmazonPrime(PluginHost::IShell* service, const Plugin::DIALServer::Config::App& config, Plugin::DIALServer* parent)
            : Default(service, config, parent)
            , _prime(nullptr)
            , _service(nullptr)
            , _hidden(false)
            , _notification(*this)
            , _lock()
            , _callsign(config.Callsign.Value())
        {
            ASSERT(service != nullptr);
            ASSERT(parent != nullptr);
            service->Register(&_notification);
        }

        ~AmazonPrime() override
        {
            Stopped({}, {});
            _service->Unregister(&_notification);
        }

    public:
        bool Connect() override
        {
            Attach();
            return _prime != nullptr;
        }
        bool IsConnected() override
        {
            return _prime != nullptr;
        }
        virtual void Stopped(const string& data, const string& payload)
        {
            Detach();
        }
        bool HasHideAndShow() const override
        {
            return true;
        }
        uint32_t Show() override
        {
            _lock.Lock();
            if (_stateControl != nullptr){
                _stateControl->Request(PluginHost::IStateControl::RESUME);
            }
            _lock.Unlock();
            return Core::ERROR_NONE;
        }
        void Hide() override
        {
            _lock.Lock();
            if (_stateControl != nullptr){
                _stateControl->Request(PluginHost::IStateControl::SUSPEND);
            }
            _lock.Unlock();
        }
        bool IsHidden() const override
        {
            return _hidden;
        }

    private:
        const string& Callsign() const
        {
            return (_callsign);
        }
        void Attach()
        {
            _lock.Lock();
            if (_prime == nullptr) {
                _prime = Plugin::DIALServer::Default::QueryInterface<Exchange::IAmazonPrime>();

                if (_prime != nullptr) {
                    _stateControl = _prime->QueryInterface<PluginHost::IStateControl>();
                }
            }
            _lock.Unlock();
        }

        void Detach()
        {
            _lock.Lock();
            _hidden = false;

            if (_stateControl != nullptr) {
                _stateControl->Release();
                _stateControl = nullptr;
            }

            if (_prime != nullptr) {
                _prime->Release();
                _prime = nullptr;
            }

            _lock.Unlock();
        }

        void StateChange(const PluginHost::IStateControl::state state)
        {
            _hidden = (state == PluginHost::IStateControl::SUSPENDED);
        }

    private:
        class Notification : public PluginHost::IPlugin::INotification,
                             public PluginHost::IStateControl::INotification {

        public:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
            explicit Notification(AmazonPrime& parent)
                : _parent(parent)
            {
            }
            ~Notification() = default;

        public:
            void StateChange(PluginHost::IShell* shell) override
            {
                ASSERT(shell != nullptr);
                if (shell->Callsign() == _parent.Callsign()) {
                    if (shell->State() == PluginHost::IShell::ACTIVATED) {
                        _parent.Attach();
                    } else if (shell->State() == PluginHost::IShell::DEACTIVATED) {
                        _parent.Detach();
                    }
                }
            }

            virtual void StateChange(const PluginHost::IStateControl::state value)
            {
                _parent.StateChange(value);
            }

        public:
            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
            INTERFACE_ENTRY(PluginHost::IStateControl::INotification)
            END_INTERFACE_MAP

        private:
            AmazonPrime& _parent;
        }; // class Notification

        Exchange::IAmazonPrime* _prime;
        PluginHost::IShell* _service;
        PluginHost::IStateControl* _stateControl;
        bool _hidden;
        Core::Sink<Notification> _notification;
        mutable Core::CriticalSection _lock;
        string _callsign;

    }; // class AmazonPrime

    static Plugin::DIALServer::ApplicationRegistrationType<AmazonPrime> _amazonHandler;
}
}
