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
#include <stdlib.h>

ich_ocb_t* _ich_ocb_calloc(resmgr_context_t* ctp, ich_dev_t* dev)
{
    ich_ocb_t* ocb;

    ocb = calloc(1, sizeof(ich_ocb_t));
    if (ocb != NULL)
    {
        ocb->dev = dev;
    }

    return ocb;
}

void _ich_ocb_free(ich_ocb_t* ocb)
{
    free(ocb);
}
