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

#define INTERFACE_PARAMETER_ATA_TRANSPORT_MODE "ata.transport_mode"

typedef enum {
	ATA_TRANSPORT_DMA = 0,
	ATA_TRANSPORT_PIO = 1,
} TCGS_ATA_TransportMode_t;

#define ATA_TRUSTED_RECEIVE 0x5C
#define ATA_TRUSTED_RECEIVE_DMA 0x5D
#define ATA_TRUSTED_SEND 0x5E
#define ATA_TRUSTED_SEND_DMA 0x5F


#endif //TCGS_INTERFACE_ATA_H
