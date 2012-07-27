/////////////////////////////////////////////////////////////////////////////
/// tcgs_types.h
///
/// Common types and definitions of TCG Storage Lib
///
/// (c) Artem Zankovich
////////////////////////////////////////////////////////////////////////////// 
#ifndef _TCGS_TYPES_H
#define _TCGS_TYPES_H

#define uint8  unsigned char 
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long long
//#define bool unsigned char
#define FALSE  (0)
#define TRUE   (!FALSE)

typedef enum 
{
	ERROR_SUCCESS,
	ERROR_BUILDER,
	ERROR_INTERFACE
} TCGS_Error_t;

//minimal block size of the storage device
#define TCGS_BLOCK_SIZE 512

#endif //_TCGS_TYPES_H
