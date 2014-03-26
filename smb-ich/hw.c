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

#include <stdint.h>
#include <sys/slog.h>
#include <sys/mman.h>
#include <hw/pci.h>
#include <hw/pci_devices.h>
#include "proto.h"

typedef struct smbus_pci_device
{
    uint16_t    vid;
    uint16_t    did;
    const char* name;
} smbus_pci_device_t;

void write_smbus(ich_dev_t* dev, uint8_t addr, uint8_t data)
{
    if (dev->base_mmio!=NULL)
    {
        /* Do MMIO access */
        dev->base_mmio[addr]=data;
    }
    else
    {
        /* Do PIO access */
        out8(dev->base + addr, data);
    }

    return;
}

uint8_t read_smbus(ich_dev_t* dev, uint8_t addr)
{
    if (dev->base_mmio!=NULL)
    {
        /* Do MMIO access */
        return dev->base_mmio[addr];
    }
    else
    {
        /* Do PIO access */
        return in8(dev->base + addr);
    }
}

smbus_pci_device_t supported_devices[]=
{
    {PCI_VENDOR_ID_INTEL, 0x1C22, "PCH C200"},          /* PCH C200         */
    {PCI_VENDOR_ID_INTEL, 0x1D22, "PCH X79"},           /* PCH X79          */
    {PCI_VENDOR_ID_INTEL, 0x1D70, "PCH X79"},           /* PCH X79          */
    {PCI_VENDOR_ID_INTEL, 0x1D71, "Patsburg"},          /* Patsburg         */
    {PCI_VENDOR_ID_INTEL, 0x1D72, "Patsburg"},          /* Patsburg         */
    {PCI_VENDOR_ID_INTEL, 0x1E22, "Panther Point"},     /* Panther Point    */
    {PCI_VENDOR_ID_INTEL, 0x1F3C, "Avoton"},            /* Avoton (SOC)     */
    {PCI_VENDOR_ID_INTEL, 0x2330, "DH89xxCC"},          /* DH89xxCC         */
    {PCI_VENDOR_ID_INTEL, 0x23B0, "Coleto Creek"},      /* Coleto Creek     */
    {PCI_VENDOR_ID_INTEL, 0x24D3, "ICH5"},              /* ICH5             */
    {PCI_VENDOR_ID_INTEL, 0x25A4, "ESB 6300"},          /* ESB 6300         */
    {PCI_VENDOR_ID_INTEL, 0x266A, "ICH6"},              /* ICH6             */
    {PCI_VENDOR_ID_INTEL, 0x269B, "ESB 63x0"},          /* ESB 63x0         */
    {PCI_VENDOR_ID_INTEL, 0x27DA, "ICH7/NM10"},         /* NM10, ICH7       */
    {PCI_VENDOR_ID_INTEL, 0x283E, "ICH8"},              /* ICH8             */
    {PCI_VENDOR_ID_INTEL, 0x2930, "ICH9"},              /* ICH9             */
    {PCI_VENDOR_ID_INTEL, 0x3A30, "ICH10"},             /* ICH10            */
    {PCI_VENDOR_ID_INTEL, 0x3A60, "ICH10"},             /* ICH10            */
    {PCI_VENDOR_ID_INTEL, 0x3B30, "PCH 3400"},          /* PCH 3400         */
    {PCI_VENDOR_ID_INTEL, 0x5032, "EP80579"},           /* EP80579          */
    {PCI_VENDOR_ID_INTEL, 0x8C22, "Lynx Point"},        /* Lynx Point       */
    {PCI_VENDOR_ID_INTEL, 0x8D22, "Wellsburg"},         /* Wellsburg        */
    {PCI_VENDOR_ID_INTEL, 0x8D7D, "Wellsburg"},         /* Wellsburg        */
    {PCI_VENDOR_ID_INTEL, 0x8D7E, "Wellsburg"},         /* Wellsburg        */
    {PCI_VENDOR_ID_INTEL, 0x8D7F, "Wellsburg"},         /* Wellsburg        */
    {PCI_VENDOR_ID_INTEL, 0x9C22, "Lynx Point-LP"},     /* Lynx Point-LP    */
    {PCI_VENDOR_ID_INTEL, 0x9CA2, "Wildcat Point-LP"},  /* Wildcat Point-LP */
    {0x0000             , 0x0000, ""}
};

