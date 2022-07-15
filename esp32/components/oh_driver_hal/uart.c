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
#include <stdlib.h>
#include "cmsis_os2.h"

#include "driver/uart.h"
#include "hal/gpio_types.h"
typedef struct {
	uint8_t port;
	uint8_t tx_io;
	uint8_t rx_io;
    uint8_t flow_ctrl;
    uint8_t rts_io;
    uint8_t cts_io;
}uartInfo_t;

static uartInfo_t *pubFunc(void *host)
{
	uartInfo_t *dev;
	int ret;
    if (host == NULL) {
        esp_rom_printf("UartHost is NULL\n");
        return ESP_FAIL;
    }
    dev = (uartInfo_t *)host;
    return dev;    
}

static int32_t uartClientInit(uint8_t port, uint8_t tx0, uint8_t rx0, uint8_t flow_ctrl, uint8_t rts_io, uint8_t cts_io)
{
	const int bufSize = 1024*2;
    int ret;
    uart_config_t uartConfig = {
    .baud_rate = 115200,                      /*!< UART baud rate*/
    .data_bits = UART_DATA_8_BITS,      /*!< UART byte size*/
    .parity = UART_PARITY_DISABLE,              /*!< UART parity mode*/
    .stop_bits = UART_STOP_BITS_1,        /*!< UART stop bits*/
    .flow_ctrl = flow_ctrl,   /*!< UART HW flow control mode (cts/rts)*/    
    };

    if (flow_ctrl == 3)
    {
        uartConfig.rx_flow_ctrl_thresh = 122;
    }

    uart_param_config(port, &uartConfig);

    ret = uart_set_pin(port, tx0, rx0, rts_io, cts_io);
    if (ret != 0)
    {
        esp_rom_printf("uart_set_pin failed\n");
        return ESP_FAIL;
    }

    ret = uart_driver_install(port, bufSize, bufSize, 0, NULL, 0);
    if (ret != 0)
    {
        esp_rom_printf("uart_driver_install failed\n");     
        return ESP_FAIL;
    }
    return ESP_OK;
}

#if 1
int32_t uartInit(void *host)
{
	uartInfo_t *dev;
	int ret;
    dev = pubFunc(host);  
	ret = uartClientInit(dev->port, dev->tx_io, dev->rx_io, dev->flow_ctrl, dev->rts_io, dev->cts_io);
	if (ret != 0)
	{
		esp_rom_printf("%s uartClientInit failed\n",__func__);
		return ESP_FAIL;
	}
	return ESP_OK;
}
#endif

int32_t uartDeinit(void *host)
{
	uartInfo_t *dev;
	int ret;
    dev = pubFunc(host);
    if ((ret = uart_driver_delete(dev->port)) != ESP_OK)
    {
        esp_rom_printf("%s uart_driver_delete failed\n",__func__);
		return ESP_FAIL;
    }
	return ESP_OK;
}

int32_t uartRead(void *host, uint8_t *data, uint32_t size)
{
    unsigned int len;
	uartInfo_t *dev;
	int ret;
    dev = pubFunc(host);
    ret = uart_get_buffered_data_len((uart_port_t)dev->port, &len);
    if (ret != 0)
    {
        esp_rom_printf("uart_get_buffered_data_len failed\n");
        return ESP_FAIL;
    } 
    if (size > len)
    {
	    ret = uart_read_bytes((uart_port_t)dev->port, (uint8_t *)data, len, 100);
        if (ret < 0)
        {
            esp_rom_printf("uart_read_bytes failed\n");
            return ESP_FAIL;
        } 
    }
    else
    {
	    ret = uart_read_bytes((uart_port_t)dev->port, (uint8_t *)data, size, 100);
        if (ret < 0)
        {
            esp_rom_printf("uart_read_bytes failed\n");
            return ESP_FAIL;
        }         
    }   

	return ret;
}

int32_t uartWrite(void *host, uint8_t *data, uint32_t size)
{
	uartInfo_t *dev;
	int ret;
    dev = pubFunc(host);   
	ret = uart_write_bytes((uart_port_t)dev->port, (const uint8_t *)data, size);
    if (ret < 0)
    {
        esp_rom_printf("uart_write_bytes failed\n");
        return ESP_FAIL;
    } 
	return ret;    
}

int32_t uartGetBaud(void *host, uint32_t *baudRate)
{
    uartInfo_t *dev;
	int ret;
    dev = pubFunc(host); 
    if((ret = uart_get_baudrate((uart_port_t)dev->port, baudRate)) != ESP_OK)
    {
        esp_rom_printf("%s uart_get_baudrate failed\n",__func__);
		return ESP_FAIL;
    }
    return ESP_OK;
}

int32_t uartSetBaud(void *host, uint32_t baudRate)
{
    uartInfo_t *dev;
	int ret;
    dev = pubFunc(host); 
    if((ret = uart_set_baudrate((uart_port_t)dev->port, baudRate)) != ESP_OK)
    {
        esp_rom_printf("%s uart_set_baudrate failed\n",__func__);
		return ESP_FAIL;
    }
    return ESP_OK;
}

int32_t uartGetAttribute(void *host, void *attribute)
{
    (void)host;
    (void)attribute;
    return ESP_OK;
}

int32_t uartSetAttribute(void *host, void *attribute)
{
    (void)host;
    (void)attribute;
    return ESP_OK;
}

int32_t uartSetTransMode(void *host, int mode)
{
    (void)host;
    (void)mode;
    return ESP_OK;
}

int32_t uartpollEvent(void *host, void *filep, void *table)
{
    (void)host;
    (void)filep;
    (void)table;
    return ESP_OK;
}