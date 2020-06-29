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
#include <interfaces/IVolumeControl.h>
#include <interfaces/JVolumeControl.h>

namespace WPEFramework {
namespace Plugin {

    class VolumeControl : public PluginHost::IPlugin, public Exchange::IVolumeControl, PluginHost::JSONRPC {
    private:
        static constexpr uint8_t MUTED = 0x80;

    public:
        VolumeControl(const VolumeControl&) = delete;
        VolumeControl& operator=(const VolumeControl&) = delete;

        VolumeControl()
            : _adminLock()
            , _notifications()
            , _volume(100)
        {
        }

        ~VolumeControl() override
        {
        }

        BEGIN_INTERFACE_MAP(VolumeControl)
       	    INTERFACE_ENTRY(PluginHost::IPlugin)
       	    INTERFACE_ENTRY(PluginHost::IDispatcher)
       	    INTERFACE_ENTRY(Exchange::IVolumeControl)
        END_INTERFACE_MAP

        // IPlugin methods
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

        // IVolumeControl methods
        void Register(Exchange::IVolumeControl::INotification* observer) override;
        void Unregister(const Exchange::IVolumeControl::INotification* observer) override;
        uint32_t Muted(const bool muted) override;
        uint32_t Muted(bool& muted) const override;
        uint32_t Volume(const uint8_t volume) override;
        uint32_t Volume(uint8_t& volume) const override;

    private:
        Core::CriticalSection _adminLock;
        std::vector<Exchange::IVolumeControl::INotification*> _notifications;

        // _volume is the current setting (0-100%). Highest bit indicates muted.
        uint8_t _volume; 
    };

} // namespace Plugin
} // namespace WPEFramework
