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

extern TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor;

typedef enum
{
	ATA_TRANSPORT_DMA = 0,
	ATA_TRANSPORT_PIO = 1,
} TCGS_ATA_TransportMode_t;

#define ATA_TRUSTED_RECEIVE 0x5C
#define ATA_TRUSTED_RECEIVE_DMA 0x5D
#define ATA_TRUSTED_SEND 0x5E
#define ATATRUSTED_SEND_DMA 0x5F


#endif //TCGS_INTERFACE_ATA_H
