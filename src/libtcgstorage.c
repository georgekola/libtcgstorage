/////////////////////////////////////////////////////////////////////////////
/// libtcgstorage.c
///
/// implementation of external API of TCG Storage Host
///
/// (c) Artem Zankovich, 2012
//////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdbool.h>

//TODO: remove debug output
#include <stdio.h>

#include "libtcgstorage.h"
#include "tcgs_types.h"
#include "tcgs_interface.h"
#include "tcgs_stream.h"
#include "tcgs_builder.h"
#include "tcgs_interface_encode.h"

      
/*****************************************************************************
 * \brief Initializes TCG Storage Host
 *
 * \par The function is to be called when before calling any other TCG Storage Host
 * function of the library API.
 *
 * \par The second call of the function fails, use TCGS_ResetHost
 * to re-initialize TCG Storage Host.
 * 
 * \see TCGS_ResetHost, TCGS_DestroyHost
 *****************************************************************************/
void TCGS_InitHost(TCGS_InterfaceDescriptor_t *interfaceDesc)
{
    TCGS_Interface_SetDescripor(interfaceDesc);
}

/*****************************************************************************
 * \brief Re-initializes TCG Storage Host
 *
 * \par The function reset current state of TCG Storage Host. All session,
 * transaction, protocol, properties, etc. variables are re-set to default values.
 *
 * \par The second call of the function fails, use TCGS_ResetHost
 * to re-initialize TCG Storage Host state.
 *
 * \return None
 *
 * \see TCGS_InitHost
 *****************************************************************************/
void TCGS_ResetHost(void)
{
    return;
} 

/*****************************************************************************
 * \brief Destroy TCG Storage Host
 *
 * \par The function frees all resources associated with TCG Storage Host
 * that was previously initialized with TCGS_HostInit. Make sure to call it
 * when work with TCG Storage host is complete.
 *
 * \par TCGS_HostInit shall be called before.
 *
 * \return None
 *
 * \see TCGS_InitHost
 *****************************************************************************/
void TCGS_DestroyHost(void)
{
    return;
}

uint8 TCGS_Buffer_Level0Discovery[TCGS_BLOCK_SIZE];

/*****************************************************************************
 * \brief Read Level 0 Discovery data from device
 *
 * \par The function fills internal buffer of one block size with data of
 * Level 0 Discovery from TPer.
 *
 * \par TCGS_HostInit shall be called before.
 *
 * \return TCGS_Error_t with error code in case of read error due to command abort
 *
 * \see TCGS_InitHost, TCGS_GetLevel0Discovery
 *****************************************************************************/
TCGS_Error_t TCGS_Level0Discovery(void)
{
    TCGS_CommandBlock_t commandBlock;
    TCGS_Error_t status;
    TCGS_InterfaceError_t errorInterface;
    TCGS_Level0Discovery_Header_t *header;
    TCGS_Level0Discovery_FeatureTper_t *headerTper;

    TCGS_PrepareInterfaceCommand(LEVEL0_DISCOVERY, NULL, &commandBlock, NULL);
    status = TCGS_Interface_IoCommand(&commandBlock, NULL, &errorInterface, &TCGS_Buffer_Level0Discovery);
    if (status != ERROR_SUCCESS)
    {
        return status;
    }

    if (errorInterface != INTERFACE_ERROR_GOOD) {
        //TODO: remove interface output
        printf("Interface error\n");
        return ERROR_INTERFACE;
    }

    header = TCGS_DecodeLevel0Discovery(&TCGS_Buffer_Level0Discovery);

#if defined(TCGS_VERBOSE)
    TCGS_PrintLevel0Discovery(header);
#endif //defined(TCGS_VERBOSE)

    return ERROR_SUCCESS;
}

static TCGS_ErrorCode_t lastError = ERROR_NONE;

/*
 * Store last error code of libtcgstore. Previous error code is overwritten.
 * Use TCGS_GetError to get it
 *
 * \see TCGS_GetError
 */
void TCGS_SetError(TCGS_ErrorCode_t errorCode)
{
    lastError = errorCode;
}

/*
 * Receives last error code of libtcgstore
 */
TCGS_ErrorCode_t TCGS_GetError(void)
{
    return lastError;
}
