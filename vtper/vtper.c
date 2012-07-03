/////////////////////////////////////////////////////////////////////////////
/// vtper.c
///
/// implementation of virtual tper
///
/// (c) Artem Zankovich
//////////////////////////////////////////////////////////////////////////////
#ifndef _TCGS_VTPER_H
#define _TCGS_VTPER_H

#include "tcgs_types.h"
#include "tcgs_interface.h"

void TCGS_VTPer_Init(void)
{
	return;
}


TCGS_TPerError_t TCGS_VTPER_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_TPerError_t *tperError, void *outputPayload)
{
	*tperError = INTERFACE_ERROR_GOOD;
	return ERROR_SUCCESS;
}

#endif //_TCGS_VTPER_H
