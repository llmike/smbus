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

#ifndef __DMI_H__
#define __DMI_H__

#include <stdint.h>

typedef struct _dmi_header
{
    uint8_t   signature[5];
    uint8_t   checksum;
    uint16_t  table_length;
    uint32_t  table_ptr;
    uint16_t  number_of_structures;
    uint8_t   amount_smb_structs;
} dmi_header_t;

typedef struct _dmi_struct_header
{
    uint8_t   type;
    uint8_t   length;
    uint16_t  handle;
} dmi_struct_header_t;

int find_product_name(char* manufacturer, char* product);

#endif /* __DMI_H__ */