int ich_init(ich_dev_t* dev, int mode)
{
    struct pci_dev_info inf;
    int                 pci_handle;
    void*               dev_handle;
    uint32_t            smbus_base=0x00000000;
    uint64_t            smbus_base_mmio=0x0000000000000000;
    uint8_t             hostc;
    int                 pciid;

    pci_handle = pci_attach(0);
    if (pci_handle < 0)
    {
        fprintf(stderr, "[smb-ich] Can't attach to PCI bus\n");
        return -1;
    }

    pciid=0;
    do {
        if (supported_devices[pciid].vid==0x0000)
        {
            fprintf(stderr, "[smb-ich] Can't find a compatible SMBus PCI controller\n");
            pci_detach(pci_handle);
            return -1;
        }
        memset(&inf, 0, sizeof (inf));
        inf.VendorId = supported_devices[pciid].vid;
        inf.DeviceId = supported_devices[pciid].did;
        dev_handle = pci_attach_device(NULL, 0, 0, &inf);
        if (dev_handle == NULL)
        {
            pciid++;
            continue;
        }
        break;
    } while(1);

    /* Try to get MMIO address of SMBus controller */
    pci_read_config(dev_handle, ICH_SMBUS_BAR0, 1, 4, &smbus_base_mmio);
    pci_read_config(dev_handle, ICH_SMBUS_BAR1, 1, 4, ((uint8_t*)&smbus_base_mmio)+4);
    if (!PCI_IS_MEM(smbus_base_mmio)||(PCI_MEM_ADDR(smbus_base_mmio)==0x0000000000000000))
    {
        /* This device doesn't provide MMIO access to SMBus controller */
        smbus_base_mmio=0x0000000000000000;
        dev->base_mmio_phys=0x0000000000000000;
        dev->base_mmio=NULL;
    }
    else
    {
        /* MMIO address is present, try to mmap it */
        dev->base_mmio_phys=smbus_base_mmio;
        if (PCI_IS_MMAP64(smbus_base_mmio))
        {
            /* mmap 64 bit address, 256 bytes */
            dev->base_mmio=mmap64(NULL, 256, PROT_READ | PROT_WRITE | PROT_NOCACHE,
                MAP_SHARED | MAP_PHYS, NOFD, PCI_MEM_ADDR((smbus_base_mmio)));
        }
        else
        {
            /* mmap 32 bit address, 256 bytes */
            dev->base_mmio=mmap(NULL, 256, PROT_READ | PROT_WRITE | PROT_NOCACHE,
                MAP_SHARED | MAP_PHYS, NOFD, (uint32_t)PCI_MEM_ADDR((smbus_base_mmio)));
        }

        /* Check if memory region has been actualy mmaped */
        if (dev->base_mmio==MAP_FAILED)
        {
            smbus_base_mmio=0x0000000000000000;
            dev->base_mmio_phys=0x0000000000000000;
            dev->base_mmio=NULL;
        }
    }

    /* Get SMBus base i/o address */
    pci_read_config(dev_handle, ICH_SMBUS_BASE, 1, sizeof(smbus_base), &smbus_base);
    if (!(PCI_IS_IO(smbus_base)))
    {
        /* PCI resource is not an i/o, terminate execution */
        fprintf(stderr, "[smb-ich] Unexpected PCI device resource type\n");
        pci_detach_device(dev_handle);
        pci_detach(pci_handle);
        return -1;
    }
    else
    {
        smbus_base=PCI_IO_ADDR(smbus_base);
    }

    /* Check and enable SMBus interface, if it was not enabled */
    pci_read_config(dev_handle, ICH_SMBUS_HOSTC, 1, sizeof(hostc), &hostc);
    if ((hostc & ICH_SMBUS_HOSTC_SMBUS_HOST_EN)!=ICH_SMBUS_HOSTC_SMBUS_HOST_EN)
    {
        hostc|=ICH_SMBUS_HOSTC_SMBUS_HOST_EN;
        hostc&=~(ICH_SMBUS_HOSTC_I2C_EN);
        pci_write_config(dev_handle, ICH_SMBUS_HOSTC, 1, sizeof(hostc), &hostc);
    }

    pci_detach_device(dev_handle);
    pci_detach(pci_handle);

    dev->base = smbus_base;

    /* Initialize SMBUS controller */
    write_smbus(dev, ICH_SMBUS_HST_STS, NSHS_INUSE_STS | NSHS_BUS_ERR |
        NSHS_FAILED | NSHS_DEV_ERR | NSHS_INTR);

    slogf(_SLOGC_ICH_SMBUS, _SLOG_INFO, "[smb-ich] SMBUS communication driver for Intel ICH/PCH");
    slogf(_SLOGC_ICH_SMBUS, _SLOG_INFO, "[smb-ich] SMBus controller: %s", supported_devices[pciid].name);
    slogf(_SLOGC_ICH_SMBUS, _SLOG_INFO, "[smb-ich] I/O port: 0x%04X", dev->base);
    if (dev->base_mmio_phys!=0x0000000000000000)
    {
        if (PCI_IS_MMAP64(smbus_base_mmio))
        {
            slogf(_SLOGC_ICH_SMBUS, _SLOG_INFO, "[smb-ich] MMIO address: %016llX (64 bit)", PCI_MEM_ADDR(smbus_base_mmio));
        }
        else
        {
            slogf(_SLOGC_ICH_SMBUS, _SLOG_INFO, "[smb-ich] MMIO address: %08X (32 bit)", (uint32_t)PCI_MEM_ADDR(smbus_base_mmio));
        }
    }
    else
    {
        slogf(_SLOGC_ICH_SMBUS, _SLOG_INFO, "[smb-ich] MMIO address: n/a");
    }

    return 0;
}
