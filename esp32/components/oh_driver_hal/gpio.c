/*
 * Copyright (c) 2022 OpenValley Digital Co., Ltd.
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

/*
    This example shows how to scan for available set of APs.
*/
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO 4
#define LED_OFF     0x00
#define LED_ON      0x01

static const char *TAG = "HDF_gpio";
#define S_LOG(tag,fmt,...) esp_rom_printf("\e[1;32m%s:" fmt "\e[0m\n",tag,##__VA_ARGS__)

int32_t InitGpioDevice(int pin)
{
    gpio_reset_pin(pin);
    return ESP_OK;
}

/* dev api */
int32_t GpioDevWrite(int pin, int val)
{
    if(pin >=GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    gpio_set_level(pin, val);
    return ESP_OK;
}

int32_t GpioDevRead(int pin, unsigned int *val)
{
    uint16_t value;
    if(pin >=GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    value = (uint16_t)gpio_get_level(pin);
    *val = value;
    return ESP_OK;
}

int32_t GpioDevSetDir(int pin, int dir)
{
    if(pin >=GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    gpio_set_direction(pin, dir +1);
    return ESP_OK;
}

int32_t GpioDevGetDir(int pin)
{
    uint16_t value;
    if(pin >=GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    value = (uint16_t)gpio_get_level(pin);
    return value;
}

int32_t GpioDevSetIrq(int pin, int mode, int pin_bit_mask)
{
    esp_err_t ret;
    if(pin >=GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = mode + 1;//匹配乐鑫参数
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = pin_bit_mask + 1;
    //0 is disable pull-down mode
    io_conf.pull_down_en = 0;
    //0 is disable pull-up mode
    io_conf.pull_up_en = 0;
    ret = gpio_config(&io_conf);
    if(ret == -1){
        esp_rom_printf("GpioDevSetIrq failed!\n");
    }
    return ESP_OK;
}

int32_t GpioDevUnSetIrq(int pin)
{
    esp_err_t ret;
    if(pin >= GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    ret = gpio_config(&io_conf);
    if(ret == -1){
        esp_rom_printf("GpioDevUnSetIrq failed!\n");
    }
    return ESP_OK;
}

int32_t GpioDevEnableIrq(int pin, int int_type, gpio_isr_t isr_handler, void *args)
{
    esp_err_t ret;
    if(pin >= GPIO_NUM_MAX) {
        esp_rom_printf("%s,error pin:%d",TAG,pin);
        return -2;
    }
    //change gpio intrrupt type for one pin
    gpio_set_intr_type(pin, int_type);
    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    ret = gpio_isr_handler_add(pin, isr_handler, (void*) args);
    if(ret == -1){
         esp_rom_printf("GpioDevEnableIrq failed!\n");
    }
    return ESP_OK;
}

int32_t GpioDevDisableIrq(int pin)
{
    return gpio_isr_handler_remove(pin);
}