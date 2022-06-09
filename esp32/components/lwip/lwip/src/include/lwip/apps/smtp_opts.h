/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Martin Hentschel <info@cl-soft.de>
 *
 */

#ifndef LWIP_HDR_APPS_SMTP_OPTS_H
#define LWIP_HDR_APPS_SMTP_OPTS_H

#include "lwip/opt.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * @defgroup smtp_opts Options
 * @ingroup smtp
 * 
 * @{
 */
    
/** Set this to 1 to enable data handler callback on BODY */
#ifndef SMTP_BODYDH
#define SMTP_BODYDH             0
#endif

/** SMTP_DEBUG: Enable debugging for SNTP. */
#ifndef SMTP_DEBUG
#define SMTP_DEBUG              LWIP_DBG_OFF
#endif

/** Maximum length reserved for server name including terminating 0 byte */
#ifndef SMTP_MAX_SERVERNAME_LEN
#define SMTP_MAX_SERVERNAME_LEN 256
#endif

/** Maximum length reserved for username */
#ifndef SMTP_MAX_USERNAME_LEN
#define SMTP_MAX_USERNAME_LEN   32
#endif

/** Maximum length reserved for password */
#ifndef SMTP_MAX_PASS_LEN
#define SMTP_MAX_PASS_LEN       32
#endif

/** Set this to 0 if you know the authentication data will not change
 * during the smtp session, which saves some heap space. */
#ifndef SMTP_COPY_AUTHDATA
#define SMTP_COPY_AUTHDATA      1
#endif

/** Set this to 0 to save some code space if you know for sure that all data
 * passed to this module conforms to the requirements in the SMTP RFC.
 * WARNING: use this with care!
 */
#ifndef SMTP_CHECK_DATA
#define SMTP_CHECK_DATA         1
#endif

/** Set this to 1 to enable AUTH PLAIN support */
#ifndef SMTP_SUPPORT_AUTH_PLAIN
#define SMTP_SUPPORT_AUTH_PLAIN 1
#endif

/** Set this to 1 to enable AUTH LOGIN support */
#ifndef SMTP_SUPPORT_AUTH_LOGIN
#define SMTP_SUPPORT_AUTH_LOGIN 1
#endif

/* Memory allocation/deallocation can be overridden... */
#ifndef SMTP_STATE_MALLOC
#define SMTP_STATE_MALLOC(size)       mem_malloc(size)
#define SMTP_STATE_FREE(ptr)          mem_free(ptr)
#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* SMTP_OPTS_H */

