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
#include <assert.h>
/* not portable yet :) */

typedef int BOOL;
#define TRUE (1)
#define FALSE (0)

#define ALLOC_PADDING 1

#define ERROR(msg)                      \
fprintf(stderr, msg "\n")

#define ERBLK(msg)                      \
do {                                    \
ERROR(msg);                             \
} while(0)

#endif /* _includes_h */
