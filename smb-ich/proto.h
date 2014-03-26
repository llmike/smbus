/*
 * $QNXLicenseC:
 * Copyright 2011-2012, QNX Software Systems.
 * Copyright 2011-2012, Mike Gorchak.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <atomic.h>
#include <sys/procmgr.h>
#include <sys/slogcodes.h>
#include <hw/inout.h>
#include "dcmd_smbus.h"

struct _ich_dev;
struct _ich_ocb;
#define IOFUNC_ATTR_T   struct _ich_dev
#define IOFUNC_OCB_T    struct _ich_ocb
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "smbus_ich.h"

typedef struct _ich_ocb
{
    iofunc_ocb_t        hdr;
    struct _ich_dev*   dev;
} ich_ocb_t;

typedef struct _ich_dev
{
    iofunc_attr_t       hdr;
    dispatch_t*         dpp;
    resmgr_context_t*   ctp;

    int                 verbose;

    /* Base i/o port */
    uintptr_t           base;
    uint64_t            base_mmio_phys;
    uint8_t*            base_mmio;

    /* Hardware access mutex */
    pthread_mutex_t     ioaccess;

    int                 priority;
    int                 chid;
    int                 coid;
    int                 tid;
} ich_dev_t;

#define _SLOGC_ICH_SMBUS (_SLOGC_TEST + 0x10)

extern ich_ocb_t* _ich_ocb_calloc(resmgr_context_t* ctp, ich_dev_t* dev);
extern void _ich_ocb_free(ich_ocb_t* ocb);
extern int ich_devctl(resmgr_context_t* ctp, io_devctl_t* msg, ich_ocb_t* ocb);
extern int ich_init(ich_dev_t* dev, int mode);
extern int ich_smbus_read(resmgr_context_t* ctp, io_devctl_t* msg, ich_ocb_t* ocb);
extern int ich_smbus_write(resmgr_context_t* ctp, io_devctl_t* msg, ich_ocb_t* ocb);

#endif /* __PROTO_H__ */
