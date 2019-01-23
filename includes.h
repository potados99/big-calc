//
//  includes.h
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#ifndef includes_h
#define includes_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* not portable yet :) */

/* definition for boolean type. */
typedef int BOOL;
#define TRUE (1)
#define FALSE (0)

#define ERROR(msg) fprintf(stderr, msg "\n")
#define ERBLK(msg) { ERROR(msg); } /* error block */
#define ERTNBK(msg) {  }

#endif /* includes_h */
