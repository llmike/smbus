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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>

#include <devctl.h>
#include <inttypes.h>

#include <dcmd_smbus.h>

int fd;

smbus_cmd_data_t cmd;

int main(int argc, char *argv[])
{
    int status;
    int it, jt;
    int smbus_id;
    int device_id;
    int found_smbus=0;
    int use_byte_read=1;
    int c;

    while (optind < argc)
    {
        if ((c = getopt(argc, argv, "bw")) == -1)
        {
            optind++;
            continue;
        }
        switch (c)
        {
            case 'b':
                 use_byte_read=1;
                 break;
            case 'w':
                 use_byte_read=0;
                 break;
        }
    }

    /* Go through all available SMBUSes */
    for (smbus_id=0; smbus_id<=99; smbus_id++)
    {
        char smbus_name[PATH_MAX];

        snprintf(smbus_name, PATH_MAX, "/dev/smb%d", smbus_id);

        fd=open(smbus_name, O_RDWR);
        if (fd<0)
        {
            if (smbus_id==99)
            {
                if (!found_smbus)
                {
                    fprintf(stderr, "Can't find any SMBus device!\n");
                }
                exit(1);
            }
            continue;
        }
        else
        {
            found_smbus=1;
        }
        fprintf(stdout, "Found SMBUS%d\n", smbus_id);

        /* Check if device is available  */
        for (device_id=0x00; device_id<0x80; device_id+=0x01)
        {
            char buff[17];

            if (use_byte_read==0)
            {
                cmd.cmd_type=SMBUS_CMD_WORD_DATA;
            }
            else
            {
                cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            }
            cmd.slv_addr=device_id;
            cmd.smbus_cmd=0x00;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                cmd.slv_addr=device_id;
                cmd.smbus_cmd=0x01;
                cmd.length=0x00;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            }
            if (status!=EOK)
            {
                cmd.slv_addr=device_id;
                cmd.smbus_cmd=0x13;
                cmd.length=0x00;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            }
            if (status!=EOK)
            {
                cmd.slv_addr=device_id;
                cmd.smbus_cmd=0x20;
                cmd.length=0x00;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            }
            if (status!=EOK)
            {
                cmd.slv_addr=device_id;
                cmd.smbus_cmd=0x80;
                cmd.length=0x00;
                status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            }
            if (status==EOK)
            {
                if ((device_id>=0x50)&&(device_id<=0x57))
                {
                    fprintf(stdout, "    Found SPD EEPROM at address 0x%02X\n", device_id);
                }
                else
                {
                    if (device_id==0x69)
                    {
                        fprintf(stdout, "    Found Clock Chip at address 0x%02X\n", device_id);
                    }
                    else
                    {
                        if (device_id==0x09)
                        {
                            fprintf(stdout, "    Found Smart Battery Charger at address 0x%02X\n", device_id);
                        }
                        else
                        {
                            if (device_id==0x0A)
                            {
                                fprintf(stdout, "    Found Smart Battery Selector/Manager at address 0x%02X\n", device_id);
                            }
                            else
                            {
                                if (device_id==0x0B)
                                {
                                    fprintf(stdout, "    Found Smart Battery at address 0x%02X\n", device_id);
                                }
                                else
                                {
                                    if ((device_id==0x4C)||(device_id==0x4D))
                                    {
                                        fprintf(stdout, "    Found Sensor Chip at address 0x%02X\n", device_id);
                                    }
                                    else
                                    {
                                        fprintf(stdout, "    Found device presence at address 0x%02X\n", device_id);
                                    }
                                }
                            }
                        }
                    }
                }

                if (use_byte_read==1)
                {
                    fprintf(stdout, "          | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
                    fprintf(stdout, "       ------------------------------------------------------------------------\n");
                }
                else
                {
                    fprintf(stdout, "          | 00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F\n");
                    fprintf(stdout, "        ------------------------------------------------------------------------------------\n");
                }

                for (jt=0; jt<16; jt++)
                {
                    fprintf(stdout, "       %02X | ", jt*16);
                    for (it=0; it<16; it++)
                    {
                        /* Read EEPROM content */
                        if (use_byte_read==0)
                        {
                            cmd.cmd_type=SMBUS_CMD_WORD_DATA;
                        }
                        else
                        {
                            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
                        }
                        cmd.slv_addr=device_id;
                        cmd.smbus_cmd=jt*16+it;
                        cmd.length=0x00;
                        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
                        if (use_byte_read==1)
                        {
                            if (status==EOK)
                            {
                                fprintf(stdout, "%02X ", cmd.data);
                                if ((cmd.data>=0x20) && (cmd.data<0x7F))
                                {
                                    buff[it]=cmd.data;
                                }
                                else
                                {
                                    buff[it]='.';
                                }
                            }
                            else
                            {
                                fprintf(stdout, "-- ");
                                buff[it]='-';
                            }
                        }
                        else
                        {
                            if (status==EOK)
                            {
                                fprintf(stdout, "%04X ", cmd.data);
                            }
                            else
                            {
                                fprintf(stdout, "---- ");
                            }
                        }
                    }
                    if (use_byte_read==1)
                    {
                        buff[16]=0x00;
                        fprintf(stdout, "   %s", buff);
                    }
                    fprintf(stdout, "\n");
                }
            }
        }
        close(fd);
    }

    exit(0);
}
