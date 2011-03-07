/*
 * Copyright 2004 Timo Hirvonen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "config/utils.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#ifdef HAVE_BYTESWAP_H
#include <byteswap.h>
#endif

#define N_ELEMENTS(array) (sizeof(array) / sizeof((array)[0]))

#define getentry(ptr, offset, type) (*((type *) ((char *) (ptr) + (offset))))

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static inline int clamp(int val, int minval, int maxval)
{
	if (val < minval)
		return minval;
	if (val > maxval)
		return maxval;
	return val;
}

static inline int scale_from_percentage(int val, int max_val)
{
	if (val < 0)
		return (val * max_val - 50) / 100;
	return (val * max_val + 50) / 100;
}

static inline int scale_to_percentage(int val, int max_val)
{
	int half = max_val / 2;

	if (max_val <= 0)
		return 100;

	if (val < 0)
		return (val * 100 - half) / max_val;
	return (val * 100 + half) / max_val;
}

static inline int str_to_int(const char *str, long int *val)
{
	char *end;

	*val = strtol(str, &end, 10);
	if (*str == 0 || *end != 0)
		return -1;
	return 0;
}

static inline int strcmp0(const char *str1, const char *str2)
{
	if (!str1)
		return str2 ? -1 : 0;
	if (!str2)
		return 1;

	return strcmp(str1, str2);
}

static inline time_t file_get_mtime(const char *filename)
{
	struct stat s;

	/* stat follows symlinks, lstat does not */
	if (stat(filename, &s) == -1)
		return -1;
	return s.st_mtime;
}

static inline void ns_sleep(int ns)
{
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = ns;
	nanosleep(&req, NULL);
}

static inline void us_sleep(int us)
{
	ns_sleep(us * 1e3);
}

static inline void ms_sleep(int ms)
{
	ns_sleep(ms * 1e6);
}

static inline int is_url(const char *name)
{
	return strncmp(name, "http://", 7) == 0;
}

static inline int is_freeform_true(const char *c)
{
	return	c[0] == '1' ||
		c[0] == 'y' || c[0] == 'Y' ||
		c[0] == 't' || c[0] == 'T';
}

static inline uint16_t bswap16(uint16_t u)
{
#if defined(bswap_16)
	/* GNU libc */
	return bswap_16(u);
#elif defined(swap16)
	/* e.g. OpenBSD */
	return swap16(u);
#else
	return (u << 8) | (u >> 8);
#endif
}

static inline uint32_t bswap32(uint32_t u)
{
#if defined(bswap_32)
	/* GNU libc */
	return bswap_32(u);
#elif defined(swap32)
	/* e.g. OpenBSD */
	return swap32(u);
#else
	return (u >> 24) | ((u & 0xff0000) >> 8) | ((u & 0xff00) << 8) | (u << 24);
#endif
}

static inline uint32_t read_le32(const char *buf)
{
	const unsigned char *b = (const unsigned char *)buf;

	return b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
}

static inline uint16_t read_le16(const char *buf)
{
	const unsigned char *b = (const unsigned char *)buf;

	return b[0] | (b[1] << 8);
}

#endif
