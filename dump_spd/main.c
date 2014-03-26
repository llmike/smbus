/*
 * $QNXLicenseC:
 * Copyright 2012, QNX Software Systems.
 * Copyright 2012, Mike Gorchak.
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

#include "spd_manufacturer.h"

int fd;
int status;

smbus_cmd_data_t cmd;

int decode_sdr(uint8_t spd)
{
    fprintf(stdout, "        SDRAM decoder is not implemented\n");

    return 0;
}

int decode_ddr(uint8_t spd)
{
    fprintf(stdout, "        DDR decoder is not implemented\n");

    return 0;
}

int decode_ddr2(uint8_t spd)
{
    int size=0;
    int rows=0;
    int columns=0;
    int width=0;
    int ranks=0;
    int it;

    /* Check initialized size  */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x00;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        SPD initialized size   : 0x%02X (%d) bytes\n", cmd.data, cmd.data);

    /* Check total size  */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x01;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        SPD total size         : ");

    switch(cmd.data)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
             size=0x01<<cmd.data;
             break;
        case 0:
        default:
            fprintf(stdout, "Unknown\n");
            break;
    }
    if (size!=0)
    {
        fprintf(stdout, "0x%04X (%d) bytes\n", size, size);
    }

    /* Check SPD revision  */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x3E;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        SPD revision           : %d.%d\n", (cmd.data & 0xF0)>>4, cmd.data & 0x0F);

    /* Check rows and columns */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x03;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    rows=cmd.data;

    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x04;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    columns=cmd.data;

    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x06;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    width=cmd.data;

    fprintf(stdout, "        Number of row addresses: %d\n", rows);
    fprintf(stdout, "        Number of col addresses: %d\n", columns);
    fprintf(stdout, "        Module data width      : %d bits\n", width);

    /* Read module attributes */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x05;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    ranks=(cmd.data & 0x07)+1;
    fprintf(stdout, "        Number of ranks        : %d\n", (cmd.data & 0x07)+1);
    fprintf(stdout, "        Card on card           : %s\n", (cmd.data & 0x08) ? "Yes" : "No");
    fprintf(stdout, "        DRAM package           : %s\n", (cmd.data & 0x10) ? "Stack" : "Planar");
    fprintf(stdout, "        Module height          : ");

    switch((cmd.data & 0xE0)>>5)
    {
        case 0:
             fprintf(stdout, "Less than 25.4mm\n");
             break;
        case 1:
             fprintf(stdout, "25.4mm\n");
             break;
        case 2:
             fprintf(stdout, "Greater than 25.4mm and less than 30.0mm\n");
             break;
        case 3:
             fprintf(stdout, "30.0mm\n");
             break;
        case 4:
             fprintf(stdout, "30.5mm\n");
             break;
        case 5:
             fprintf(stdout, "Greater than 30.5mm\n");
             break;
        case 6:
        case 7:
             fprintf(stdout, "Unknown\n");
             break;
    }

    fprintf(stdout, "        SDRAM amount of banks  : ");
    /* Read banks amount */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x11;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "%d\n", cmd.data);

    fprintf(stdout, "        Module memory size     : ");

    /* Read rank size */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x1F;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    switch (cmd.data)
    {
        case 0x01:
             fprintf(stdout, "%dMb\n", 1024 * ranks);
             break;
        case 0x02:
             fprintf(stdout, "%dMb\n", 2048 * ranks);
             break;
        case 0x04:
             fprintf(stdout, "%dMb\n", 4096 * ranks);
             break;
        case 0x08:
             fprintf(stdout, "%dMb\n", 8192 * ranks);
             break;
        case 0x10:
             fprintf(stdout, "%dMb\n", 16384 * ranks);
             break;
        case 0x20:
             fprintf(stdout, "%dMb\n", 128 * ranks);
             break;
        case 0x40:
             fprintf(stdout, "%dMb\n", 256 * ranks);
             break;
        case 0x80:
             fprintf(stdout, "%dMb\n", 512 * ranks);
             break;
        default:
             fprintf(stdout, "Undefined\n");
             break;
    }

    fprintf(stdout, "        Module type information: ");
    /* Read module type info */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x14;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    switch(cmd.data)
    {
        case 0x01:
             fprintf(stdout, "RDIMM\n");
             break;
        case 0x02:
             fprintf(stdout, "UDIMM\n");
             break;
        case 0x04:
             fprintf(stdout, "SO-DIMM\n");
             break;
        case 0x06:
             fprintf(stdout, "72b-SO-CDIMM\n");
             break;
        case 0x07:
             fprintf(stdout, "72b-SO-RDIMM\n");
             break;
        case 0x08:
             fprintf(stdout, "Micro-DIMM\n");
             break;
        case 0x10:
             fprintf(stdout, "Mini-RDIMM\n");
             break;
        case 0x20:
             fprintf(stdout, "Mini-UDIMM\n");
             break;
        default:
             fprintf(stdout, "Undefined\n");
             break;
    }

    /* Read voltage interface */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x08;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        Voltage interface level: ");

    switch(cmd.data)
    {
        case 0:
             fprintf(stdout, "TTL/5V tolerant\n");
             break;
        case 1:
             fprintf(stdout, "LVTTL (not 5V tolerant)\n");
             break;
        case 2:
             fprintf(stdout, "HSTL 1.5V\n");
             break;
        case 3:
             fprintf(stdout, "SSTL 3.3V\n");
             break;
        case 4:
             fprintf(stdout, "SSTL 2.5V\n");
             break;
        case 5:
             fprintf(stdout, "SSTL 1.8V\n");
             break;
        default:
             fprintf(stdout, "Unknown\n");
    }

    fprintf(stdout, "        Supported CAS latencies: ");

    /* Read supported CAS latencies */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x12;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    if (cmd.data==0x00)
    {
        fprintf(stdout, "Undefined");
    }
    if (cmd.data & 0x80)
    {
        fprintf(stdout, "7");
        if (cmd.data & 0x7C)
        {
            fprintf(stdout, ", ");
        }
    }
    if (cmd.data & 0x40)
    {
        fprintf(stdout, "6");
        if (cmd.data & 0x3C)
        {
            fprintf(stdout, ", ");
        }
    }
    if (cmd.data & 0x20)
    {
        fprintf(stdout, "5");
        if (cmd.data & 0x1C)
        {
            fprintf(stdout, ", ");
        }
    }
    if (cmd.data & 0x10)
    {
        fprintf(stdout, "4");
        if (cmd.data & 0x0C)
        {
            fprintf(stdout, ", ");
        }
    }
    if (cmd.data & 0x08)
    {
        fprintf(stdout, "3");
        if (cmd.data & 0x04)
        {
            fprintf(stdout, ", ");
        }
    }
    if (cmd.data & 0x04)
    {
        fprintf(stdout, "2");
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "        Memory module frequency: ");

    /* Read supported CAS latencies */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x09;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    {
        int frequency=10000*2/(((cmd.data & 0xF0)>>4)*10 + (cmd.data & 0x0F));

        fprintf(stdout, "%dMHz", frequency);
        switch(frequency)
        {
            case 400:
                 fprintf(stdout, " (PC2-3200)\n");
                 break;
            case 533:
                 fprintf(stdout, " (PC2-4200)\n");
                 break;
            case 666:
                 fprintf(stdout, " (PC2-5300)\n");
                 break;
            case 800:
                 fprintf(stdout, " (PC2-6400)\n");
                 break;
            case 1066:
                 fprintf(stdout, " (PC2-8500)\n");
                 break;
            default:
                 fprintf(stdout, "\n");
                 break;
        }
    }

    fprintf(stdout, "        Module manufacturer ID : ");

    /* Read manufacturer JEDEC ID */
    for (it=0x40; it<0x48; it++)
    {
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=spd;
        cmd.smbus_cmd=it;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            return -1;
        }
        if (cmd.data==0x7F)
        {
            if (it==0x47)
            {
                fprintf(stdout, "Unknown\n");
            }
            continue;
        }
        fprintf(stdout, "Bank: %d, ID: 0x%02X (", it-0x40, cmd.data);
        fprintf(stdout, "%s", jedec_manufacturer[it-0x40][cmd.data & 0x7F]);
        fprintf(stdout, ")\n");
        break;
    }

    fprintf(stdout, "        Module serial number   : ");

    /* Construct module serial number */
    for (it=0x5F; it<0x63; it++)
    {
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=spd;
        cmd.smbus_cmd=it;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            return -1;
        }
        fprintf(stdout, "%02X", cmd.data);
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "        Module manufactur. date: ");
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x5D;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "Year: %d ", ((cmd.data & 0xF0)>>4)*10 + (cmd.data & 0x0F) + 2000);
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x5E;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "Week: %d%d\n", (cmd.data & 0xF0)>>4, cmd.data & 0x0F);

    fprintf(stdout, "        Module part number     : ");
    for(it=0x49; it<0x5B; it++)
    {
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=spd;
        cmd.smbus_cmd=it;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            return -1;
        }
        if (cmd.data==0x00)
        {
            break;
        }
        if ((cmd.data>=0x20) && (cmd.data<=0x7E))
        {
            fprintf(stdout, "%c", cmd.data);
        }
        else
        {
            fprintf(stdout, "_");
        }
    }
    fprintf(stdout, "\n");

    return 0;
}

