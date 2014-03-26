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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/neutrino.h>

#include "ec.h"

#define SONY_LID_STATUS                 0xAC
/* bit 0 of this register reports 1 if lid is closed and 0 when opened   */
#define     SONY_LID_STATUS_CLOSED      0x01
/* bit 6 of this register reports 1 if wireless switched on and 0 if off */
#define     SONY_WIRELESS_SWITCH_ON     0x40

int main(int argc, char *argv[])
{
    uint8_t data;

    if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {
        fprintf(stderr, "Can't gain i/o permissions\n");
        return -1;
    }

    nanospin_calibrate(0);

    data=read_ec(SONY_LID_STATUS);
    if (data & SONY_LID_STATUS_CLOSED)
    {
        fprintf(stdout, "LID is closed\n");
    }
    else
    {
        fprintf(stdout, "LID is opened\n");
    }

    if (data & SONY_WIRELESS_SWITCH_ON)
    {
        fprintf(stdout, "Wireless is switched ON\n");
    }
    else
    {
        fprintf(stdout, "Wireless is switched OFF\n");
    }

    exit(0);
}
