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

#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "los_memory.h"
#include "los_task.h"

#define OLED_POWER_IO 26
#define OLED_I2C_SDA_IO 9
#define OLED_I2C_SCL_IO 4
#define OLED_I2C_DEV_NUM 0
#define OLED_I2C_ADDR 0x78


static const char *TAG = "oled";
#define S_LOG(tag,fmt,...) esp_rom_printf("\e[1;32m%s:" fmt "\e[0m\n",tag,##__VA_ARGS__)

unsigned int DeviceI2cWrite(int id,unsigned char addr,const unsigned char *data, unsigned int dataLen,int ack)
{
    unsigned int ret;
    i2c_cmd_handle_t cmd;
	if (dataLen < 1)return 0;
	cmd = i2c_cmd_link_create();
    if(( ret = i2c_master_start(cmd) ) != 0)
    {
        printf("DeviceI2cWrite i2c_master_start failed ret = %x", ret);
        return ret;
    }
    if(( ret = i2c_master_write_byte(cmd, addr | 0x00, ack) ) != 0)
    {
        printf("DeviceI2cWrite i2c_master_write_addr failed ret = %x", ret);
        return ret;
    }

    if(( ret = i2c_master_write(cmd, data, dataLen, 1) ) != 0)
    {
        printf("DeviceI2cWrite i2c_master_write failed ret = %x", ret);
        return ret;
    }
	//for(unsigned i = 0;i < dataLen;++i) {
	//	if(( ret = i2c_master_write_byte(cmd, data[i], ack) ) != 0)
    //    {
    //        printf("DeviceI2cWrite i2c_master_write_data failed ret = %x", ret);
    //        return ret;
    //    }
	//}
    if(( ret = i2c_master_stop(cmd) ) != 0)
    {
        printf("DeviceI2cWrite i2c_master_stop failed ret = %x", ret);
        return ret;
    }
    if(( ret = i2c_master_cmd_begin(id, cmd, 1000 / portTICK_RATE_MS) ) != 0)
    {
        printf("DeviceI2cWrite i2c_master_cmd_begin failed ret = %x", ret);
        return ret;
    }
    i2c_cmd_link_delete(cmd);
    return ret;
}

unsigned int DeviceI2cRead(int id,unsigned char addr,const unsigned char *data, unsigned int dataLen,int ack)
{
    unsigned int ret;
    i2c_cmd_handle_t cmd;
	if (dataLen < 1)return 0;
	cmd = i2c_cmd_link_create();
    if(( ret = i2c_master_start(cmd) ) != 0)
    {
        printf("DeviceI2cRead i2c_master_start failed ret = %x", ret);
        return ret;
    }
    if(( ret = i2c_master_write_byte(cmd, addr | 0x01, ack) ) != 0)
    {
        printf("DeviceI2cRead i2c_master_write_addr failed ret = %x", ret);
        return ret;
    }
	//for(unsigned i = 0;i < dataLen;++i) {
	//	if(( ret = i2c_master_read_byte(cmd, data[i], ack) ) != 0)
    //    {
    //        printf("DeviceI2cRead i2c_master_read_byte failed ret = %x", ret);
    //        return ret;
    //    }
	//}

    if (dataLen > 1) {
        if(( ret = i2c_master_read(cmd, data, dataLen - 1, 0) ) != 0)
        {
            printf("DeviceI2cRead i2c_master_read failed ret = %x", ret);
            return ret;
        }
    }
    if(( ret = i2c_master_read_byte(cmd, data + dataLen - 1, 1) ) != 0)
    {
        printf("DeviceI2cRead i2c_master_read_byte failed ret = %x", ret);
        return ret;
    }

    if(( ret = i2c_master_stop(cmd) ) != 0)
    {
        printf("DeviceI2cRead i2c_master_stop failed ret = %x", ret);
        return ret;
    }
    if(( ret = i2c_master_cmd_begin(id, cmd, 1000 / portTICK_RATE_MS) ) != 0)
    {
        printf("DeviceI2cRead i2c_master_cmd_begin failed ret = %x", ret);
        return ret;
    }
    i2c_cmd_link_delete(cmd);
    return ret;
}
int DeviceI2cInit(int cmd,int id,int scl_pin,int sda_pin,int speed,int mode)
{
    if(0 == cmd) {
        i2c_config_t conf;
		memset(&conf,0,sizeof(conf));
        conf.mode = (mode & 1) ? I2C_MODE_SLAVE : I2C_MODE_MASTER;
        conf.sda_io_num = sda_pin;
        conf.scl_io_num = scl_pin;
        conf.sda_pullup_en = (mode & 2) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
        conf.scl_pullup_en = (mode & 4) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
        conf.master.clk_speed = speed;
        i2c_param_config(id, &conf);
    
        i2c_driver_install(id, conf.mode, 0, 0, 0);
		return 0;
    }
	if(-1 == cmd){
		return 0;
	}
	return -1;
}
void DeviceGpioInit(int pin,int mode,int val)
{
	gpio_config_t gpio;
	gpio.pin_bit_mask = (1<<pin);
	gpio.mode = (mode & 1) ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT;
	if(mode & 2) gpio.mode |= GPIO_MODE_DEF_OD;
	gpio.pull_up_en   = (mode & 0x10) ? GPIO_PULLUP_ENABLE   : GPIO_PULLUP_DISABLE;
	gpio.pull_down_en = (mode & 0x20) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
	gpio.intr_type    = (mode & 0xF00) >> 8; // GPIO_INTR_DISABLE
	gpio_config(&gpio);
	gpio_set_level(pin, val);
}
#if 0
unsigned int OLED_I2CWrite(const unsigned char *data, unsigned int dataLen)
{
    i2c_cmd_handle_t cmd;
	if (dataLen < 1)return 0;
	cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, OLED_I2C_ADDR | 0x00, 1);
	for(unsigned i = 0;i < dataLen;++i) {
		i2c_master_write_byte(cmd, data[i], 1);
	}
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(OLED_I2C_DEV_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}
int DeviceOLEDInit(int cmd)
{
    {
		// 开启oled电源
        gpio_config_t gpio;
        gpio.pin_bit_mask = (1<<OLED_POWER_IO);
        gpio.mode = GPIO_MODE_OUTPUT;
        gpio.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio.pull_down_en = GPIO_PULLDOWN_DISABLE;
        gpio.intr_type = GPIO_INTR_DISABLE;
        gpio_config(&gpio);
        gpio_set_level(OLED_POWER_IO, 1);
    }
    {
		// 初始化I2C
        i2c_config_t conf;
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = OLED_I2C_SDA_IO;
        conf.scl_io_num = OLED_I2C_SCL_IO;
        conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
        conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
        conf.master.clk_speed = 400*1000;
        i2c_param_config(OLED_I2C_DEV_NUM, &conf);
    
        i2c_driver_install(OLED_I2C_DEV_NUM, I2C_MODE_MASTER, 0, 0, 0);
    }
	LOS_TaskDelay(1);
	return 0;
}
#endif

	