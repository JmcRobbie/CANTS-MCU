/**
 * @file bitops.h
 *
 */

/**
 * @addtogroup Common
 * @{
 */

#ifndef BITOPS_H_
#define BITOPS_H_

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define Bit(N) (1UL << (N))

#define Clamp(val, min, max) ((val) > (max) ? (max) : (((val) < (min)) ? (min) : (val)))

#define SetBits(var, bits) ((var) |= (bits))
#define ClrBits(var, bits) ((var) &= ~(bits))
#define WriteBits(var, bits, set) ((var) = ((set) ? ((var) | (bits)) : ((var) & ~(bits))))

#define SetBitsTyped(var, bits, type) ((var) |= (type)(bits))
#define ClrBitsTyped(var, bits, type) ((var) &= (type)~(bits))
#define WriteBitsTyped(var, bits, set, type) ((var) = ((set) ? (var | (type)(bits)) : (var & (type)~(bits))))

#define GetBit(var, bit) (!!((var) & (1UL << (bit))))
#define IsBitSet(var, bit) (((var) & (bit)) == (bit))

#define ClrSetBits(var, clrbits, setbits) ((var) = ((var) & (~(clrbits))) | (setbits))
#define ClrSetBitsTyped(var, clrbits, setbits, type) ((var) = ((var) & (type)~(clrbits)) | (type)(setbits))

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif

/**
 * @}
 */
