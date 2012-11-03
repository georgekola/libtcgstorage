/////////////////////////////////////////////////////////////////////////////
/// tcgs_config.h
///
/// Common parameters for libtcgstorage
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#ifndef TCGS_CONFIG_H_
#define TCGS_CONFIG_H_

#include <stdbool.h>

//supported interfaces
#define TCGS_INTERFACE_SCSI_SUPPORTED
#define TCGS_INTERFACE_ATA_SUPPORTED
#define TCGS_INTERFACE_NVM_EXPRESS_SUPPORTED
#define TCGS_INTERFACE_VTPER_SUPPORTED

//debug output
#define TCGS_VERBOSE TRUE

#endif /* TCGS_CONFIG_H_ */
