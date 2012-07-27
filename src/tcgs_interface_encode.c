/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_encode.c
///
/// Encode/decodes data sent/received from transport interface
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "tcgs_types.h"
#include "tcgs_parser.h"
#include "tcgs_interface_encode.h"


void TCGS_DecodeLevel0Discovery (uint8* data)
{
	TCGS_Level0Discovery_Header_t *header;
	TCGS_Level0Discovery_Feature_t *iter;

	header = TCGS_GetLevel0DiscoveryHeader(data);
	if (header != NULL)
	{
		header->length = _swap32(header->length);
		header->versionMajor = _swap16(header->versionMajor);
		header->versionMinor = _swap16(header->versionMinor);
	}
    iter = TCGS_GetLevel0DiscoveryFirstFeatureHeater(data);
	while (iter != NULL)
	{
		iter->code = _swap16(iter->code);
		switch (iter->code)
		{
		case FEATURE_TPER:
			//no decoding required
			break;
		case FEATURE_LOCKING:
			//no decoding required
			break;
		case FEATURE_OPAL1:
			((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->baseComID =
					_swap16(((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->baseComID);
			((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->numberOfComIDs =
					_swap16(((TCGS_Level0Discovery_FeatureOpal1_t*)iter)->numberOfComIDs);
			break;
		//TODO: add decoding for other features
		}
        iter = TCGS_GetLevel0DiscoveryNextFeatureHeater(data, iter);
	}
}
