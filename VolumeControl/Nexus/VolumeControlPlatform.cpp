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
 
#include "../VolumeControlPlatform.h"

#include <nexus_config.h>
#include <nexus_audio_types.h>
#include <nexus_platform.h>
#include <nexus_simple_audio_decoder.h>
#include <nxclient.h>

namespace WPEFramework {
namespace Plugin {

namespace {

class Nexus {
public:
    Nexus() {
        NxClient_JoinSettings joinSettings;
        NxClient_GetDefaultJoinSettings(&joinSettings);
        snprintf(joinSettings.name, NXCLIENT_MAX_NAME, "%s", "wpevolumecontrol");
        NxClient_Join(&joinSettings);
        NxClient_UnregisterAcknowledgeStandby(NxClient_RegisterAcknowledgeStandby());
    }
    ~Nexus() override
    {
        NxClient_Uninit();
    }
};

static Nexus _initialization;

}  // nameless namespace

constexpr uint8_t kMinVolume = 0;
constexpr uint8_t kMaxVolume = 100;

static int32_t ToNexusLinear(const uint8_t vol)
{
    return std::round(static_cast<double>((vol * (NEXUS_AUDIO_VOLUME_LINEAR_NORMAL - NEXUS_AUDIO_VOLUME_LINEAR_MIN)) /
          (kMaxVolume - kMinVolume)) + NEXUS_AUDIO_VOLUME_LINEAR_MIN);
}

static uint8_t FromNexusLinear(const int32_t vol)
{
    return  std::round(static_cast<double>((vol - NEXUS_AUDIO_VOLUME_LINEAR_MIN) * (kMaxVolume - kMinVolume)) /
          (NEXUS_AUDIO_VOLUME_LINEAR_NORMAL - NEXUS_AUDIO_VOLUME_LINEAR_MIN));
}

static int32_t ToNexusDb(const uint8_t vol)
{
    int32_t result = 0;
    auto gain = 2000.0 * log10(static_cast<double>(vol) / (kMaxVolume - kMinVolume));
    if (std::isinf(gain)) {
        result = NEXUS_AUDIO_VOLUME_DB_MIN;
    } else if (gain == 0) {
        result = NEXUS_AUDIO_VOLUME_DB_NORMAL;
    } else {
        result = NEXUS_AUDIO_VOLUME_DB_NORMAL + std::floor(gain);
    }

    return std::max(NEXUS_AUDIO_VOLUME_DB_MIN, result);
}

static uint8_t FromNexusDb(const int32_t vol)
{
    auto gain = NEXUS_AUDIO_VOLUME_DB_NORMAL - vol;
    auto factor = std::pow(10, (static_cast<double>(gain) / 2000.0));
    return std::round((kMaxVolume - kMinVolume) / factor);
}

uint32_t platform_set_volume(uint8_t volume)
{
    volume = std::max(volume, kMinVolume);
    volume = std::min(volume, kMaxVolume);
    int32_t toSet = volume;
    NxClient_AudioSettings audioSettings;
    NxClient_GetAudioSettings(&audioSettings);
    if (audioSettings.volumeType == NEXUS_AudioVolumeType_eDecibel) {
          toSet = VolumeControlPlatformNexus::ToNexusDb(volume);
    } else {
          toSet = VolumeControlPlatformNexus::ToNexusLinear(volume);
    }
    audioSettings.muted = (volume == 0);

    audioSettings.leftVolume = audioSettings.rightVolume = toSet;
    return NxClient_SetAudioSettings(&audioSettings) != 0 ?
           Core::ERROR_GENERAL :
           Core::ERROR_NONE;
}

uint32_t platform_get_volume(uint8_t* volume)
{
    NxClient_AudioSettings audioSettings;
    NxClient_GetAudioSettings(&audioSettings);
    ASSERT(audioSettings.leftVolume == audioSettings.rightVolume);
    *volume = audioSettings.volumeType == NEXUS_AudioVolumeType_eDecibel ?
          VolumeControlPlatformNexus::FromNexusDb(audioSettings.leftVolume) :
          VolumeControlPlatformNexus::FromNexusLinear(audioSettings.leftVolume);

    return (Core::ERROR_NONE); 
}

