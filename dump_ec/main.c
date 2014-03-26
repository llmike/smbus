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

int main(int argc, char *argv[])
{
    int it, jt;
    uint8_t data;
    char buff[17];

    if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {
        fprintf(stderr, "Can't gain i/o permissions\n");
        return -1;
    }

    nanospin_calibrate(0);

    fprintf(stdout, "Embedded Controller, 256 registers:\n\n");
    fprintf(stdout, "       | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    fprintf(stdout, "    -------------------------------------------------------------------------\n");

    for (jt=0; jt<16; jt++)
    {
        fprintf(stdout, "    %02X | ", jt*16);
        for (it=0; it<16; it++)
        {
            data=read_ec(jt*16+it);
            fprintf(stdout, "%02X ", data);
            if ((data>=0x20) && (data<0x7F))
            {
                buff[it]=data;
            }
            else
            {
                buff[it]='.';
            }
        }
        buff[16]=0x00;
        fprintf(stdout, "    %s", buff);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");

    exit(0);
}
