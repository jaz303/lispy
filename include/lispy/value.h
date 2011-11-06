#ifndef VALUE_H
#define VALUE_H

#include <stdint.h>

#include "lispy/object.h"
#include "lispy/global.h"

#define _INT_BIT				(1 << 0)
#define _INT_SHIFT				1

#define _SPECIAL_BIT			(1 << 1)
#define _SPECIAL_SHIFT			2

#define _SPECIAL_NIL			0
#define _SPECIAL_FALSE			1
#define _SPECIAL_TRUE			3
#define _SPECIAL_ATOM			4
#define _SPECIAL_IDENT			6

#define _NIL_MASK				(_SPECIAL_BIT | (_SPECIAL_NIL << _SPECIAL_SHIFT))
#define _BOOL_MASK				(_SPECIAL_BIT | (_SPECIAL_FALSE << _SPECIAL_SHIFT))
#define _FALSE_MASK				(_SPECIAL_BIT | (_SPECIAL_FALSE << _SPECIAL_SHIFT))
#define _TRUE_MASK				(_SPECIAL_BIT | (_SPECIAL_TRUE << _SPECIAL_SHIFT))
#define _ATOM_MASK				(_SPECIAL_BIT | (_SPECIAL_ATOM << _SPECIAL_SHIFT))
#define _IDENT_MASK				(_SPECIAL_BIT | (_SPECIAL_IDENT << _SPECIAL_SHIFT))

#define _IDERN_SHIFT			(_SPECIAL_SHIFT + 3)

// Pointer

#define VALUE_IS_PTR(v)			(((INT)v & (_INT_BIT | _SPECIAL_BIT)) == 0)

// Error

#define kError					0
#define VALUE_IS_ERROR(v)		(((INT)v) == kError)

// Integers

#define VALUE_IS_INT(v)			(((INT)v & _INT_BIT) == _INT_BIT)
#define MK_INTVAL(i)			((VALUE)(((INT)i << _INT_SHIFT) | _INT_BIT))
#define SET_INTVAL(v, i)		(v = MK_INTVAL(i))
#define INTVAL(v)				(((INT)v) >> _INT_SHIFT)

// Nil

#define kNil					(_NIL_MASK)

#define VALUE_IS_NIL(v)			((INT)v == _NIL_MASK)
#define MK_NIL()				(_NIL_MASK)
#define SET_NIL(v)				(v = MK_NIL())

// Boolean

#define kTrue					((VALUE)(_TRUE_MASK))
#define kFalse					((VALUE)(_FALSE_MASK))

#define VALUE_IS_BOOL(v)		(((INT)v & _BOOL_MASK) == _BOOL_MASK)
#define VALUE_IS_TRUE(v)		((INT)v == _TRUE_MASK)
#define VALUE_IS_FALSE(v)		((INT)v == _FALSE_MASK)
#define BOOLVAL(v)				((INT)v & ~_BOOL_MASK)

// Atom

#define VALUE_IS_ATOM(v)		(((INT)v & _ATOM_MASK) == _ATOM_MASK)
#define MK_ATOM(i)				((VALUE)(((INT)i << _IDERN_SHIFT) | _ATOM_MASK))
#define SET_ATOM(v, i)			(v = MK_ATOM(i))
#define ATOM(v)					(((INT)v) >> _IDERN_SHIFT)

// Ident

#define VALUE_IS_IDENT(v)		(((INT)v & _IDENT_MASK) == _IDENT_MASK)
#define MK_IDENT(i)				((VALUE)(((INT)i << _IDERN_SHIFT) | _IDENT_MASK))
#define SET_IDENT(v, i)			(v = MK_IDENT(i))
#define IDENT(v)				(((INT)v) >> _IDERN_SHIFT)

// Truthy

#define VALUE_IS_TRUTHY(v)		(!VALUE_IS_FALSE(v) && !VALUE_IS_NIL(v))

#endif // VALUE_H
