/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_virtual.c
///
/// Virtual TPer interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include "tcgs_interface_virtual.h"
#include "tcgs_types.h"
#include "tcgs_interface.h"
#include "vtper.h"


TCGS_InterfaceFunctions_t TCGS_Interface_Virtual_Funcs =
{
	(TCGS_SendCommand_t)&TCGS_Virtual_SendCommand,
};


/*****************************************************************************
 * \brief Map command to virtual TPer interface and send it to TPer. Return response and status
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to Virtual
 * TPer transport sent to TPer and the last returned response (error status
 * code and payload). Error code ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
TCGS_InterfaceError_t TCGS_Virtual_SendCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	return TCGS_VTPER_SendCommand(inputCommandBlock, inputPayload, tperError, outputPayload);
}
