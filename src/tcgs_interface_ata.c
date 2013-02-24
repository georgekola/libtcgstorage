/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// ATA interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include "tcgs_interface.h"
#include "tcgs_interface_ata.h"
#include "tcgs_types.h"

static TCGS_IntefaceParameter_t parameter[] =
{
    {INTERFACE_PARAMETER_ATA_TRANSPORT_MODE,                   (uint32)ATA_TRANSPORT_DMA},
    {INTERFACE_PARAMETER_ATA_TRUSTED_FEATURE_SUPPORTED,        (uint32)FALSE},
    {INTERFACE_PARAMETER_ATA_SECURITY_FEATURE_SUPPORTED,       (uint32)FALSE},
    {INTERFACE_PARAMETER_ATA_SECURITY_FEATURE_ENABLED,         (uint32)FALSE},
    {INTERFACE_PARAMETER_ATA_EXTENDED_ERROR_FEATURE_SUPPORTED, (uint32)FALSE},
};

//* \see TCGS_ATA_GetParameters -- getter of this list
static TCGS_InterfaceParameters_t parameters =
{
    SIZEOF_ARRAY(parameter),
    parameter
};

TCGS_InterfaceParameters_t* TCGS_ATA_GetParameters (void)
{
    return &parameters;
}


