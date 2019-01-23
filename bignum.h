//
//  bignum.h
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#ifndef bignum_h
#define bignum_h

#include "includes.h"

#define GHN(b) (((b) >> 4) & 0x0F) /* get high nibble */
#define GLN(b) ((b) & 0x0F) /* get low nibble */

#define BYTE(high, low) (((high) << 4) | low & 0x0F) /* nibble to byte */

#define SHN(b, n) (((n) << 4) | GLN(b)) /* set high nibble */
#define SLN(b, n) (n | GHN(b)) /* set low nibble */

#define NINB(n) (((n) >> 1) + 1) /* nibbles in bytes */

#define BIDX(n) ((n) >> 1) /* byte index */

#define ALLOC_PADDING 1

typedef uint8_t byte;

typedef struct _big_num {
    size_t _alloc_size;
    size_t _length; /* max 9,223,372,036,854,775,807 digits. */
    byte * _nums;
} BIGNUM;

BIGNUM * bn_new(void);
void bn_stob(BIGNUM * _dest, char * _source); /* string to bignum */
char * bn_btos(BIGNUM * _source);
size_t bn_len(BIGNUM * _dest);
void bn_print(FILE * _stream, BIGNUM * _num);

void bn_add(BIGNUM * _dest, BIGNUM * _source);

void bn_realloc(BIGNUM * _num, size_t _size);
void bn_free(BIGNUM * _num);


#endif /* bignum_h */
