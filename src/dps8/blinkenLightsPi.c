#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

/*
 * fbtestfnt.h
 *
 * 
 *
 * Original work by J-P Rosti (a.k.a -rst- and 'Raspberry Compote')
 *
 * Licensed under the Creative Commons Attribution 3.0 Unported License
 * (http://creativecommons.org/licenses/by/3.0/deed.en_US)
 *
 * Distributed in the hope that this will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// Add'l font from http://www.piclist.com/techref/datafile/charset/8x8.htm

#define FONTW 8
#define FONTH 8

char fontImg[][FONTW * FONTH] = {
    { // ' ' (space)
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // !
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // "
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // # 
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // $ (TODO)
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // % (TODO)
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // & (TODO)
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 1, 0, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0
    },
    { // '
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // (
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // )
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // * (TODO)
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // +
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // ,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0
    },
    { // -
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // .
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // /
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    {  // 0 (zero)
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 1, 1, 0,
            0, 1, 0, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 0, 1, 0,
            0, 1, 0, 1, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // 1
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 1, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },

    { // 2
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // 3 
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // 4 
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // 5 
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // 6 
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // 7
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // 8
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    {  // 9
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // :
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // ;
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0
    },
    { // <
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // =
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // >
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // ?
            0, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
// Use @ for lamp on

//    { // @
//            0, 0, 1, 1, 1, 1, 0, 0,
//            0, 1, 0, 0, 0, 0, 1, 0,
//            1, 0, 0, 1, 1, 0, 1, 0,
//            1, 0, 1, 0, 1, 0, 1, 0,
//            1, 0, 0, 1, 1, 1, 0, 0,
//            0, 1, 0, 0, 0, 0, 1, 0,
//            0, 0, 1, 1, 1, 1, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0
//    },

    { // @
            0, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },

    { // A
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // B
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // C
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // D
            0, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // E
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // F
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    // G-Z (TODO)
    { // G
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // I
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // J
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // K
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 0,
            0, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 1, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0
    },
    { // L
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // M
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // N
            1, 0, 0, 0, 0, 0, 1, 0,
            1, 1, 0, 0, 0, 0, 1, 0,
            1, 0, 1, 0, 0, 0, 1, 0,
            1, 0, 0, 1, 0, 0, 1, 0,
            1, 0, 0, 0, 1, 0, 1, 0,
            1, 0, 0, 0, 0, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // O
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // P
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // Q
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 1, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 1, 1, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0  
    },
    { // R
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // S
            0, 1, 1, 1, 1, 1, 0, 0,
            1, 0, 0, 0, 0, 0, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0,
            1, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // T
            1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // U
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 0, 0, 1, 0,
            0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // V
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // W
            1, 0, 0, 0, 0, 0, 1, 0,
            1, 0, 0, 1, 0, 0, 1, 0,
            1, 0, 0, 1, 0, 0, 1, 0,
            1, 0, 0, 1, 0, 0, 1, 0,
            1, 0, 0, 1, 0, 0, 1, 0,
            1, 0, 0, 1, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // Z
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    // [\]^` (TODO)
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    },
    { // _
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    },
    { // ` (TODO)
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    }
    // a-z (TODO) ...
    // {|}~ (TODO) ...

};

// helper function to get the 'image'/'pixel map' index
// for a given character - returns 0 (space) if not found
int font_index(char a) {
    int ret = a - 32;
    // is the value in the 'printable' range (a >= 32) and 
    // within the defined entries
    if ( (ret >= 0) && ( ret < ( sizeof(fontImg) / (FONTW * FONTH) ) ) ) {
        ret = a - 32; // we start at zero
    }
    else {  // if not, return 0 (== space character)
        ret = 0;
    }
    return ret;
}

typedef unsigned short int pxl;

// 'global' variables to store screen info
static pxl *fbp = 0;
static int bytes_per_pixel = 2;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
static pxl bg_col = 0;
static pxl fg_col = 0xffff;

// helper function to 'plot' a pixel in given color
void put_pixel(int x, int y, pxl c)
{
    // calculate the pixel's byte offset inside the buffer
    unsigned int pix_offset = x + y * finfo.line_length/bytes_per_pixel;

    // now this is about the same as 'fbp[pix_offset] = value'
    *(fbp + pix_offset) = c;
}

// helper function to draw a rectangle in given color
void fill_rect(int x, int y, int w, int h, pxl c) {
    int cx, cy;
    for (cy = 0; cy < h; cy++) {
        for (cx = 0; cx < w; cx++) {
            put_pixel(x + cx, y + cy, c);
        }
    }
}

// helper function to clear the screen - fill whole
// screen with given color
void clear_screen(pxl c) {
    memset(fbp, c, vinfo.xres * vinfo.yres * sizeof (pxl));
}

static pxl grey = 0x8410;

// helper function for drawing - no more need to go mess with
// the main function when just want to change what to draw...
static void draw (int textX, int textY, char *arg)
  {

    textX *= FONTW;
    textY *= FONTH;

    char * text = arg;
    pxl textC = 0xffff;

    int i, l, x, y;

    // loop through all characters in the text string
    l = strlen (text);
    for (i = 0; i < l; i ++)
      {
        char ch = text [i];
        if (ch == 'n') // newline
          {
            textX = 0;
            textY += FONTH;
            continue;
          }
        pxl color = fg_col;
        if (ch == '+')
          {
            ch = '@';
          }
        else if (ch == '-')
          {
            ch = '@';
            color = bg_col;
          }
        // get the 'image' index for this character
        int ix = font_index (ch);
        // get the font 'image'
        char * img = fontImg [ix];
       // loop through pixel rows
        for (y = 0; y < FONTH; y ++)
          {
            // loop through pixel columns
            for (x = 0; x < FONTW; x ++)
              {
                // get the pixel value
                char b = img [y * FONTW + x];
                if (b > 0)
                  { // plot the pixel
                    put_pixel (textX + x, textY + y, color);
                  }
                else
                  {
                    put_pixel (textX + x, textY + y, grey);
                  }
              } // end "for x"
          } // end "for y"
        textX += FONTW;
      } // end "for i"
  }


#include "dps8.h"
#include "dps8_sys.h"
#include "dps8_faults.h"
#include "dps8_cpu.h"

#include "dps8_mp.h"
#include "shm.h"

static pid_t sid;
cpu_state_t * cpus;
cpu_state_t * cpun;



static char buf [128];
static void drawn (int n, word36 v, int x, int y)
  {
    char * p = buf;
    for (int i = n - 1; i >= 0; i --)
      {
        * p ++ = ((1llu << i) & v) ? '+' : '-';
      }
    * p ++ = 0;
    draw (x, y, buf);
  }

static void draw36 (word36 v, int x, int y)
  {
    char * p = buf;
    for (int i = 35; i >= 0; i --)
      {
        * p ++ = ((1llu << i) & v) ? '+' : '-';
      }
    * p ++ = 0;
    draw (x, y, buf);
  }

int main (int argc, char * argv [])
  {
    sid = getsid (0);
    if (argc > 1 && strlen (argv [1]))
      {
        char * end;
        long p = strtol (argv [1], & end, 0);
        if (* end == 0)
          {
            sid = p;
            argv [1] [0] = 0;
          }
      }

    int cpunum = 0;
    if (argc > 2 && strlen (argv [2]))
      {
        char * end;
        long p = strtol (argv [2], & end, 0);
        if (* end == 0)
          {
            cpunum = p;
            argv [1] [0] = 0;
          }
      }
    if (cpunum < 0 || cpunum > N_CPU_UNITS_MAX - 1)
      {
        printf ("invalid cpu number %d\n", cpunum);
        return 1;
      }

    cpus = (cpu_state_t *) open_shm ("cpus", sid, sizeof (cpu_state_t) * N_CPU_UNITS_MAX);
    if (! cpus)
      {
        perror ("cpus open_shm");
        return 1;
      }
    cpun = cpus + cpunum;



    int fbfd = 0;
    struct fb_var_screeninfo orig_vinfo;
    long int screensize = 0;

    // Open the framebuffer file for reading and writing
    fbfd = open("/dev/fb1", O_RDWR);
    if (!fbfd) {
      printf("Error: cannot open framebuffer device.\n");
      return(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
      printf("Error reading variable information.\n");
    }
    printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres,
       vinfo.bits_per_pixel );
    bytes_per_pixel = vinfo.bits_per_pixel / 8;

    // Store for reset (copy vinfo to vinfo_orig)
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
      printf("Error reading fixed information.\n");
    }
    //printf("Fixed info: smem_len %d, line_length %d\n", finfo.smem_len, finfo.line_length);
    // map fb to user mem
    screensize = finfo.smem_len;
    //printf ("screensize accoring to ioctl %d; according to s/w %d\n", screensize, vinfo.xres * vinfo.yres * sizeof (pxl));
    fbp = (pxl*)mmap(0,
              screensize,
              PROT_READ | PROT_WRITE,
              MAP_SHARED,
              fbfd,
              0);

    if ((int)fbp == -1) {
        printf("Failed to mmap.\n");
        return 1;
    }

//  0  PRR P PSR
//  1  IC
//  2  IWB
//  3  A
//  4  Q
//  5  E (8bits)
//  6  X0 X1
//  7  X2 X3
//  8  X4 X5
//  9  X6 X7
// 10  IR
// 11  TR
// 12  RALR

    fill_rect (0, 0, vinfo.xres, vinfo.yres, grey);

//                 "0123456789012345678901234567890123456789"
    int l = 0;
    draw (0, l ++, "PRR ___ P _ PSR _______________");
    draw (0, l ++, "TRR ___ TSR _______________ TBR ______");
    draw (0, l ++, "IC  __________________");
    draw (0, l ++, "IWB ____________________________________");
    draw (0, l ++, "A   ____________________________________");
    draw (0, l ++, "Q   ____________________________________");
    draw (0, l ++, "E   ________ CA __________________");
    draw (0, l ++, "01 __________________ __________________");
    draw (0, l ++, "23 __________________ __________________");
    draw (0, l ++, "45 __________________ __________________");
    draw (0, l ++, "67 __________________ __________________");
    draw (0, l ++, "IR  __________________ RALR ___");
    draw (0, l ++, "TR  __________________________");
    draw (0, l ++, "0 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "1 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "2 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "3 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "4 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "5 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "6 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
    draw (0, l ++, "7 _______________ ___");
    draw (0, l ++, "  __ ____ __________________");
//DBSR  ADDR 24, BND 14 U 1 STACK 12
    draw (0, l ++, "ADDR ________________________");
    draw (0, l ++, "BND ______________ U _ STK ____________");
    draw (0, l ++, "FLT _____");
    draw (0, l ++, "SBF ____________________________________");
//                 "0123456789012345678901234567890123456789"



    while (1)
      {
        l = 0;
        drawn ( 3, cpun -> PPR.PRR, 4,  l); drawn ( 1, cpun -> PPR.P,  10,  l); drawn (15, cpun -> PPR.PSR, 16,  l);
        l ++;
        drawn ( 3, cpun -> TPR.TRR, 4,  l); drawn (15, cpun -> TPR.TSR,  12,  l); drawn (6, cpun -> TPR.TBR, 32,  l);
        l ++;
        drawn (18, cpun -> PPR.IC,  4,  l);
        l ++;
        drawn (36, cpun -> cu.IWB,  4,  l);
        l ++;
        drawn (36, cpun -> rA,      4,  l);
        l ++;
        drawn (36, cpun -> rQ,      4,  l);
        l ++;
        drawn ( 8, cpun -> rE,      4,  l);
        drawn (18, cpun -> TPR.CA, 16,  l);
        l ++;
        for (int i = 0; i < 8; i += 2)
          {
            drawn (18, cpun -> rX [i + 0],  3,  l); drawn (18, cpun -> rX [i + 1], 22,  l);
            l ++;
          }
        drawn (18, cpun -> cu . IR,   4, l); drawn ( 3, cpun -> rRALR,   28, l);
        l ++;
        drawn (26, cpun -> rTR,     4, l);
        l ++;
        for (int i = 0; i < 8; i ++)
          {
            drawn (15, cpun -> PAR [i] . SNR,     2, l); 
            drawn ( 3, cpun -> PAR [i] . RNR,    18, l); 
            l ++;
            drawn ( 2, cpun -> PAR [i] . CHAR,    2, l);
            drawn ( 4, cpun -> PAR [i] . BITNO ,  5, l);
            drawn (18, cpun -> PAR [i] . WORDNO, 10, l);
            l ++;
          }
        drawn (24, cpun -> DSBR.ADDR, 5, l);
        l ++;
        drawn (14, cpun -> DSBR.BND,    4, l);
        drawn ( 1, cpun -> DSBR.U,     21, l);
        drawn (12, cpun -> DSBR.STACK, 27, l);
        l ++;
        drawn ( 5, cpun -> faultNumber, 4, l);
        l ++;
        drawn (36, cpun -> subFault, 4, l);



        usleep (10000);
      }
// XXX

    // cleanup
    munmap(fbp, screensize);
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
        printf("Error re-setting variable information.\n");
    }
    close(fbfd);

    return 0;


}

