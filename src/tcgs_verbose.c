/////////////////////////////////////////////////////////////////////////////
/// tcgs_verbose.c
///
/// Output content of interface packets
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>

#include "tcgs_config.h"
#include "tcgs_stream.h"
#include "tcgs_parser.h"
#include "tcgs_verbose.h"

#if defined(TCGS_VERBOSE)

void TCGS_PrintLevel0DiscoveryFeature(TCGS_Level0Discovery_Feature_t *feature)
{
	printf(TCGS_VERBOSE_BLOCK_SEPARATOR "\n");
	printf( "Feature Code:       %3d\n",
			//"Version:            %3d\n"
			//"Length:             %3d\n",
			feature->code);
			//feature->version,
			//feature->length);

	switch(feature->code)
	{
	case FEATURE_TPER:
		#define featureTPer ((TCGS_Level0Discovery_FeatureTper_t*)(void*)feature)
		printf( "Sync Supported:       %d\n"
				"Async Supported:      %d\n"
				"ACK/NAK Supported:    %d\n"
				"Buf Mgmt Supported:   %d\n"
				"Streaming Supported:  %d\n"
				"ComID Mgmt Supported: %d\n",
				featureTPer->syncSupported,
				featureTPer->asyncSupported,
				featureTPer->ackSupported,
				featureTPer->bufferManagementSupported,
				featureTPer->streamingSupported,
				featureTPer->comIdManagementSupported);
		break;
	case FEATURE_LOCKING:
		#define featureLocking ((TCGS_Level0Discovery_FeatureLocking_t*)(void*)feature)
		printf(
			"Locking Supported:    %d\n"
			"Locking Enabled:      %d\n"
			"Locked:               %d\n"
			"Media Encr. Locked:   %d\n"
			"MBR Enabled:          %d\n"
			"MBR Done:             %d\n",
			featureLocking->lockingSupport,
			featureLocking->lockingEnabled,
			featureLocking->locked,
			featureLocking->mediaEncryption,
			featureLocking->MBREnabled,
			featureLocking->MBRDone);
		break;
	case FEATURE_OPAL1:
		#define featureOpal1 ((TCGS_Level0Discovery_FeatureOpal1_t*)(void*)feature)
		printf(
			"Base ComID:      0x%04X\n"
			"Number of ComIDs:   %3d\n"
			"Range crossing:     %3d\n",
			featureOpal1->baseComID,
			featureOpal1->numberOfComIDs,
			featureOpal1->rangeCrossing);
		break;
	}
}
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
void TCGS_PrintLevel0Discovery(void* payload)
{
	TCGS_Level0Discovery_Feature_t* iter;
#define header ((TCGS_Level0Discovery_Header_t*)payload)

	printf("LEVEL 0 DISCOVERY\n");

	//printf( "Length:             %3d\n"
	//		"Version Major:      %3d\n"
	//		"Version Minor:      %3d\n",
	//		header->length,
	//		header->versionMajor,
	//		header->versionMinor);

	iter = TCGS_GetLevel0DiscoveryFirstFeatureHeater(payload);
	while (iter != NULL)
	{
		TCGS_PrintLevel0DiscoveryFeature(iter);
		iter = TCGS_GetLevel0DiscoveryNextFeatureHeater(payload, iter);
	}

	return;
}

#endif //defined(TCGS_VERBOSE)


