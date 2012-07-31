/////////////////////////////////////////////////////////////////////////////
/// tcgs_verbose.h
///
/// Output content of interface packets
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#ifndef TCGS_VERBOSE_H_
#define TCGS_VERBOSE_H_

#include "tcgs_config.h"

#if defined(TCGS_VERBOSE)

#define TCGS_VERBOSE_COMMAND_SEPARATOR "======================================="
#define TCGS_VERBOSE_BLOCK_SEPARATOR   "---------------------------------------"

/*****************************************************************************
 * \brief Print content of Level 0 Discovery
 *
 * \par TCGS_Level0Discovery shall be called before
 *
 * @param[in]  payload  Pointer to payload returned by TCGS_Level0Discovery
 *
 * \return None
 *
 * \see TCGS_Level0Discovery
 *****************************************************************************/
void TCGS_PrintLevel0Discovery(void* payload);

#endif //defined(TCGS_VERBOSE)

#endif /* TCGS_VERBOSE_H_ */
