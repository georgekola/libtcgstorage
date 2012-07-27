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
