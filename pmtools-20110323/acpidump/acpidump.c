/*
 * (c) Alexey Starikovskiy, Intel, 2005-2006.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>

#include <acpi/acconfig.h>
#include <acpi/platform/acenv.h>
#include <acpi/actypes.h>
#include <acpi/actbl.h>

#include "getopt_long.h"

static inline uint8_t checksum(uint8_t * buffer, uint32_t length)
{
	uint8_t sum = 0, *i = buffer;
	buffer += length;
	for (; i < buffer; sum += *(i++));
	return sum;
}

static unsigned long psz, addr, length;
static int print, connect, skip;
static uint8_t select_sig[4];

static uint8_t *acpi_map_memory(unsigned long where, unsigned length)
{
	unsigned long offset;
	uint8_t *there;
	int fd = open("/dev/mem", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "acpi_os_map_memory: cannot open /dev/mem\n");
		exit(1);
	}
	offset = where % psz;
	there = mmap(0, length + offset, PROT_READ, MAP_SHARED, fd, where - offset);
	close(fd);
	if (there == MAP_FAILED) return 0;
	return (there + offset);
}

static void acpi_unmap_memory(uint8_t * there, unsigned length)
{
	unsigned long offset = (unsigned long)there % psz;
	munmap(there - offset, length + offset);
}

static struct acpi_table_header *acpi_map_table(unsigned long where, char *sig)
{
	unsigned size;
	struct acpi_table_header *tbl = (struct acpi_table_header *)
	    acpi_map_memory(where, sizeof(struct acpi_table_header));
	if (!tbl || (sig && memcmp(sig, tbl->Signature, 4))) return 0;
	size = tbl->Length;
	acpi_unmap_memory((uint8_t *) tbl, sizeof(struct acpi_table_header));
	return (struct acpi_table_header *)acpi_map_memory(where, size);
}

static void acpi_unmap_table(struct acpi_table_header *tbl)
{
	acpi_unmap_memory((uint8_t *)tbl, tbl->Length);
}

static struct acpi_table_rsdp* acpi_scan_for_rsdp(uint8_t *begin, uint32_t length)
{
	struct acpi_table_rsdp* rsdp;
	uint8_t *i, *end = begin + length;
	/* Search from given start address for the requested length */
	for (i = begin; i < end; i += ACPI_RSDP_SCAN_STEP) {
		/* The signature and checksum must both be correct */
		if (memcmp((char *)i, "RSD PTR ", 8)) continue;
		rsdp = (struct acpi_table_rsdp*)i;
		/* Signature matches, check the appropriate checksum */
		if (!checksum((uint8_t *) rsdp, (rsdp->Revision < 2) ?
			      ACPI_RSDP_CHECKSUM_LENGTH :
			      ACPI_RSDP_XCHECKSUM_LENGTH))
			/* Checksum valid, we have found a valid RSDP */
			return rsdp;
	}
	/* Searched entire block, no RSDP was found */
	return 0;
}

/*
 * Output data
 */
static void acpi_show_data(int fd, uint8_t * data, int size)
{
	char buffer[256];
	int len;
	int i, remain = size;
	while (remain > 0) {
		len = snprintf(buffer, 256, "  %04x:", size - remain);
		for (i = 0; i < 16 && i < remain; i++) {
			len +=
			    snprintf(&buffer[len], 256 - len, " %02x", data[i]);
		}
		for (; i < 16; i++) {
			len += snprintf(&buffer[len], 256 - len, "   ");
		}
		len += snprintf(&buffer[len], 256 - len, "  ");
		for (i = 0; i < 16 && i < remain; i++) {
			buffer[len++] = (isprint(data[i])) ? data[i] : '.';
		}
		buffer[len++] = '\n';
		write(fd, buffer, len);
		data += 16;
		remain -= 16;
	}
}

/*
 * Output ACPI table
 */
static void acpi_show_table(int fd, struct acpi_table_header *table, unsigned long addr)
{
	char buff[80];
	int len = snprintf(buff, 80, "%.4s @ %p\n", table->Signature, (void *)addr);
	write(fd, buff, len);
	acpi_show_data(fd, (uint8_t *) table, table->Length);
	buff[0] = '\n';
	write(fd, buff, 1);
}

static void write_table(int fd, struct acpi_table_header *tbl, unsigned long addr)
{
	static int select_done = 0;
	if (!select_sig[0]) {
		if (print) {
			acpi_show_table(fd, tbl, addr);
		} else {
			write(fd, tbl, tbl->Length);
		}
	} else if (!select_done && !memcmp(select_sig, tbl->Signature, 4)) {
		if (skip > 0) {
			--skip;
			return;
		}
		if (print) {
			acpi_show_table(fd, tbl, addr);
		} else {
			write(fd, tbl, tbl->Length);
		}
		select_done = 1;
	}
}

