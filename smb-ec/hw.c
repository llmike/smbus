/*
 * $QNXLicenseC:
 * Copyright 2011-2012, QNX Software Systems.
 * Copyright 2011-2013, Mike Gorchak.
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
#include <stdint.h>
#include <sys/slog.h>
#include <sys/mman.h>
#include <hw/pci.h>
#include <hw/pci_devices.h>

#include "proto.h"
#include "smbus_ec.h"
#include "dmi.h"

typedef struct _productdb
{
    const char* manufacturer;
    const char* product;
    int         offset;
} productdb_t;

/* HP Compaq CQ56 uses offset 0x0000, but I do not know exact DMI strings */
/* Acer Aspire 4720Z uses offset 0x0018, but I do not sure about spaces   */
/* at the end of each string                                              */

productdb_t product_database[]=
{
    {"ASUSTeK Computer INC.",             "1215N",            0x0000},
    {"Sony Corporation",                  "VPCW22Z1R",        0x0018},
    {"FUJITSU",                           "LIFEBOOK AH531",   0x0018},
    {"MICRO-STAR INTERNATIONAL CO., LTD", "U90/U100",         0x0000},
    {"Acer      ",                        "Aspire 4720Z    ", 0x0018},
    {"",                                  "",                 -1}
};

int ec_init(ec_dev_t* dev, int mode)
{
    char manufacturer[1024];
    char product[1024];
    int deviceid=0;

    nanospin_calibrate(0);

    slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] SMBUS communication driver for ACPI Embedded Controller");

    /* Try to find DMI signature */
    if (find_product_name(manufacturer, product)==0)
    {
        fprintf(stderr, "[smb-ec] Can't find DMI BIOS signature, using defaults\n");
    }
    else
    {
        do {
            if (product_database[deviceid].offset==-1)
            {
                deviceid=-1;
                break;
            }
            if (strcmp(product_database[deviceid].manufacturer, manufacturer)==0)
            {
                if (strcmp(product_database[deviceid].product, product)==0)
                {
                    dev->smbus_offset=product_database[deviceid].offset;
                    slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] Found %s/%s, offset 0x%02X", manufacturer, product, product_database[deviceid].offset);
                    break;
                }
            }
            deviceid++;
        } while(1);
    }

    if (deviceid==-1)
    {
        int empty_manufacturer=1;
        int empty_product=1;
        int it;

        for (it=0; it<strlen(manufacturer); it++)
        {
            if ((manufacturer[it]!=0x00) && (manufacturer[it]!=0x20))
            {
                empty_manufacturer=0;
                break;
            }
        }

        for (it=0; it<strlen(product); it++)
        {
            if ((product[it]!=0x00) && (product[it]!=0x20))
            {
                empty_product=0;
                break;
            }
        }
        if ((!empty_manufacturer) && (!empty_product))
        {
            slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] Found unlisted %s/%s, please report", manufacturer, product);
        }
        else
        {
            if ((empty_manufacturer) && (empty_product))
            {
                slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] Your system is unidentificable due to empty DMI strings");
            }
            else
            {
                if (empty_manufacturer)
                {
                    slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] Your system manufacturer is unidentificable, product name: %s", product);
                }
                else
                {
                    slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] Your system product name is unidentificable, manufacturer: %s", manufacturer);
                }
            }
        }
    }

    /* Check if autodetect must be used */
    if (dev->smbus_offset==-1)
    {
        /* Try 0x0000 first */
        dev->smbus_offset=0x0000;

        /* Store pattern to SMBus data registers */
        write_smbus(dev, SMB_DATA0, 0x55);
        write_smbus(dev, SMB_DATA1, 0xAA);
        write_smbus(dev, SMB_DATA2, 0x33);
        write_smbus(dev, SMB_DATA3, 0xCC);

        /* Check for stored pattern */
        if ((read_smbus(dev, SMB_DATA0)!=0x55) || (read_smbus(dev, SMB_DATA1)!=0xAA) ||
            (read_smbus(dev, SMB_DATA2)!=0x33) || (read_smbus(dev, SMB_DATA3)!=0xCC))
        {
            /* Try 0x0018 then */
            dev->smbus_offset=0x0018;

            /* Store pattern to SMBus data registers */
            write_smbus(dev, SMB_DATA0, 0x55);
            write_smbus(dev, SMB_DATA1, 0xAA);
            write_smbus(dev, SMB_DATA2, 0x33);
            write_smbus(dev, SMB_DATA3, 0xCC);

            if ((read_smbus(dev, SMB_DATA0)!=0x55) || (read_smbus(dev, SMB_DATA1)!=0xAA) ||
                (read_smbus(dev, SMB_DATA2)!=0x33) || (read_smbus(dev, SMB_DATA3)!=0xCC))
            {
                return -1;
            }
        }

        slogf(_SLOGC_EC_SMBUS, _SLOG_INFO, "[smb-ec] Autodetected offset is 0x%02X", dev->smbus_offset);

        return 0;
    }

    /* Store pattern to SMBus data registers */
    write_smbus(dev, SMB_DATA0, 0x55);
    write_smbus(dev, SMB_DATA1, 0xAA);
    write_smbus(dev, SMB_DATA2, 0x33);
    write_smbus(dev, SMB_DATA3, 0xCC);

    /* Check for stored pattern */
    if ((read_smbus(dev, SMB_DATA0)!=0x55) || (read_smbus(dev, SMB_DATA1)!=0xAA) ||
        (read_smbus(dev, SMB_DATA2)!=0x33) || (read_smbus(dev, SMB_DATA3)!=0xCC))
    {
        return -1;
    }

    return 0;
}
