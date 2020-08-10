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

#include "Dolby.h"

#include <nexus_config.h>
#include <nexus_platform.h>
#include <nxclient.h>
#include <nexus_core_utils.h>
/* #include "nexus_hdmi_output_dba.h" */

using namespace WPEFramework::Exchange;

uint32_t
set_audio_output_type(const Dolby::IOutput::Type type)
{
    uint32_t result = WPEFramework::Core::ERROR_GENERAL;
    TRACE_L1("NOT IMPLEMENTED");
    return result;
}

Dolby::IOutput::Type toEnum(int code, uint32_t &error)
{
    Dolby::IOutput::Type result;
    switch (code)
    {
    case Dolby::IOutput::Type::AUTO:
    {
        result = Dolby::IOutput::Type::AUTO;
        error = WPEFramework::Core::ERROR_NONE;
        break;
    }
    case Dolby::IOutput::Type::ATMOS_PASS_THROUGH:
    {
        result = Dolby::IOutput::Type::ATMOS_PASS_THROUGH;
        error = WPEFramework::Core::ERROR_NONE;
    }
    case Dolby::IOutput::Type::DIGITAL_PASS_THROUGH:
    {
        result = Dolby::IOutput::Type::DIGITAL_PASS_THROUGH;
        error = WPEFramework::Core::ERROR_NONE;
    }
    case Dolby::IOutput::Type::DIGITAL_PCM:
    {
        result = Dolby::IOutput::Type::DIGITAL_PCM;
        error = WPEFramework::Core::ERROR_NONE;
    }
    default:
    {
        result = Dolby::IOutput::Type::AUTO;
        error = WPEFramework::Core::ERROR_GENERAL;
        TRACE_L1("Could not map the provided dolby output: %d to Dolby::IOutput::Type enumeration.");
    }
    }

    return result;
}

WPEFramework::Exchange::Dolby::IOutput::Type
get_audio_output_type(uint32_t &error)
{
    Dolby::IOutput::Type result = Dolby::IOutput::Type::AUTO;
    TRACE_L1("NOT IMPLEMENTED");
    error = WPEFramework::Core::ERROR_GENERAL;

    return result;
}

bool get_atmos_lock(uint32_t& error)
{
    fprintf(stderr, "***AG-PRINT*[%s:%d %s]  \n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    bool result = false;

    NxClient_AudioProcessingSettings apSettings;
    NxClient_GetAudioProcessingSettings(&apSettings);

    result = apSettings.dolby.ddre.fixedAtmosOutput;

    return result;
}

uint32_t set_atmos_lock(bool enable)
{
    uint32_t status = WPEFramework::Core::ERROR_NONE;

    fprintf(stderr, "***AG-PRINT*[%s:%d %s]  \n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    NxClient_AudioProcessingSettings apSettings;
    NxClient_GetAudioProcessingSettings(&apSettings);

    apSettings.dolby.ddre.fixedAtmosOutput = enable;

    NEXUS_Error rc = NxClient_SetAudioProcessingSettings(&apSettings);
    if(rc != NEXUS_SUCCESS) {
        TRACE_L1("NxClient_SetAudioProcessingSettings failed");
        status = WPEFramework::Core::ERROR_GENERAL;
    }

    return status;
}
