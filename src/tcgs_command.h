/////////////////////////////////////////////////////////////////////////////
/// tcgs_command.h
///
/// Internal representation of TCG Storage command
///
/// (c) Artem Zankovich, 2013
/////////////////////////////////////////////////////////////////////////////
#ifndef _TCGS_COMMAND_H
#define _TCGS_COMMAND_H

#include "tcgs_types.h"

typedef enum TCGS_TokenType {
	TCGS_Value_Int,
	TCGS_Value_Bytes,
	TCGS_Value_List,
	TCGS_Value_Set,
	TCGS_Value_Named,
} TCGS_TokenType_t;


typedef union TCGS_TokenValue {
	void *ref;
	uint32 integer32;
	uint64 integer64;
} TCGS_TokenValue_t;


typedef struct TCGS_Token {
	TCGS_TokenType_t	type;
	TCGS_TokenValue_t	value;
	uint32				size;
	struct TCGS_Token	*next;
} TCGS_Token_t;



TCGS_Token_t *get_new_int_token(uint32 value);

#endif //_TCGS_COMMAND_H