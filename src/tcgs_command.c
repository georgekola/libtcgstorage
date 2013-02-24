/////////////////////////////////////////////////////////////////////////////
/// tcgs_command.c
///
/// Internal representation of TCG Storage command   
///
/// (c) Artem Zankovich, 2013
//////////////////////////////////////////////////////////////////////////////

#include "tcgs_command.h"
#include <stdlib.h>

TCGS_Token_t *get_new_int_token(uint32 value)
{
	TCGS_Token_t *token = (TCGS_Token_t*)malloc(sizeof(TCGS_Token_t));
	token->type = TCGS_Value_Int;
	token->value.integer32 = value;
	token->size = sizeof(uint32);
    
    return token;
}
