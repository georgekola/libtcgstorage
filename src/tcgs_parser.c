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
 * \brief Extracts Level 0 Discovery header out of command payload
 *
 * \par The function returns a pointer to internal buffer with Level 0 Discovery
 * header data.
 *
 * \par TCGS_Level0Discovery shall be called before.
 *
 * \return [IN] TCGS_Level0Discovery_Header_t* pointer to Level 0 Discovery header data
 *
 * \see TCGS_Level0Discovery
 *****************************************************************************/
TCGS_Level0Discovery_Header_t* TCGS_GetLevel0DiscoveryHeader(void* payload)
{
	return (TCGS_Level0Discovery_Header_t*) payload;
}

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryFirstFeatureHeater(void* payload)
{
	uint32 payloadLength = ((TCGS_Level0Discovery_Header_t*)payload)->length;
	uint32 prevFeaturesLength = sizeof(TCGS_Level0Discovery_Header_t);
	TCGS_Level0Discovery_Feature_t *feature;

	//if there is space in the payload after the main header
	if (payloadLength > prevFeaturesLength)
	{
		feature = (TCGS_Level0Discovery_Feature_t*) (payload + prevFeaturesLength);
		//if feature suits payload
		if (feature->length > 0 && (payloadLength >= prevFeaturesLength + feature->length))
		{
			return feature;
		}
	}
	return NULL;
}

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryNextFeatureHeater(void* payload, TCGS_Level0Discovery_Feature_t* featureHeader)
{
	uint32 payloadLength = ((TCGS_Level0Discovery_Header_t*)payload)->length;
	uint32 prevFeaturesLength = ((uint32)((void*)featureHeader - payload) + featureHeader->length);
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
 * \return void* pointer to Level 0 Discovery feature header data, NULL is returned
 * when feature with specified code is not included in response
 *
 * \see TCGS_Level0Discovery
 *****************************************************************************/
void* TCGS_GetLevel0DiscoveryFeatureHeader(void* payload, TCGS_Level0Discovery_FeatureCode_t featureCode)
{
	void* iter;

	//Note: Field length of Level0Discovery header indicates the total number of bytes that are valid in
	//the Level 0 Discovery header and all of the feature descriptors returned, not including this field
	for(
			iter = payload + sizeof(TCGS_Level0Discovery_Header_t);
			iter < payload + ((TCGS_Level0Discovery_Header_t*)payload)->length;
			iter = ((uint8*)iter + ((TCGS_Level0Discovery_Feature_t*)iter)->length + sizeof(TCGS_Level0Discovery_Feature_t)))
	{
		if (featureCode == ((TCGS_Level0Discovery_Feature_t*)iter)->code)
		{
			return iter;
		}
	}

	return NULL;
}