static void acpi_dump_FADT(int fd, struct acpi_table_header *tbl, unsigned long xaddr) {
	struct acpi_table_fadt x;
	unsigned long addr;
	size_t len = sizeof(struct acpi_table_fadt);
	if (len > tbl->Length) len = tbl->Length;
	memcpy(&x, tbl, len);
	x.Header.Length = len;
	if (checksum((uint8_t *)tbl, len)) {
		fprintf(stderr, "Wrong checksum for FADT!\n");
	}
	if (x.Header.Length >= 148 && x.XDsdt) {
		addr = (unsigned long)x.XDsdt;
		if (connect) {
			x.XDsdt = lseek(fd, 0, SEEK_CUR);
		}
	} else if (x.Header     .Length >= 44 && x.Dsdt) {
		addr = (unsigned long)x.Dsdt;
		if (connect) {
			x.Dsdt = lseek(fd, 0, SEEK_CUR);
		}
	} else {
		fprintf(stderr, "No DSDT in FADT!\n");
		goto no_dsdt;
	}
	tbl = acpi_map_table(addr, ACPI_SIG_DSDT);
	if (!tbl) goto no_dsdt;
	if (checksum((uint8_t *)tbl, tbl->Length))
		fprintf(stderr, "Wrong checksum for DSDT!\n");
	write_table(fd, tbl, addr);
	acpi_unmap_table(tbl);
no_dsdt:
	if (x.Header.Length >= 140 && x.XFacs) {
		addr = (unsigned long)x.XFacs;
		if (connect) {
			x.XFacs = lseek(fd, 0, SEEK_CUR);
		}
	} else if (x.Header.Length >= 40 && x.XFacs) {
		addr = (unsigned long)x.XFacs;
		if (connect) {
			x.XFacs = lseek(fd, 0, SEEK_CUR);
		}
	} else {
		fprintf(stderr, "No FACS in FADT!\n");
		goto no_facs;
	}
	tbl = acpi_map_table(addr, ACPI_SIG_FACS);
	if (!tbl) goto no_facs;
	/* do not checksum FACS */
	write_table(fd, tbl, addr);
	acpi_unmap_table(tbl);
no_facs:
	write_table(fd, (struct acpi_table_header *)&x, xaddr);
}

static int acpi_dump_SDT(int fd, struct acpi_table_rsdp* rsdp)
{
	struct acpi_table_header *sdt, *tbl = 0;
	int xsdt = 1, i, num;
	char *offset;
	unsigned long addr;
	if (rsdp->Revision > 1 && rsdp->XsdtPhysicalAddress) {
		tbl = acpi_map_table(rsdp->XsdtPhysicalAddress, "XSDT");
	}
	if (!tbl && rsdp->RsdtPhysicalAddress) {
		xsdt = 0;
		tbl = acpi_map_table(rsdp->RsdtPhysicalAddress, "RSDT");
	}
	if (!tbl) return 0;
	sdt = malloc(tbl->Length);
	memcpy(sdt, tbl, tbl->Length);
	acpi_unmap_table(tbl);
	if (checksum((uint8_t *)sdt, sdt->Length))
		fprintf(stderr, "Wrong checksum for %s!\n", (xsdt)?"XSDT":"RSDT");
	num = (sdt->Length - sizeof(struct acpi_table_header))/((xsdt)?sizeof(uint64_t):sizeof(uint32_t));
	offset = (char *)sdt + sizeof(struct acpi_table_header);
	for (i = 0; i < num; ++i, offset += ((xsdt) ? sizeof(uint64_t) : sizeof(uint32_t))) {
		addr = (xsdt) ? (unsigned long)(*(uint64_t *)offset):
				(unsigned long)(*(uint32_t *)offset);
		if (!addr) continue;
		tbl = acpi_map_table(addr, 0);
		if (!tbl) continue;
		if (!memcmp(tbl->Signature, ACPI_SIG_FADT, 4)) {
			acpi_dump_FADT(fd, tbl, addr);
		} else {
			if (checksum((uint8_t *)tbl, tbl->Length))
				fprintf(stderr, "Wrong checksum for generic table!\n");
			write_table(fd, tbl, addr);
		}
		acpi_unmap_table(tbl);
		if (connect) {
			if (xsdt)
				(*(uint64_t*)offset) = lseek(fd, 0, SEEK_CUR);
			else
				(*(uint32_t*)offset) = lseek(fd, 0, SEEK_CUR);
		}
	}
	if (xsdt) {
		addr = (unsigned long)rsdp->XsdtPhysicalAddress;
		if (connect) {
			rsdp->XsdtPhysicalAddress = lseek(fd, 0, SEEK_CUR);
		}
	} else {
		addr = (unsigned long)rsdp->RsdtPhysicalAddress;
		if (connect) {
			rsdp->RsdtPhysicalAddress = lseek(fd, 0, SEEK_CUR);
		}
	}
	write_table(fd, sdt, addr);
	free (sdt);
	return 1;
}

