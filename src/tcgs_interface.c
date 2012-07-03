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
#include <string.h>

static TCGS_Interface_t currentInterface;
TCGS_IntefaceFunctions_t *TCGS_Interface_Funcs;

void TCGS_SetInterfaceFunctions(TCGS_IntefaceFunctions_t *functs)
{
	TCGS_Interface_Funcs = functs;
	currentInterface = INTERFACE_UNKNOWN;
}

void TCGS_SetInterface(TCGS_Interface_t interface)
{
	switch(interface)
	{
	case INTERFACE_SCSI:
		break;
	case INTERFACE_ATA:
		TCGS_SetInterfaceFunctions(&TCGS_Interface_ATA_Funcs);
		break;
	case INTERFACE_NVM_EXPRESS:
		break;
	case INTERFACE_UNKNOWN:
		break;
	}
	currentInterface = interface;
}

/*****************************************************************************
 * \brief Map command to ATA interface and send it to TPer. Return response and status.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to ATA transport
 * sent to TPer and the last returned response (error status code and payload). Error code
 * ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
TCGS_TPerError_t TCGS_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_TPerError_t *tperError, void *outputPayload)
{
	return (*TCGS_Interface_Funcs->send)(inputCommandBlock, inputPayload, tperError, outputPayload);
}

#define MAX_INTERFACE_PARAMETER_LENGTH 32

typedef struct
{
	char    name[MAX_INTERFACE_PARAMETER_LENGTH + 1];
	uint32  value;
} TCGS_IntefaceParameter_t;

TCGS_IntefaceParameter_t parameters[] =
{
		{"ata.transport_mode", (uint32)ATA_TRANSPORT_DMA},
};

void TCGS_SetParameter(char *name, uint32 value)
{
	int i;

	for (i = 0; i < sizeof(parameters) / sizeof(parameters[0]); i++)
	{
		if (strncmp(name, parameters[i].name, MAX_INTERFACE_PARAMETER_LENGTH))
		{
			parameters[i].value = value;
			break;
		}
	}
}

uint32 TCGS_GetParameter(char *name)
{
	int i;

	for (i = 0; i < sizeof(parameters) / sizeof(parameters[0]); i++)
	{
		if (strncmp(name, parameters[i].name, MAX_INTERFACE_PARAMETER_LENGTH))
		{
			return parameters[i].value;
		}
	}

	return 0;
}
