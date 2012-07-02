/////////////////////////////////////////////////////////////////////////////
/// libtcgstorage.h
///
/// the main include file of the library
/// contains external API of TCG Storage Host
/// (c) Artem Zankovich
//////////////////////////////////////////////////////////////////////////////
#ifndef _LIBTCGSTORAGE_H
#define _LIBTCGSTORAGE_H  

#include "stdbool.h"
#include "tcgs_types.h"

/*****************************************************************************
 * \brief Initializes TCG Storage Host
 *
 * \par The function is to be called when before calling any other TCG Storage Host
 * function of the library API.
 *
 * \par The second call of the function fails, use TCGS_ResetHost
 * to re-initialize TCG Storage Host.
 *
 * \return
 *  TRUE if initialisation is completed successfully,
 *  FALSE otherwise
 * 
 * \see TCGS_ResetHost, TCGS_DestroyHost
 *****************************************************************************/
bool TCGS_InitHost(void);

/*****************************************************************************
 * \brief Re-initializes TCG Storage Host
 *
 * \par The function reset current state of TCG Storу Host. All session,
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

#endif //_LIBTCGSTORAGE_H
