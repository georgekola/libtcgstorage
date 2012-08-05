/////////////////////////////////////////////////////////////////////////////
/// tcgs_parser.h
///
/// Parser for responses of TPer
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "tcgs_parser.h"
#include "tcgs_stream.h"

/*****************************************************************************
 * \brief Extracts the first Level 0 Discovery feature header from command
 * payload
 *
 * \par The function returns a pointer to internal buffer with the first
 * feature code from Level 0 Discovery command payload.
 *
 * \par TCGS_Level0Discovery shall be called before.
 *
 * @param[in]  payload      Pointer to payload returned by TCGS_Level0Discovery
 *
 * \return TCGS_Level0Discovery_Feature_t* pointer to feature header
 *
 * \see TCGS_Level0Discovery
 *****************************************************************************/
TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryFirstFeatureHeader(TCGS_Level0Discovery_Header_t* payload)
{
	uint32 payloadLength = payload->length;
	uint32 prevFeaturesLength = sizeof(TCGS_Level0Discovery_Header_t);
	TCGS_Level0Discovery_Feature_t *feature;

	//if there is space in the payload after the main header
	if (payloadLength > prevFeaturesLength)
	{
		feature = (TCGS_Level0Discovery_Feature_t*) ((void*)payload + prevFeaturesLength);
		//if feature suits payload
		if (feature->length > 0 && (payloadLength >= prevFeaturesLength + feature->length))
		{
			return feature;
		}
	}
	return NULL;
}

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryNextFeatureHeader(TCGS_Level0Discovery_Header_t* payload, TCGS_Level0Discovery_Feature_t* featureHeader)
{
	uint32 payloadLength = payload->length;
	uint32 prevFeaturesLength = ((uint32)((void*)featureHeader - (void*)payload) + featureHeader->length);
	TCGS_Level0Discovery_Feature_t *feature;

	//if there is more space in the payload after the current feature header
	if (payloadLength > prevFeaturesLength)
	{
		feature = (TCGS_Level0Discovery_Feature_t*)((void*)featureHeader + featureHeader->length + sizeof(TCGS_Level0Discovery_Feature_t));
		//if feature suits payload
		if (feature->length > 0 && (payloadLength >= prevFeaturesLength + feature->length))
		{
			return feature;
		}
	}
	return NULL;
}

/*****************************************************************************
 * \brief Return Level 0 Discovery feature header with specified code
 *
 * \par The function returns a pointer to internal buffer with Level 0 Discovery
 * feature header data.
 *
 * \par TCGS_Level0Discovery shall be called before.
 *
 * @param[in]  payload      Pointer to payload returned by TCGS_Level0Discovery
 * @param[in]  featureCode  Code of the feature
 *
 * \return void* pointer to Level 0 Discovery feature header data, NULL is returned
 * when feature with specified code is not included in response
 *
 * \see TCGS_Level0Discovery, TCGS_Level0Discovery_FeatureCode_t
 *****************************************************************************/
TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryFeatureHeader(
		TCGS_Level0Discovery_Header_t* payload, TCGS_Level0Discovery_FeatureCode_t featureCode)
{
	TCGS_Level0Discovery_Feature_t* iter;

	iter = TCGS_GetLevel0DiscoveryFirstFeatureHeader(payload);
	while (iter != NULL)
	{
		if (iter->code == featureCode)
		{
			return iter;
		}
	}

	return NULL;
}
