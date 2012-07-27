/////////////////////////////////////////////////////////////////////////////
/// vtper.h
///
/// implementation of virtual tper
///
/// (c) Artem Zankovich
//////////////////////////////////////////////////////////////////////////////
#ifndef _TCGS_VTPER_H
#define _TCGS_VTPER_H

#include "tcgs_types.h"
#include "tcgs_interface.h"


void TCGS_VTPer_Init(void);

TCGS_InterfaceError_t TCGS_VTPER_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload);


#endif //_TCGS_VTPER_H
