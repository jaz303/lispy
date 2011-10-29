#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>

#ifdef _LP64
	typedef void* VALUE;
	typedef int64_t INT;
	typedef uint64_t INTERN;
#else
	typedef void* VALUE;
	typedef int32_t INT;
	typedef uint32_t INTERN;
#endif

typedef enum token {
	T_ERROR			= -1,
	T_EOF			=  0,
	T_L_PAREN		=  1,
	T_R_PAREN		=  2,
	T_INT			=  3,
	T_TRUE			=  4,
	T_FALSE			=  5,
	T_STRING			=  6,
	T_ATOM			=  7,
	T_IDENT			=  8
} token_t;

#endif // GLOBAL_H
