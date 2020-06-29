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
 
#include "VolumeControl.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(VolumeControl, 1, 0);

    const string VolumeControl::Initialize(PluginHost::IShell* service)
    {
        Exchange::JVolumeControl::Register(*this, _implementation);
        platform_get_volume(_volume);

        return (EMPTY_STRING);
    }

    void VolumeControl::Deinitialize(PluginHost::IShell* service)
    {
        Exchange::JVolumeControl::Unregister(*this);
    }

    string VolumeControl::Information() const
    {
        return EMPTY_STRING;
    }

    void VolumeControl::Register(Exchange::IVolumeControl::INotification* notification) /* override */
    {
        ASSERT(notification != nullptr);
        ASSERT(std::find(_notifications.begin(), _notifications.end(), notification) == _notification.end());
        _adminLock.Lock();
        notification->AddRef();
        _notifications.push_back(notification);
        _adminLock.Unlock();
    }

    void VolumeControl::Unregister(const Exchange::IVolumeControl::INotification* notification)
    {
        ASSERT(notification != nullptr);
        _adminLock.Lock();
        auto item = std::find(_notifications.begin(), _notifications.end(), notification);
        ASSERT(item != _notifications.end());
        _notifications.erase(item);
        notification->Release();
        _adminLock.Unlock();
    }


    uint32_t VolumeControl::Muted(const bool muted)
    {
        uint32_t result = Core::ERROR_NONE;

        _adminLock.Lock();

        if (((_volume & MUTED) != 0) ^ (muted == true)) {
            result = platform_set_volume(muted ? 0 : (_volume & ~MUTED));
            if (result == Core::ERROR_NONE) {
                _volume = (muted ? (MUTED | _volume) : (_volume & ~MUTED));
                for (auto* notification : _notifications) {
                    notification->Muted(muted);
                }
            }
        }

        _adminLock.Unlock();

        return (result);
    }

    uint32_t VolumeControl::Muted(bool& muted) const
    {
        muted = ((_volume & MUTED) != 0);
        return Core::ERROR_NONE;
    }

    uint32_t VolumeControl::Volume(const uint8_t volume)
    {
        uint32_t result = (volume <= 100 ? Core::ERROR_NONE : Core::ERROR_BAD_REQUEST);

        _adminLock.Lock();

        if ((volume < 100) && ((_volume & ~MUTED) != volume)) {
            result = platform_set_volume(volume);
            if (result == Core::ERROR_NONE) {
                _volume = (volume | (_volume & MUTED));
                for (auto* notification : _notifications) {
                    notification->Volume(volume);
                }
            }
        }
        
        return (result);
    }

    uint32_t VolumeControl::Volume(uint8_t& vol) const
    {
        vol = (_volume & ~MUTED);
        return Core::ERROR_NONE;
    }

} // namespace Plugin
} // namespace WPEFramework
