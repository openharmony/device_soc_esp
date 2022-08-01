/*
 * Copyright (c) 2022 Hunan OpenValley Digital Industry Development Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdlib.h>
#include "esp_osal.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "los_config.h"
#include "los_task.h"
#include "los_debug.h"
#include "los_queue.h"
#include "los_memory.h"
#include "esp_osal/event_groups.h"
#include "esp_rom_uart.h"
#include "xtensa_rtos.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <errno.h>
#include "hal/uart_ll.h"

extern UINT64 OsTickCount;
// =================================== 调试相关
extern BOOL g_taskScheduled;
static UINT32 s_LogMuxHandle = 0;
extern UINT16 g_losTaskLock;
static void s_vprintf(const char* fmt, va_list ap)
{
    int len;
    uint8_t taskLock;
    static char buf[2][128 * 3];
    char* pbuf;
    if (xPortInterruptedFromISRContext() || g_losTaskLock || (!g_taskScheduled))
    {
        taskLock = 1;
        pbuf = buf[1];
    }
    else
    {
        taskLock = 0;
        pbuf = buf[0];
    }
    if (!taskLock)
    {
        while (!s_LogMuxHandle)
        {
            LOS_MuxCreate(&s_LogMuxHandle);
            LOS_TaskDelay(1);
        }
        LOS_MuxPend(s_LogMuxHandle, LOS_WAIT_FOREVER);
    }
    len = vsnprintf(pbuf, sizeof(buf[0]) - 4, fmt, ap);
    if (len > 0)
    {
        uint16_t fill_len;
        for (fill_len = uart_ll_get_txfifo_len(&UART0); fill_len < len;)
        {
            if (fill_len)
            {
                uart_ll_write_txfifo(&UART0, (uint8_t*)pbuf, fill_len);
                len -= fill_len;
                pbuf += fill_len;
            }
            if (!taskLock)
                LOS_TaskDelay(1); // LOS_TaskYield();
            fill_len = uart_ll_get_txfifo_len(&UART0);
        }
        if (len > 0)
            uart_ll_write_txfifo(&UART0, (uint8_t*)pbuf, len);
        // while(!uart_ll_is_tx_idle(&UART0))LOS_TaskYield();
    }
    if (!taskLock)
        LOS_MuxPost(s_LogMuxHandle);
}

// 获取当前滴答时间
TickType_t IRAM_ATTR xTaskGetTickCount(void)
{
    return (TickType_t)OsTickCount;
}

// 获取当前滴答时间(中断内)
TickType_t IRAM_ATTR xTaskGetTickCountFromISR(void)
{
    return (TickType_t)OsTickCount;
}

#ifndef MALLOC_CAP_8BIT
#define MALLOC_CAP_8BIT (1 << 2)
void* heap_caps_malloc(size_t size, uint32_t caps);
void heap_caps_free(void* ptr);
void* heap_caps_aligned_alloc(size_t alignment, size_t size, uint32_t caps);
#endif

UINT32 __wrap_OsMemSystemInit(VOID)
{
    return LOS_OK;
}

IRAM_ATTR VOID* __wrap_LOS_MemAlloc(VOID* pool, UINT32 size)
{
    return heap_caps_malloc(size, MALLOC_CAP_8BIT);
}

IRAM_ATTR UINT32 __wrap_LOS_MemFree(VOID* pool, VOID* ptr)
{
    if (!ptr) {
        return LOS_NOK;
    }
    else {
        heap_caps_free(ptr);
        return LOS_OK;
    }
}

IRAM_ATTR VOID* __wrap_LOS_MemAllocAlign(VOID* pool, UINT32 size, UINT32 boundary)
{
    return heap_caps_aligned_alloc(boundary, size, MALLOC_CAP_8BIT);
}

#define socklen_t uint32_t

int accept(int s, void* addr, socklen_t* addrlen)
{
    int lwip_accept(int s, void* addr, socklen_t * addrlen);
    return lwip_accept(s, addr, addrlen);
}
int bind(int s, const void* name, socklen_t namelen)
{
    int lwip_bind(int s, const void* name, socklen_t namelen);
    return lwip_bind(s, name, namelen);
}
int setsockopt(int s, int level, int optname, const void* opval, socklen_t optlen)
{
    int lwip_setsockopt(int s, int level, int optname, const void* opval, socklen_t optlen);
    return lwip_setsockopt(s, level, optname, opval, optlen);
}
int getsockname(int s, void* name, socklen_t* namelen)
{
    int lwip_getsockname(int s, void* name, socklen_t * namelen);
    return lwip_getsockname(s, name, namelen);
}
int getsockopt(int s, int level, int optname, void* opval, socklen_t* optlen)
{
    int lwip_getsockopt(int s, int level, int optname, void* opval, socklen_t * optlen);
    return lwip_getsockopt(s, level, optname, opval, optlen);
}

int access(const char* path, int amode)
{
    esp_rom_printf("path=%s mode=0x%X\n", path, amode);
    return -1;
}

int shutdown(int s, int how)
{
    int lwip_shutdown(int s, int how);
    return lwip_shutdown(s, how);
}

const char* inet_ntop(int af, const void* src, char* dst, socklen_t size)
{
    const char* lwip_inet_ntop(int af, const void* src, char* dst, socklen_t size);
    return lwip_inet_ntop(af, src, dst, size);
}

int inet_pton(int af, const char* src, void* dst)
{
    int lwip_inet_pton(int af, const char* src, void* dst);
    return lwip_inet_pton(af, src, dst);
}

uint32_t inet_addr(const char* cp)
{
    uint32_t ipaddr_addr(const char* cp);
    return ipaddr_addr(cp);
}

ssize_t recv(int s, void* mem, size_t len, int flags)
{
    ssize_t lwip_recv(int s, void* mem, size_t len, int flags);
    return lwip_recv(s, mem, len, flags);
}

ssize_t recvfrom(int s, void* mem, size_t len, int flags, void* from, socklen_t* fromlen)
{
    ssize_t lwip_recvfrom(int s, void* mem, size_t len, int flags, void* from, socklen_t * fromlen);
    return lwip_recvfrom(s, mem, len, flags, from, fromlen);
}

ssize_t send(int s, const void* dataptr, size_t size, int flags)
{
    ssize_t lwip_send(int s, const void* dataptr, size_t size, int flags);
    return lwip_send(s, dataptr, size, flags);
}

ssize_t sendmsg(int s, const void* message, int flags)
{
    ssize_t lwip_sendmsg(int s, const void* message, int flags);
    return lwip_sendmsg(s, message, flags);
}

ssize_t sendto(int s, const void* dataptr, size_t size, int flags, const void* to, socklen_t tolen)
{
    ssize_t lwip_sendto(int s, const void* dataptr, size_t size, int flags, const void* to, socklen_t tolen);
    return lwip_sendto(s, dataptr, size, flags, to, tolen);
}

int socket(int domain, int type, int protocol)
{
    int lwip_socket(int domain, int type, int protocol);
    return lwip_socket(domain, type, protocol);
}

int closesocket(int s)
{
    int lwip_close(int s);
    return lwip_close(s);
}

int connect(int s, const void* name, socklen_t namelen)
{
    int lwip_connect(int s, const void* name, socklen_t namelen);
    return lwip_connect(s, name, namelen);
}

int listen(int s, int backlog)
{
    int lwip_listen(int s, int backlog);
    return lwip_listen(s, backlog);
}

int gethostbyname_r(const char* name, void* ret, char* buf, size_t buflen, void** result, int* h_errnop)
{
    int lwip_gethostbyname_r(const char* name, void* ret, char* buf, size_t buflen, void** result, int* h_errnop);
    return lwip_gethostbyname_r(name, ret, buf, buflen, result, h_errnop);
}

void* gethostbyname(const char* name)
{
    void* lwip_gethostbyname(const char* name);
    return lwip_gethostbyname(name);
}

void freeaddrinfo(void* ai)
{
    void lwip_freeaddrinfo(void* ai);
    lwip_freeaddrinfo(ai);
}

int getaddrinfo(const char* nodename, const char* servname, const void* hints, void** res)
{
    int lwip_getaddrinfo(const char* nodename, const char* servname, const void* hints, void** res);
    return lwip_getaddrinfo(nodename, servname, hints, res);
}
