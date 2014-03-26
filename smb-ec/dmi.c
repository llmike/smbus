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
#include <string.h>
#include <sys/mman.h>

#include "dmi.h"
#include "proto.h"

int find_product_name(char* manufacturer, char* product)
{
    void*                bios_ptr;
    uint8_t*             bios_ptr8;
    uint32_t*            bios_ptr32;
    int                  it;
    dmi_header_t*        dmi_hdr;
    dmi_struct_header_t* hdr;

    bios_ptr = mmap(0, 0x00020000, PROT_READ, MAP_PHYS | MAP_SHARED, NOFD, 0x000E0000);
    if (bios_ptr==MAP_FAILED)
    {
        fprintf(stderr, "[smb-ec] Can't mmap BIOS region\n");
        return 0;
    }

    bios_ptr32=(uint32_t*)bios_ptr;
    for (it=0; it<0x00020000/4; it++)
    {
        /* Check for "_DMI" string */
        if (*(bios_ptr32+it)==0x494D445F)
        {
            bios_ptr8=(uint8_t*)(bios_ptr32+it);
            if (*(bios_ptr8+4)=='_')
            {
                int     jt;
                uint8_t checksum=0x00;

                /* Check checksum */
                for (jt=0; jt<15; jt++)
                {
                    checksum+=*(bios_ptr8+jt);
                }
                if (checksum==0)
                {
                    break;
                }
            }
        }
    }

    /* _DMI_ was not found */
    if (it==0x00010000/4)
    {
        fprintf(stderr, "[smb-ec] Can't locate DMI in BIOS data\n");
        munmap(bios_ptr, 0x00020000);
        return 0;
    }

    /* We have found correct DMI signature */
    dmi_hdr=(dmi_header_t*)bios_ptr8;
    if (dmi_hdr->table_ptr>=0x000E0000)
    {
        bios_ptr8=(uint8_t*)bios_ptr + dmi_hdr->table_ptr - 0x000E0000;
    }
    else
    {
        fprintf(stderr, "[smb-ec] Non-standard BIOS mapping has been found: %08X\n", dmi_hdr->table_ptr);
        munmap(bios_ptr, 0x00020000);
        return 0;
    }

    do {
        /* We have found table start */
        hdr=(dmi_struct_header_t*)bios_ptr8;

        /* We are searching for type 1 table, product information */
        if (hdr->type==1)
        {
            /* We have found a type 1 table */
            break;
        }
        if (hdr->type==127)
        {
            /* We have found a type 127 table, end of tables */
            munmap(bios_ptr, 0x00020000);
            return 0;
        }

        /* skip this table */
        bios_ptr8+=hdr->length;

        /* search end of table signature 0x0000 */
        do {
            if ((*(bios_ptr8+0)==0x00) && (*(bios_ptr8+1)==0x00))
            {
                bios_ptr8+=2;
                break;
            }
            bios_ptr8++;
        } while(1);
    } while(1);

    /* skip this table to the end */
    bios_ptr8+=hdr->length;

    /* Copy strings */
    strcpy(manufacturer, (char*)bios_ptr8);
    strcpy(product, (char*)bios_ptr8+strlen((char*)bios_ptr8)+0x01);

    /* Unmap and exit */
    munmap(bios_ptr, 0x00020000);

    return 1;
}
