#ifndef TYPES_H
#define TYPES_H

typedef __INTPTR_TYPE__   INTPTR;
typedef __UINTPTR_TYPE__  UINTPTR;
typedef __PTRDIFF_TYPE__  PTRDIFF;
typedef __SIZE_TYPE__     SIZE_T;

typedef __UINT8_TYPE__  UINT8;
typedef __UINT16_TYPE__ UINT16;
typedef __UINT32_TYPE__ UINT32;
typedef __UINT64_TYPE__ UINT64;

typedef __INT8_TYPE__   INT8;
typedef __INT16_TYPE__  INT16;
typedef __INT32_TYPE__  INT32;
typedef __INT64_TYPE__  INT64;

typedef unsigned int UINT;

typedef double  DOUBLE;
typedef float   FLOAT;

typedef UINT8 BOOLEAN;
typedef int   BOOL;

#define TRUE  1
#define FALSE 0

#define NULL  ((void*)0)

#endif
