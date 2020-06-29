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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function to request the current volume (0-100%)
 */
uint32_t platform_get_volume(uint8_t* volume);

/**
 * Function to set the current volume (0-100%)
 */
uint32_t platform_get_volume(const uint8_t volume);

#ifdef __cplusplus
}
#endif