int decode_ddr3(uint8_t spd)
{
    int sdram_capacity=0;
    int primary_bus_width=0;
    int sdram_width=8;
    int ranks=0;
    int bank=0;
    int ctime=0;
    int ddr3_clock=0;
    int pc_clock=0;
    int dividend=0;
    int divisor=0;
    int it=0;

    /* Check initialized size  */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x00;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        CRC covers bytes range : %s\n", (cmd.data & 0x80) ? "0~116" : "0~125");
    fprintf(stdout, "        SPD bytes total        : %s\n", (((cmd.data & 0x70)>>4)==1) ? "256" : "Undefined");
    fprintf(stdout, "        SPD bytes used         : ");
    switch(cmd.data & 0x0F)
    {
        case 1:
             fprintf(stdout, "128\n");
             break;
        case 2:
             fprintf(stdout, "176\n");
             break;
        case 3:
             fprintf(stdout, "256\n");
             break;
        default:
             fprintf(stdout, "Undefined\n");
    }

    /* Check SPD revision  */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x01;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        SPD revision           : %d.%d\n", (cmd.data & 0xF0)>>4, cmd.data & 0x0F);

    fprintf(stdout, "        Module type information: ");
    /* Read module type info */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x03;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    switch(cmd.data & 0x0F)
    {
        case 0x01:
             fprintf(stdout, "RDIMM\n");
             break;
        case 0x02:
             fprintf(stdout, "UDIMM\n");
             break;
        case 0x03:
             fprintf(stdout, "SO-DIMM\n");
             break;
        case 0x04:
             fprintf(stdout, "Micro-DIMM\n");
             break;
        case 0x05:
             fprintf(stdout, "Mini-RDIMM\n");
             break;
        case 0x06:
             fprintf(stdout, "Mini-UDIMM\n");
             break;
        case 0x07:
             fprintf(stdout, "Mini-CDIMM\n");
             break;
        case 0x08:
             fprintf(stdout, "72b-SO-UDIMM\n");
             break;
        case 0x09:
             fprintf(stdout, "72b-SO-RDIMM\n");
             break;
        case 0x0A:
             fprintf(stdout, "72b-SO-CDIMM\n");
             break;
        case 0x0B:
             fprintf(stdout, "LRDIMM\n");
             break;
        case 0x0C:
             fprintf(stdout, "16b-SO-DIMM\n");
             break;
        case 0x0D:
             fprintf(stdout, "32b-SO-DIMM\n");
             break;
        default:
             fprintf(stdout, "Undefined\n");
             break;
    }

    /* Read SDRAM density and banks */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x04;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "        SDRAM amount of banks  : ");
    switch((cmd.data & 0x70)>>4)
    {
        case 0:
             fprintf(stdout, "8\n");
             break;
        case 1:
             fprintf(stdout, "16\n");
             break;
        case 2:
             fprintf(stdout, "32\n");
             break;
        case 3:
             fprintf(stdout, "64\n");
             break;
    }

    /* Store SDRAM capacity for further usage */
    switch(cmd.data & 0x0F)
    {
        case 0:
             sdram_capacity=256;
             break;
        case 1:
             sdram_capacity=512;
             break;
        case 2:
             sdram_capacity=1024;
             break;
        case 3:
             sdram_capacity=2048;
             break;
        case 4:
             sdram_capacity=4096;
             break;
        case 5:
             sdram_capacity=8192;
             break;
        case 6:
             sdram_capacity=16384;
             break;
    }

    /* Read SDRAM addressing */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x05;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        Number of row addresses: ");
    switch((cmd.data & 0x38)>>3)
    {
        case 0:
             fprintf(stdout, "12\n");
             break;
        case 1:
             fprintf(stdout, "13\n");
             break;
        case 2:
             fprintf(stdout, "14\n");
             break;
        case 3:
             fprintf(stdout, "15\n");
             break;
        case 4:
             fprintf(stdout, "16\n");
             break;
        default:
             fprintf(stdout, "Undefined\n");
             break;
    }
    fprintf(stdout, "        Number of col addresses: ");
    switch(cmd.data & 0x07)
    {
        case 0:
             fprintf(stdout, "9\n");
             break;
        case 1:
             fprintf(stdout, "10\n");
             break;
        case 2:
             fprintf(stdout, "11\n");
             break;
        case 3:
             fprintf(stdout, "12\n");
             break;
        default:
             fprintf(stdout, "Undefined\n");
             break;
    }

    /* Read module organization */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x07;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        Number of ranks        : ");
    switch((cmd.data & 0x38)>>3)
    {
        case 0:
             fprintf(stdout, "1");
             ranks=1;
             break;
        case 1:
             fprintf(stdout, "2");
             ranks=2;
             break;
        case 2:
             fprintf(stdout, "3");
             ranks=3;
             break;
        case 3:
             fprintf(stdout, "4");
             ranks=4;
             break;
    }
    fprintf(stdout, "\n");
    switch(cmd.data & 0x07)
    {
        case 0:
             sdram_width=4;
             break;
        case 1:
             sdram_width=8;
             break;
        case 2:
             sdram_width=16;
             break;
        case 3:
             sdram_width=32;
             break;
    }

    /* Read primary bus width */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x08;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    switch(cmd.data & 0x07)
    {
        case 0:
             primary_bus_width=8;
             break;
        case 1:
             primary_bus_width=16;
             break;
        case 2:
             primary_bus_width=32;
             break;
        case 3:
             primary_bus_width=64;
             break;
    }

    fprintf(stdout, "        Module memory size     : ");
    fprintf(stdout, "%dMb\n", sdram_capacity/8*primary_bus_width/sdram_width*ranks);

    /* Read dividend */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x0A;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    dividend=cmd.data;

    /* Read divisor */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x0B;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    divisor=cmd.data;

    /* Read ctime */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x0C;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    ctime=(int)cmd.data*1000*dividend/divisor;
    ddr3_clock=2*1000000000/ctime;
    pc_clock=ddr3_clock*primary_bus_width/8;
    fprintf(stdout, "        Module memory speed    : %dMHz (PC3-%d)\n", ddr3_clock/1000, pc_clock/1000);

    /* Read nominal voltage */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x06;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "        Module nominal voltage : ");
    if (cmd.data & 0x04)
    {
        fprintf(stdout, "1.25V");
        if ((cmd.data & 0x02) || ((cmd.data & 0x01)==0x00))
        {
            fprintf(stdout, ", ");
        }
    }
    if (cmd.data & 0x02)
    {
        fprintf(stdout, "1.35V");
        if ((cmd.data & 0x01)==0x00)
        {
            fprintf(stdout, ", ");
        }
    }
    if ((cmd.data & 0x01)==0x00)
    {
        fprintf(stdout, "1.5V");
    }
    fprintf(stdout, "\n");

    /* Read memory manufacturer */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x75;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    bank=cmd.data & 0x7F;

    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x76;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        Module manufacturer ID : ");
    fprintf(stdout, "Bank: %d, ID: 0x%02X (", bank, cmd.data);
    fprintf(stdout, "%s", jedec_manufacturer[bank][cmd.data & 0x7F]);
    fprintf(stdout, ")\n");

    fprintf(stdout, "        Module manufactur. date: ");
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x78;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "Year: %d ", ((cmd.data & 0xF0)>>4)*10 + (cmd.data & 0x0F) + 2000);
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x79;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    fprintf(stdout, "Week: %d%d\n", (cmd.data & 0xF0)>>4, cmd.data & 0x0F);

    fprintf(stdout, "        Module serial number   : ");

    /* Construct module serial number */
    for (it=0x7A; it<0x7E; it++)
    {
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=spd;
        cmd.smbus_cmd=it;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            return -1;
        }
        fprintf(stdout, "%02X", cmd.data);
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "        Module part number     : ");
    for(it=0x80; it<0x92; it++)
    {
        cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
        cmd.slv_addr=spd;
        cmd.smbus_cmd=it;
        cmd.length=0x00;
        status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
        if (status!=EOK)
        {
            return -1;
        }
        if (cmd.data==0x00)
        {
            break;
        }
        if ((cmd.data>=0x20) && (cmd.data<=0x7E))
        {
            fprintf(stdout, "%c", cmd.data);
        }
        else
        {
            fprintf(stdout, "_");
        }
    }
    fprintf(stdout, "\n");

    /* Read DRAM manufacturer */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x94;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }
    bank=cmd.data & 0x7F;

    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x95;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        DRAM manufacturer ID   : ");
    if (cmd.data==0)
    {
        fprintf(stdout, "Not supported\n");
    }
    else
    {
        fprintf(stdout, "Bank: %d, ID: 0x%02X (", bank, cmd.data);
        fprintf(stdout, "%s", jedec_manufacturer[bank][cmd.data & 0x7F]);
    }

    return 0;
}

