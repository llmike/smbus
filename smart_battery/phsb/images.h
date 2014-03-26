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

#ifndef __IMAGES_H__
#define __IMAGES_H__

#define BATTERY_IMAGE_WIDTH  50
#define BATTERY_IMAGE_HEIGHT 25
#define BATTERY_IMAGE_FORMAT Pg_IMAGE_DIRECT_888

extern unsigned char empty_charge_image[BATTERY_IMAGE_HEIGHT][BATTERY_IMAGE_WIDTH*3];
extern unsigned char one_fourth_charge_image[BATTERY_IMAGE_HEIGHT][BATTERY_IMAGE_WIDTH*3];
extern unsigned char half_charge_image[BATTERY_IMAGE_HEIGHT][BATTERY_IMAGE_WIDTH*3];
extern unsigned char three_fourth_charge_image[BATTERY_IMAGE_HEIGHT][BATTERY_IMAGE_WIDTH*3];
extern unsigned char full_charge_image[BATTERY_IMAGE_HEIGHT][BATTERY_IMAGE_WIDTH*3];
extern unsigned char charging_image[BATTERY_IMAGE_HEIGHT][BATTERY_IMAGE_WIDTH*3];

#endif /* __IMAGES_H__ */
