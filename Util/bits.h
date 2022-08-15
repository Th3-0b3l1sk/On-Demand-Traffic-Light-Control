/*
 * bits.h
 *
 * Created: 7/31/2022 9:56:27 PM
 *  Author: Mahmoud
 */ 


#ifndef BITS_H_
#define BITS_H_

#define BIT_0	0
#define BIT_1	1
#define BIT_2	2
#define BIT_3	3
#define BIT_4	4
#define BIT_5	5
#define BIT_6	6
#define BIT_7	7


#define BIT_READ(x, n)   ( ((unsigned) x & ( 1 << n)) >> n )
#define BIT_WRITE(x, n)  ( x |= (1U << n))						 
#define BIT_CLEAR(x, n)  ( x  &= (~(1U << n )))
#define BIT_TOGGLE(x, n)  x ^= (1U << n) 
#define BIT_MASK_WRITE(x, m) ( x |= (m) ) 
#define BIT_COND_WRITE(x, n, v)  ( x |= ((unsigned) v << n))
#define IS_SET(x, n) (x & (1UL << n) )


#endif /* BITS_H_ */