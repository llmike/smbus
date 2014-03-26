/*
 * $QNXLicenseC:
 * Copyright 2011-2012, QNX Software Systems.
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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <atomic.h>
#include <hw/inout.h>

#include "ec.h"

void write_ec_sc(uint8_t data)
{
    out8(EC_SC, data);
}

uint8_t read_ec_sc()
{
    return in8(EC_SC);
}

void write_ec_data(uint8_t data)
{
    out8(EC_DATA, data);
}

uint8_t read_ec_data()
{
    return in8(EC_DATA);
}

void write_ec(uint8_t reg, uint8_t data)
{
    volatile uint8_t tempval;
    struct timespec  ts_ns={0, 1};
    int              timeout=100;

    /* Send Write from Embedded Controller command */
    write_ec_sc(WR_EC);

    /* Wait for empty input buffer */
    do {
        tempval=read_ec_sc();
        if ((tempval & EC_SC_IBF)!=EC_SC_IBF)
        {
            /* Write completed */
            timeout=100;
            break;
        }
        nanosleep(&ts_ns, NULL);
        timeout--;
        if (timeout==0)
        {
            timeout=100;
            break;
        }
    } while(1);

    /* Send register index to Data Port */
    write_ec_data(reg);

    /* Wait for empty input buffer */
    do {
        tempval=read_ec_sc();
        if ((tempval & EC_SC_IBF)!=EC_SC_IBF)
        {
            /* Write completed */
            timeout=100;
            break;
        }
        nanosleep(&ts_ns, NULL);
        timeout--;
        if (timeout==0)
        {
            timeout=100;
            break;
        }
    } while(1);

    /* Send register data to Data Port */
    write_ec_data(data);

    /* Wait for empty input buffer */
    do {
        tempval=read_ec_sc();
        if ((tempval & EC_SC_IBF)!=EC_SC_IBF)
        {
            /* Write completed */
            break;
        }
        nanosleep(&ts_ns, NULL);
        timeout--;
        if (timeout==0)
        {
            break;
        }
    } while(1);
}

uint8_t read_ec(uint8_t reg)
{
    volatile uint8_t tempval;
    struct timespec  ts_ns={0, 1};
    int              timeout=100;

    /* Send Read from Embedded Controller command */
    write_ec_sc(RD_EC);

    /* Wait for empty input buffer */
    do {
        tempval=read_ec_sc();
        if ((tempval & EC_SC_IBF)!=EC_SC_IBF)
        {
            /* Write completed */
            timeout=100;
            break;
        }
        nanosleep(&ts_ns, NULL);
        timeout--;
        if (timeout==0)
        {
            timeout=100;
            break;
        }
    } while(1);

    /* Send register index to Data Port */
    write_ec_data(reg);

    /* Wait for full output buffer */
    do {
        tempval=read_ec_sc();
        if ((tempval & EC_SC_OBF)==EC_SC_OBF)
        {
            /* Read completed */
            break;
        }
        nanosleep(&ts_ns, NULL);
        timeout--;
        if (timeout==0)
        {
            return 0xFF;
        }
    } while(1);

    return read_ec_data();
}
