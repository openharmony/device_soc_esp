/*
 * Copyright (c) 2022 Hunan OpenValley Digital Industry Development Co., Ltd.
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
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "hal/dac_types.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling
#define s_Delay(x) vTaskDelay(x / portTICK_RATE_MS)

static esp_adc_cal_characteristics_t *adc_chars;

// static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_11db;//ADC_ATTEN_DB_0;
adc_unit_t unit = ADC_UNIT_1;

void check_efuse(void)
{
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        esp_rom_printf("eFuse Two Point: Supported\n");
    } else {
        esp_rom_printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        esp_rom_printf("eFuse Vref: Supported\n");
    } else {
        esp_rom_printf("eFuse Vref: NOT supported\n");
    }
}


void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        esp_rom_printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        esp_rom_printf("Characterized using eFuse Vref\n");
    } else {
        esp_rom_printf("Characterized using Default Vref\n");
    }
}

int32_t AdcDevOpen(void *handle, uint32_t number)
{
   //Check if Two Point or Vref are burned into eFuse
    check_efuse();
    // esp_rom_printf("AdcDevOpen number: %d\n",number);
    if(number == 2){
        unit = ADC_UNIT_2;
    }
    else{
        unit = ADC_UNIT_1;
    }
   adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
   esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);//在特定衰减下表征ADC>的特性
    return ESP_OK;
}

void AdcDevClose(void *handle)
{
    //no code
}

int32_t AdcDevRead(void *handle, uint32_t number, uint32_t channel, uint32_t *val)
{
    // check_efuse();
    // esp_rom_printf("AdcDevRead number: %d,channel: %d\n",number,channel);
    if(number == 2){
        unit = ADC_UNIT_2;
    }
    else{
        unit = ADC_UNIT_1;
    }
    // esp_rom_printf("AdcDevRead Unit: %d,channel: %d\n",unit,channel);
    //Configure ADC
    if (unit == ADC_UNIT_1) {//adc1/2选择配置
        adc1_config_width(width);;//采集宽度
        adc1_config_channel_atten(channel, atten);//配置通道以及衰减度
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
   // adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
   // esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);//在特定衰减下表征ADC的特性
    //print_char_val_type(val_type);
    //Continuously sample ADC1
    uint32_t adc_reading = 0;
    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        if (unit == ADC_UNIT_1) {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);//从单个通道获取ADC1读数
        } else {
            int raw;
            adc2_get_raw((adc2_channel_t)channel, width, &raw);
            adc_reading += raw;
        }
    }
    adc_reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);//将ADC读数转换为以mV为单位的电压
    if(voltage != 0){
        *val = voltage;
    }
    // esp_rom_printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
    return ESP_OK;
}
