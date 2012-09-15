/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// ATA interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#if TCGS_VERBOSE
#include <stdio.h>
#endif //TCGS_VERBOSE

#include <stdio.h>

#include "tcgs_interface.h"
#include "tcgs_interface_ata.h"
#include "tcgs_types.h"
#include "tcgs_verbose.h"

/// Current interface descriptor
static TCGS_InterfaceDescriptor_t *TCGS_Interface_Desc = NULL;

void TCGS_SetInterfaceFunctions(TCGS_InterfaceDescriptor_t *descriptor)
{
	TCGS_Interface_Desc = descriptor;
}

TCGS_InterfaceDescriptor_t* TCGS_GetInterface(void)
{
	return TCGS_Interface_Desc;
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
 * ERROR_INTERFACE is returned when
 *
 *****************************************************************************/
TCGS_InterfaceError_t TCGS_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	TCGS_InterfaceError_t error;
#if TCGS_VERBOSE
	printf(TCGS_VERBOSE_COMMAND_SEPARATOR "\n");
	TCGS_PrintCommand(inputCommandBlock);
#endif //TCGS_VERBOSE
	error = (*TCGS_Interface_Desc->send)(inputCommandBlock, inputPayload, tperError, outputPayload);
#if TCGS_VERBOSE
	printf(TCGS_VERBOSE_COMMAND_SEPARATOR "\n");
#endif //TCGS_VERBOSE
	return error;
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
