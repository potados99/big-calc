//
//  includes.h
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#ifndef _includes_h
#define _includes_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "debug.h"

typedef int BOOL;

#define TRUE (1)
#define FALSE (0)

#define ALLOC_PADDING 1 /* added to size of memory to be allocated. */

/**
 * Iterate array.
 *
 * @param _item_pointer_decl    Declaration for each item.
 * @param _array                Array to iterate.
 */
#define foreach(_item_pointer_decl, _array) \
for(int keep = 1, count = 0, size = sizeof (_array) / sizeof *(_array); \
keep && count != size; \
keep = !keep, count++) \
for(_item_pointer_decl = (_array) + count; keep; keep = !keep)

#endif /* _includes_h */
