/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// ATA interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include "tcgs_interface.h"
#include "tcgs_interface_ata.h"
#include "tcgs_types.h"

static void TCGS_ATA_SetParameter(char *name, uint32 value)
{
	return;
}

static uint32 TCGS_ATA_GetParameter(char *name)
{
	return 0;
}

/*
TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor =
{
	INTERFACE_ATA,
	(TCGS_OpenCommand_t)&TCGS_ATA_Open,
	(TCGS_IoCommand_t)&TCGS_ATA_Send,
	(TCGS_SetParameterCommand_t)&TCGS_ATA_SetParameter,
	(TCGS_GetParameterCommand_t)&TCGS_ATA_GetParameter,
};
*/

