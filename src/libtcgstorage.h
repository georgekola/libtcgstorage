/////////////////////////////////////////////////////////////////////////////
/// libtcgstorage.h
///
/// the main include file of the library
/// contains external API of TCG Storage Host
///
/// (c) Artem Zankovich, 2012
//////////////////////////////////////////////////////////////////////////////
#ifndef _LIBTCGSTORAGE_H
#define _LIBTCGSTORAGE_H  

#include <stdbool.h>

#include "tcgs_config.h"
#include "tcgs_types.h"
#include "tcgs_stream.h"
#include "tcgs_interface.h"

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
void TCGS_InitHost(TCGS_InterfaceDescriptor_t *interfaceDesc);

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
void TCGS_ResetHost(void); 

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
void TCGS_DestroyHost(void);

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
TCGS_Error_t TCGS_Level0Discovery(void);

/*****************************************************************************
 * \brief Process TCG command
 *
 * \par The function sends TCG command to opened device and gets response
 *
 * \par TCGS_HostInit shall be called before.
 *
 * \return TCGS_Error_t with error code in case of error occured during
 * command processing
 *
 * \see TCGS_InitHost
 *****************************************************************************/
TCGS_Error_t TCGS_ProcessCommand(char *command, char *response, unsigned responseLength);

/*
 * Receives last error code of libtcgstore
 */
TCGS_ErrorCode_t TCGS_GetError(void);

#endif //_LIBTCGSTORAGE_H
