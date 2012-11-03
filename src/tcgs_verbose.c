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
#include "tcgs_interface.h"

#if defined(TCGS_VERBOSE)

static const char * const commandVerboseMap[IF_LAST] =
{
	"IF_SEND",
	"IF_RECV",
};

char* TCGS_Verbose_GetCommand (TCGS_Command_t command, char* commandBuf)
{
	if (command < IF_LAST)
	{
		sprintf(commandBuf, "%s", commandVerboseMap[command]);
	}
	else
	{
		sprintf(commandBuf, "%7d", command);
	}

	return commandBuf;
}

#define MAX_FEATURE_NAME_LENGTH sizeof("ENTERPRISE")
typedef struct
{
	const TCGS_Level0Discovery_FeatureCode_t featureCode;
	const char featureName[MAX_FEATURE_NAME_LENGTH];
} featureVerboseMap_t;

static const featureVerboseMap_t featureVerboseMap[] =
{
	{FEATURE_RESERVED,  "RESERVED"},
	{FEATURE_TPER,      "TPER"},
	{FEATURE_LOCKING,	"LOCKING"},
	{FEATURE_GEOMETRY,	"GEOMETRY"},
	{FEATURE_ENTERPRISE,"ENTERPRISE"},
	{FEATURE_OPAL1,     "OPAL1"},
	{FEATURE_OPAL2,		"OPAL2"},
};

const char * TCGS_Verbose_GetFeature (TCGS_Level0Discovery_FeatureCode_t featureCode, char* featureBuf)
{
	const char *result = NULL;
	int i;

	for (i = 0; i < sizeof(featureVerboseMap) / sizeof(featureVerboseMap[0]); i++)
	{
		if (featureVerboseMap[i].featureCode == featureCode)
		{
			result = featureVerboseMap[i].featureName;
		}
	}

	if (result != NULL)
	{
		snprintf(featureBuf, MAX_FEATURE_NAME_LENGTH + 1, "%*s", (int)MAX_FEATURE_NAME_LENGTH, result);
	}
	else
	{
		snprintf(featureBuf, MAX_FEATURE_NAME_LENGTH + 1, "0x%X", featureCode);
	}

	return featureBuf;
}


/*****************************************************************************
 * \brief Print content of interface command block
 *
 * @param[in]  command  Pointer to command block structure
 *
 * \return None
 *****************************************************************************/
void TCGS_PrintCommand(TCGS_CommandBlock_t* command)
{
	char commandBuf[sizeof(commandVerboseMap[0])];

	printf( "Command:          %s\n"
			"Protocol ID:         %4d\n"
			"Transfer Length:     %4d\n"
			"ComID:             0x%04X\n",
			TCGS_Verbose_GetCommand(command->command, commandBuf),
			command->protocolId,
			command->length,
			command->comId);
}

void TCGS_PrintLevel0DiscoveryFeature(TCGS_Level0Discovery_Feature_t *feature)
{
	char featureBuf[MAX_FEATURE_NAME_LENGTH + 1];

	printf(TCGS_VERBOSE_BLOCK_SEPARATOR "\n");
	printf( "Feature Code: %*s\n"
			"Version:              %3d\n"
			"Length:               %3d\n",
			(int)MAX_FEATURE_NAME_LENGTH, TCGS_Verbose_GetFeature((TCGS_Level0Discovery_FeatureCode_t)feature->code, featureBuf),
			feature->version,
			feature->length);
	
	switch((TCGS_Level0Discovery_FeatureCode_t)feature->code)
	{
	case FEATURE_TPER:
		#define featureTPer ((TCGS_Level0Discovery_FeatureTper_t*)(void*)feature)
		printf( "Sync Supported:         %d\n"
				"Async Supported:        %d\n"
				"ACK/NAK Supported:      %d\n"
				"Buf Mgmt Supported:     %d\n"
				"Streaming Supported:    %d\n"
				"ComID Mgmt Supported:   %d\n",
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
			"Locking Supported:      %d\n"
			"Locking Enabled:        %d\n"
			"Locked:                 %d\n"
			"Media Encr. Locked:     %d\n"
			"MBR Enabled:            %d\n"
			"MBR Done:               %d\n",
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
			"Base ComID:        0x%04X\n"
			"Number of ComIDs:     %3d\n"
			"Range crossing:       %3d\n",
			featureOpal1->baseComID,
			featureOpal1->numberOfComIDs,
			featureOpal1->rangeCrossing);
		break;
	case FEATURE_OPAL2:
		#define featureOpal2 ((TCGS_Level0Discovery_FeatureOpal2_t*)(void*)feature)
		printf(
			"Base ComID:        0x%04X\n"
			"Number of ComIDs:     %3d\n"
			"Range crossing:       %3d\n"
			"Num. of Admins:       %3d\n"
			"Num. of Users:        %3d\n"
			"InitialPinSidIndic.:  %3d\n"
			"BehaviorPinSinRevert: %3d\n",
			featureOpal2->baseComID,
			featureOpal2->numberOfComIDs,
			featureOpal2->rangeCrossing,
			featureOpal2->numberOfAdminsSupported,
			featureOpal2->numberOfUsersSupported,
			featureOpal2->initialPinSidIndicator,
			featureOpal2->behaviorPinSinRevert);
		break;
	case FEATURE_ENTERPRISE:
		#define featureEnterprise ((TCGS_Level0Discovery_FeatureEnterprise_t*)(void*)feature)
		printf(
			"Base ComID:        0x%04X\n"
			"Number of ComIDs:     %3d\n"
			"Range crossing:       %3d\n",
			featureEnterprise->baseComID,
			featureEnterprise->numberOfComIDs,
			featureEnterprise->rangeCrossing);
		break;
	default:
		//do nothing
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
void TCGS_PrintLevel0Discovery(TCGS_Level0Discovery_Header_t* payload)
{
	TCGS_Level0Discovery_Feature_t* iter;

	printf("LEVEL 0 DISCOVERY\n");

	//printf( "Length:             %3d\n"
	//		"Version Major:      %3d\n"
	//		"Version Minor:      %3d\n",
	//		header->length,
	//		header->versionMajor,
	//		header->versionMinor);

	iter = TCGS_GetLevel0DiscoveryFirstFeatureHeader(payload);
	while (iter != NULL)
	{
		TCGS_PrintLevel0DiscoveryFeature(iter);
		iter = TCGS_GetLevel0DiscoveryNextFeatureHeader(payload, iter);
	}

	return;
}

#endif //defined(TCGS_VERBOSE)


