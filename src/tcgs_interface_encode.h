/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_encode.h
///
/// Encode/decodes data sent/received from transport interface
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#ifndef TCGS_INTERFACE_ENCODE_H_
#define TCGS_INTERFACE_ENCODE_H_

#include "tcgs_types.h"

#define _swap16(x) (((((uint16)((x) & 0xFF00)) >> 8)) | ((uint16)(((x) & 0x00FF) << 8)))

#define _swap32(x) \
	((((uint32)((x) & 0x000000FF)) << 24) | \
	 (((uint32)((x) & 0x0000FF00)) << 8) | \
	 (((uint32)((x) & 0x00FF0000)) >> 8) | \
	 (((uint32)((x) & 0xFF000000)) >> 24))

#define _swap64(value)                                \
	(((((uint64)(x))<<56) & 0xFF00000000000000ULL)  | \
	 ((((uint64)(x))<<40) & 0x00FF000000000000ULL)  | \
	 ((((uint64)(x))<<24) & 0x0000FF0000000000ULL)  | \
	 ((((uint64)(x))<< 8) & 0x000000FF00000000ULL)  | \
	 ((((uint64)(x))>> 8) & 0x00000000FF000000ULL)  | \
	 ((((uint64)(x))>>24) & 0x0000000000FF0000ULL)  | \
	 ((((uint64)(x))>>40) & 0x000000000000FF00ULL)  | \
	 ((((uint64)(x))>>56) & 0x00000000000000FFULL))

TCGS_Level0Discovery_Header_t* TCGS_DecodeLevel0Discovery();

#endif //#TCGS_INTERFACE_ENCODE_H_
