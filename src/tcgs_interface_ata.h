/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// ATA interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _TCGS_INTERFACE_ATA_H
#define _TCGS_INTERFACE_ATA_H

#include "tcgs_stream.h"

//definition of ATA interface descriptor is in platform-specific file
extern TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor;

TCGS_InterfaceParameters_t* TCGS_ATA_GetParameters (void);

//< Modes of ATA interface for trusted commands
typedef enum {
	ATA_TRANSPORT_PIO = 0,
    ATA_TRANSPORT_DMA = 1,
} TCGS_ATA_TransportMode_t;

/////////////////////////////////////////////////////////////////////////////
//* ATA interface parameters
/////////////////////////////////////////////////////////////////////////////

//< Current libtcgstorage ATA interface mode for trusted commands (\see TCGS_ATA_TransportMode_t)
#define INTERFACE_PARAMETER_ATA_TRANSPORT_MODE                   "ata.transport_mode"
//< Device read-only boolean parameter -- Trusted Feature Set is supported
#define INTERFACE_PARAMETER_ATA_TRUSTED_FEATURE_SUPPORTED        "ata.trusted_feature.supported"
//< Device read-only boolean parameter -- Security Feature Set is supported
#define INTERFACE_PARAMETER_ATA_SECURITY_FEATURE_SUPPORTED       "ata.security_feature.supported"
//< Device read-only boolean parameter -- Security Feature Set is enabled
#define INTERFACE_PARAMETER_ATA_SECURITY_FEATURE_ENABLED         "ata.security_feature.enabled"
//< Device read-only boolean parameter -- Extended Error Status is supported
#define INTERFACE_PARAMETER_ATA_EXTENDED_ERROR_FEATURE_SUPPORTED "ata.ext_error_feature.supported"


/////////////////////////////////////////////////////////////////////////////
//< ATA command codes
#define ATA_IDENTIFY 0xEC
#define ATA_TRUSTED_RECEIVE 0x5C
#define ATA_TRUSTED_RECEIVE_DMA 0x5D
#define ATA_TRUSTED_SEND 0x5E
#define ATA_TRUSTED_SEND_DMA 0x5F


#endif //TCGS_INTERFACE_ATA_H