int print_spd(uint8_t spd)
{
    fprintf(stdout, "    Found SPD EEPROM at address 0x%02X\n", spd);

    /* Check memory type */
    cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
    cmd.slv_addr=spd;
    cmd.smbus_cmd=0x02;
    cmd.length=0x00;
    status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
    if (status!=EOK)
    {
        return -1;
    }

    fprintf(stdout, "        Memory module type     : ");

    switch(cmd.data)
    {
        case 1:
             fprintf(stdout, "FPM DRAM\n");
             break;
        case 2:
             fprintf(stdout, "EDO\n");
             break;
        case 3:
             fprintf(stdout, "Pipelined Nibble\n");
             break;
        case 4:
             fprintf(stdout, "SDRAM\n");
             decode_sdr(spd);
             break;
        case 5:
             fprintf(stdout, "ROM\n");
             break;
        case 6:
             fprintf(stdout, "DDR SGRAM\n");
             break;
        case 7:
             fprintf(stdout, "DDR SDRAM\n");
             decode_ddr(spd);
             break;
        case 8:
             fprintf(stdout, "DDR2 SDRAM\n");
             decode_ddr2(spd);
             break;
        case 9:
             fprintf(stdout, "DDR2 SDRAM FB-DIMM\n");
             break;
        case 10:
             fprintf(stdout, "DDR2 SDRAM FB-DIMM PROBE\n");
             break;
        case 11:
             fprintf(stdout, "DDR3 SDRAM\n");
             decode_ddr3(spd);
             break;
        case 0:
        default:
             fprintf(stdout, "Unknown\n");
             break;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int smbus_id;
    int found_smbus=0;
    int spd;

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

        for (spd=0x50; spd<0x58; spd++)
        {
            /* Check if device is available  */
            cmd.cmd_type=SMBUS_CMD_BYTE_DATA;
            cmd.slv_addr=spd;
            cmd.smbus_cmd=0x00;
            cmd.length=0x00;
            status=devctl(fd, DCMD_SMBUS_READ, &cmd,  sizeof(cmd), NULL);
            if (status==EOK)
            {
                if (print_spd(spd)==-1)
                {
                    fprintf(stdout, "\n");
                    fprintf(stderr, "I/O error occured\n");
                }
            }
        }
    }

    exit(0);
}