static void usage(const char *progname)
{
	puts("Usage:");
	printf("%s [--addr 0x1234][--table DSDT][--output filename]"
		"[--binary][--length 0x456][--help]\n", progname);
	puts("\t--addr 0x1234 or -a 0x1234 -- look for tables at this physical address");
	puts("\t--table DSDT or -t DSDT -- only dump table with DSDT signature");
	puts("\t--output filename or -o filename -- redirect output from stdin to filename");
	puts("\t--binary or -b -- dump data in binary form rather than in hex-dump format");
	puts("\t--length 0x456 or -l 0x456 -- works only with --addr, dump physical memory"
		"\n\t\tregion without trying to understand it's contents");
	puts("\t--skip 2 or -s 2 -- skip 2 tables of the given name and output only 3rd one");
	puts("\t--help or -h -- this help message");
	exit(0);
}

static struct option long_options[] = {
	{"addr", 1, 0, 0},
	{"table", 1, 0, 0},
	{"output", 1, 0, 0},
	{"binary", 0, 0, 0},
	{"length", 1, 0, 0},
	{"skip", 1, 0, 0},
	{"help", 0, 0, 0},
	{0, 0, 0, 0}
};
int main(int argc, char **argv)
{
	int option_index, c, fd;
	uint8_t *raw;
	struct acpi_table_rsdp rsdpx, *x = 0;
	char *filename = 0;
	char buff[80];
	memset(select_sig, 0, 4);
	print = 1;
	connect = 0;
	addr = 0;
	length = 0;
	skip = 0;
	while (1) {
		option_index = 0;
		c = getopt_long(argc, argv, "a:t:o:bl:s:h",
				    long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			switch (option_index) {
			case 0:
				addr = strtoul(optarg, (char **)NULL, 16);
				break;
			case 1:
				memcpy(select_sig, optarg, 4);
				break;
			case 2:
				filename = optarg;
				break;
			case 3:
				print = 0;
				break;
			case 4:
				length = strtoul(optarg, (char **)NULL, 16);
				break;
			case 5:
				skip = strtoul(optarg, (char **)NULL, 10);
				break;
			case 6:
				usage(argv[0]);
				exit(0);
			}
			break;
		case 'a':
			addr = strtoul(optarg, (char **)NULL, 16);
			break;
		case 't':
			memcpy(select_sig, optarg, 4);
			break;
		case 'o':
			filename = optarg;
			break;
		case 'b':
			print = 0;
			break;
		case 'l':
			length = strtoul(optarg, (char **)NULL, 16);
			break;
		case 's':
			skip = strtoul(optarg, (char **)NULL, 10);
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			printf("Unknown option!\n");
			usage(argv[0]);
			exit(0);
		}
	}

	fd = STDOUT_FILENO;
	if (filename) {
		fd = creat(filename, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
		if (fd < 0)
			return fd;
	}

	if (!select_sig[0] && !print) {
		connect = 1;
	}

	psz = sysconf(_SC_PAGESIZE);
	if (length && addr) {
		/* We know length and address, it means we just want a memory dump */
		if (!(raw = acpi_map_memory(addr, length)))
			goto not_found;
		write(fd, raw, length);
		acpi_unmap_memory(raw, length);
		close(fd);
		return 0;
	}

	length = sizeof(struct acpi_table_rsdp);
	if (!addr) {
		addr = ACPI_HI_RSDP_WINDOW_BASE;
		length = ACPI_HI_RSDP_WINDOW_SIZE;
	}

	if (!(raw = acpi_map_memory(addr, length)) ||
	    !(x = acpi_scan_for_rsdp(raw, length)))
		goto not_found;

	/* Find RSDP and print all found tables */
	memcpy(&rsdpx, x, sizeof(struct acpi_table_rsdp));
	acpi_unmap_memory(raw, length);
	if (connect) {
		lseek(fd, sizeof(struct acpi_table_rsdp), SEEK_SET);
	}
	if (!acpi_dump_SDT(fd, &rsdpx))
		goto not_found;
	if (connect) {
		lseek(fd, 0, SEEK_SET);
		write(fd, x, (rsdpx.Revision < 2) ?
			ACPI_RSDP_CHECKSUM_LENGTH : ACPI_RSDP_XCHECKSUM_LENGTH);
	} else if (!select_sig[0] || !memcmp("RSD PTR ", select_sig, 4)) {
		addr += (long)x - (long)raw;
		length = snprintf(buff, 80, "RSD PTR @ %p\n", (void *)addr);
		write(fd, buff, length);
		acpi_show_data(fd, (uint8_t *) & rsdpx, (rsdpx.Revision < 2) ?
				ACPI_RSDP_CHECKSUM_LENGTH : ACPI_RSDP_XCHECKSUM_LENGTH);
		buff[0] = '\n';
		write(fd, buff, 1);
	}
	close(fd);
	return 0;
not_found:
	close(fd);
	fprintf(stderr, "ACPI tables were not found. If you know location "
		"of RSD PTR table (from dmesg, etc), "
		"supply it with either --addr or -a option\n");
	return 1;
}
