// Copyright 2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>

#include "mbedtls/entropy_poll.h"

#ifndef MBEDTLS_ENTROPY_HARDWARE_ALT
#error "MBEDTLS_ENTROPY_HARDWARE_ALT should always be set in ESP-IDF"
#endif

int mbedtls_hardware_poll( void *data,
                           unsigned char *output, size_t len, size_t *olen )
{
    esp_fill_random(output, len);
    *olen = len;
    return 0;
}
