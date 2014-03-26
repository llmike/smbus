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

#include "g781.h"

int fd;
int status;

smbus_cmd_data_t cmd;

int print_g781(uint8_t slv_addr)
{
    /* Make sure that this is G781, read VID and DID */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=slv_addr;
    cmd.smbus_cmd=G781_MFGIO;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if ((status==EOK) && (cmd.data==G781_MFGIO_GMT))
    {
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=slv_addr;
        cmd.smbus_cmd=G781_DEVID;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if ((status==EOK) && ((cmd.data==G781_DEVID_G781)||(cmd.data==G781_DEVID_G7811)))
        {
            int remote_sign=0;

            fprintf(stdout, "Found G781 on address 0x%02X\n", slv_addr);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RLTS;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, "    Local temperature %dC", (int8_t)cmd.data);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RLLI;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, " (limit from %dC to ", (int8_t)cmd.data);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RLHN;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, "%dC)\n", (int8_t)cmd.data);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RRTE;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, "    Remote temperature %d", (int8_t)cmd.data);

            /* Check sign */
            if ((cmd.data & 0x80)==0x80)
            {
                remote_sign=1;
            }

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RTEXT;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            switch(cmd.data & G781_RTEXT_MASK)
            {
                case 0x00:
                     fprintf(stdout, ".000C");
                     break;
                case 0x20:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".875C");
                     }
                     else
                     {
                         fprintf(stdout, ".125C");
                     }
                     break;
                case 0x40:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".750C");
                     }
                     else
                     {
                         fprintf(stdout, ".250C");
                     }
                     break;
                case 0x60:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".625C");
                     }
                     else
                     {
                         fprintf(stdout, ".375C");
                     }
                     break;
                case 0x80:
                     fprintf(stdout, ".500C");
                     break;
                case 0xA0:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".375C");
                     }
                     else
                     {
                         fprintf(stdout, ".625C");
                     }
                     break;
                case 0xC0:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".250C");
                     }
                     else
                     {
                         fprintf(stdout, ".750C");
                     }
                     break;
                case 0xE0:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".125C");
                     }
                     else
                     {
                         fprintf(stdout, ".875C");
                     }
                     break;
                default:
                     {
                         fprintf(stdout, "\n");
                         fprintf(stdout, "Unsupported extended code 0x%02X\n", cmd.data);
                         return -1;
                     }
            }

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RRLS;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, " (limit from %dC to ", (int8_t)cmd.data);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RRHI;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, "%dC)\n", (int8_t)cmd.data);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RTOFS;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            fprintf(stdout, "    Remote temperature offset %d", (int8_t)cmd.data);

            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=slv_addr;
            cmd.smbus_cmd=G781_RTOFSEXT;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status!=EOK)
            {
                return -1;
            }
            switch(cmd.data & G781_RTOFSEXT_MASK)
            {
                case 0x00:
                     fprintf(stdout, ".000C");
                     break;
                case 0x20:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".875C");
                     }
                     else
                     {
                         fprintf(stdout, ".125C");
                     }
                     break;
                case 0x40:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".750C");
                     }
                     else
                     {
                         fprintf(stdout, ".250C");
                     }
                     break;
                case 0x60:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".625C");
                     }
                     else
                     {
                         fprintf(stdout, ".375C");
                     }
                     break;
                case 0x80:
                     fprintf(stdout, ".500C");
                     break;
                case 0xA0:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".375C");
                     }
                     else
                     {
                         fprintf(stdout, ".625C");
                     }
                     break;
                case 0xC0:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".250C");
                     }
                     else
                     {
                         fprintf(stdout, ".750C");
                     }
                     break;
                case 0xE0:
                     if (remote_sign==1)
                     {
                         fprintf(stdout, ".125C");
                     }
                     else
                     {
                         fprintf(stdout, ".875C");
                     }
                     break;
                default:
                     {
                         fprintf(stdout, "\n");
                         fprintf(stdout, "Unsupported extended code 0x%02X\n", cmd.data);
                         return -1;
                     }
            }
            fprintf(stdout, "\n        (this value must be added to remote temperature)\n");
        }
        else
        {
            if (status!=EOK)
            {
                return -1;
            }
        }
    }
    else
    {
        if (status!=EOK)
        {
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int smbus_id;
    int found_smbus=0;
    int found_g781=0;

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
                    exit(1);
                }
            }
            continue;
        }
        else
        {
            found_smbus=1;
        }

        /* Check if device is available  */
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=G781_SLAVE_ADDRESS;
        cmd.smbus_cmd=0x00;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status==EOK)
        {
            found_g781=1;
            if (print_g781(G781_SLAVE_ADDRESS)==-1)
            {
                fprintf(stdout, "\n");
                fprintf(stderr, "I/O error occured\n");
            }
        }

        /* Check if device is available using another address */
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=G781_SLAVE_ADDRESS2;
        cmd.smbus_cmd=0x00;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status==EOK)
        {
            found_g781=1;
            if (print_g781(G781_SLAVE_ADDRESS2)==-1)
            {
                fprintf(stdout, "\n");
                fprintf(stderr, "I/O error occured\n");
            }
        }
    }

    if (found_g781==0)
    {
        fprintf(stderr, "Can't find GMT G781 temperature sensor(s)!\n");
    }

    exit(0);
}
