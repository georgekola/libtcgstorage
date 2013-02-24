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
#include "tcgs_types.h"
#include "tcgs_verbose.h"

/// Current interface descriptor
static TCGS_InterfaceDescriptor_t *TCGS_InterfaceDesc = NULL;

void TCGS_Interface_SetDescriptor(TCGS_InterfaceDescriptor_t *descriptor)
{
	TCGS_InterfaceDesc = descriptor;
}

TCGS_InterfaceDescriptor_t* TCGS_Interface_GetDesciptor(void)
{
	return TCGS_InterfaceDesc;
}

//array of supported interfaces (type code + printable name)
TCGS_InterfaceName_t TCGS_InterfaceNames[] = 
{
#if defined(TCGS_INTERFACE_SCSI_SUPPORTED)
    {INTERFACE_SCSI, "SCSI"},
#endif //TCGS_INTERFACE_SCSI_SUPPORTED
#if defined(TCGS_INTERFACE_ATA_SUPPORTED)
    {INTERFACE_ATA, "ATA"},
#endif //TCGS_INTERFACE_ATA_SUPPORTED
#if defined(TCGS_INTERFACE_NVM_EXPRESS_SUPPORTED)
    {INTERFACE_NVM_EXPRESS, "NVMExpress"},
#endif //TCGS_INTERFACE_NVM_EXPRESS_SUPPORTED
#if defined(TCGS_INTERFACE_VTPER_SUPPORTED)
    {INTERFACE_VTPER, "VTPer"},
#endif //TCGS_INTERFACE_VTPER_SUPPORTED
    {INTERFACE_UNKNOWN, NULL},
};

/*****************************************************************************
 * \brief Get internal interface code by printable interface name
 *
 * @param[in]  name     printable interface name
 *
 * \return internal interface code
 *
 * \see TCGS_InterfaceNames
 *
 *****************************************************************************/
TCGS_Interface_t TCGS_Interface_GetCode(char *name)
{
    TCGS_InterfaceName_t *interface;
    
    for (interface = &TCGS_InterfaceNames[0]; interface->type != INTERFACE_UNKNOWN; interface++) {
        if (strncasecmp(interface->name, name, MAX_INTERFACE_PARAMETER_NAME_LENGTH) == 0) {
            return interface->type;
        }
    }
    
    return INTERFACE_UNKNOWN;
}

TCGS_Error_t TCGS_Interface_OpenDevice(char *device)
{
    return (*TCGS_InterfaceDesc->open)(device);
}

/*****************************************************************************
 * \brief Map command to current interface and send it to TPer. Return response (if provided)
 * and status.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload                 input payload. NULL if command has no data
 * @param[out] tperError                     interface command error status
 * @param[out] outputPayload              output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to transport command
 * sent to TPer and the last returned response (error status code and payload). Error code
 * ERROR_INTERFACE is returned when
 *
 *****************************************************************************/
TCGS_InterfaceError_t TCGS_Interface_IoCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	TCGS_InterfaceError_t error;

#if TCGS_VERBOSE
	printf(TCGS_VERBOSE_COMMAND_SEPARATOR "\n");
	TCGS_PrintCommand(inputCommandBlock);
	printf(TCGS_VERBOSE_COMMAND_SEPARATOR "\n");
#endif //TCGS_VERBOSE
	error = (*TCGS_InterfaceDesc->send)(inputCommandBlock, inputPayload, tperError, outputPayload);

    return error;
}

void TCGS_Interface_SetParameter(TCGS_InterfaceParameters_t *params, char *name, uint32 value)
{
	int i;

	for (i = 0; i < params->length; i++) {
		if (strncasecmp(name, params->param[i].name, MAX_INTERFACE_PARAMETER_NAME_LENGTH)) {
			params->param[i].value = value;
			break;
		}
	}
}

uint32 TCGS_Interface_GetParameter(TCGS_InterfaceParameters_t *params, char *name)
{
	int i;

	for (i = 0; i < params->length; i++) {
		if (strncasecmp(name, params->param[i].name, MAX_INTERFACE_PARAMETER_NAME_LENGTH)) {
			return params->param[i].value;
		}
	}

	return 0;
}
