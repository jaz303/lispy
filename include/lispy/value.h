#ifndef VALUE_H
#define VALUE_H

#define _INT_BIT				0x01
#define _INT_SHIFT				1

#define _SPECIAL_BIT			0x02
#define _SPECIAL_SHIFT			2

#define _SPECIAL_NIL			0x00
#define _SPECIAL_FALSE			0x01
#define _SPECIAL_TRUE			0x03
#define _SPECIAL_ATOM			0x04
#define _SPECIAL_IDENT			0x06

#define _SPECIAL_MASK           0x1f

#define _NIL_MASK				(_SPECIAL_BIT | (_SPECIAL_NIL   << _SPECIAL_SHIFT))
#define _BOOL_MASK				(_SPECIAL_BIT | (_SPECIAL_FALSE << _SPECIAL_SHIFT))
#define _FALSE_MASK				(_SPECIAL_BIT | (_SPECIAL_FALSE << _SPECIAL_SHIFT))
#define _TRUE_MASK				(_SPECIAL_BIT | (_SPECIAL_TRUE  << _SPECIAL_SHIFT))
#define _ATOM_MASK				(_SPECIAL_BIT | (_SPECIAL_ATOM  << _SPECIAL_SHIFT))
#define _IDENT_MASK				(_SPECIAL_BIT | (_SPECIAL_IDENT << _SPECIAL_SHIFT))

#define _IDERN_SHIFT			(_SPECIAL_SHIFT + 3)

// Pointer

#define VALUE_IS_PTR(v)			(((INT)v & (_INT_BIT | _SPECIAL_BIT)) == 0)

// Error

#define kError					0
#define kUnreachable            0
#define VALUE_IS_ERROR(v)		(((INT)v) == kError)

// Integers

#define VALUE_IS_INT(v)			(((INT)v & _INT_BIT) == _INT_BIT)
#define MK_INTVAL(i)			((VALUE)(((INT)(i) << _INT_SHIFT) | _INT_BIT))
#define INTVAL(v)				(((INT)v) >> (_INT_SHIFT))

// Nil

#define kNil					((VALUE)(_NIL_MASK))

#define VALUE_IS_NIL(v)			(((INT)v) == _NIL_MASK)
#define MK_NIL()				(_NIL_MASK)

// Boolean

#define kTrue					((VALUE)(_TRUE_MASK))
#define kFalse					((VALUE)(_FALSE_MASK))

#define VALUE_IS_BOOL(v)		(((INT)v & _BOOL_MASK) == _BOOL_MASK)
#define VALUE_IS_TRUE(v)		((INT)v == _TRUE_MASK)
#define VALUE_IS_FALSE(v)		((INT)v == _FALSE_MASK)
#define BOOLVAL(v)				((INT)v & ~_BOOL_MASK)

// Atom

#define VALUE_IS_ATOM(v)		(((INT)v & _SPECIAL_MASK) == _ATOM_MASK)
#define MK_ATOM(i)				((VALUE)(((INT)(i) << _IDERN_SHIFT) | _ATOM_MASK))
#define ATOM(v)					(((INT)v) >> _IDERN_SHIFT)

// Ident

#define VALUE_IS_IDENT(v)		(((INT)v & _SPECIAL_MASK) == _IDENT_MASK)
#define MK_IDENT(i)				((VALUE)(((INT)(i) << _IDERN_SHIFT) | _IDENT_MASK))
#define IDENT(v)				(((INT)v) >> _IDERN_SHIFT)

// Truthy

#define VALUE_IS_TRUTHY(v)		(!VALUE_IS_FALSE(v) && !VALUE_IS_NIL(v))

#endif
