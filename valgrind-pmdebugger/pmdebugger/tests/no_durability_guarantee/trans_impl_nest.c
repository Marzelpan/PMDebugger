/*
 * Persistent memory checker.
 * Copyright (c) 2015, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, or (at your option) any later version, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
#include "../common.h"
#include <stdint.h>

#define FILE_SIZE (16 * 1024 * 1024)

int main ( void )
{
    /* make, map and register a temporary file */
    void *base = make_map_tmpfile(FILE_SIZE);

    int8_t *i8p = base;
    int16_t *i16p = (int16_t *)((uintptr_t)base + 8);
    int32_t *i32p = (int32_t *)((uintptr_t)base + 16);
    int64_t *i64p = (int64_t *)((uintptr_t)base + 24);

    *i8p = 1;

    VALGRIND_PMC_START_TX;
    VALGRIND_PMC_ADD_TO_TX(i32p, sizeof (*i32p));

    VALGRIND_PMC_START_TX;
    VALGRIND_PMC_ADD_TO_TX(i16p, sizeof (*i16p));

    /* dirty stores */
    *i8p = 1;
    *i16p = 2;
    *i32p = 3;
    *i64p = 4;

    VALGRIND_PMC_END_TX;

    VALGRIND_PMC_WRITE_STATS;

    /* still in the same transaction */
    *i16p = 2;

    VALGRIND_PMC_END_TX;

    *i16p = 5;

    return 0;
}
