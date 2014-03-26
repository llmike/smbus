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

#include "proto.h"

int ich_devctl(resmgr_context_t* ctp, io_devctl_t* msg, ich_ocb_t* ocb)
{
    int         status, ret=EOK;
    ich_dev_t* dev=ocb->dev;

    status = iofunc_devctl_default(ctp, msg, &ocb->hdr);
    if (status != _RESMGR_DEFAULT)
    {
        slogf(_SLOGC_ICH_SMBUS, _SLOG_ERROR, "[smb-ich] iofunc_devctl_default() failed");
        return status;
    }

    /* Lock hardware access mutex */
    pthread_mutex_lock(&dev->ioaccess);

    switch (msg->i.dcmd)
    {
        case DCMD_SMBUS_READ:
             ret=ich_smbus_read(ctp, msg, ocb);
             break;
        case DCMD_SMBUS_WRITE:
             ret=ich_smbus_write(ctp, msg, ocb);
             break;
        default:
             slogf(_SLOGC_ICH_SMBUS, _SLOG_ERROR, "[smb-ich] Unknown devctl command: %x", msg->i.dcmd);
             ret = ENOSYS;
             break;
    }

    /* Unlock hardware access mutex */
    pthread_mutex_unlock(&dev->ioaccess);

    return (_RESMGR_ERRNO(ret));
}
