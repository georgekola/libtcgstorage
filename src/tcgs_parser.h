/////////////////////////////////////////////////////////////////////////////
/// tcgs_parser.h
///
/// Parser for responses of TPer
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#ifndef TCGS_PARSER_H_
#define TCGS_PARSER_H_

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
TCGS_Level0Discovery_Header_t* TCGS_GetLevel0DiscoveryHeader(void* payload);

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryFirstFeatureHeater(void* payload);

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryNextFeatureHeater(void* payload, TCGS_Level0Discovery_Feature_t* featureHeader);

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
void* TCGS_GetLevel0DiscoveryFeatureHeader(void* payload, TCGS_Level0Discovery_FeatureCode_t featureCode);

#define TCGS_GetLevel0DiscoveryFeatureTperHeader(payload) ((TCGS_Level0Discovery_FeatureTper_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_TPER))

#define TCGS_GetLevel0DiscoveryFeatureLockingHeader(payload) ((TCGS_Level0Discovery_FeatureLocking_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_LOCKING))

#define TCGS_GetLevel0DiscoveryFeatureGeometryHeader(payload) ((TCGS_Level0Discovery_FeatureGeometry_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_GEOMETRY))

#define TCGS_GetLevel0DiscoveryFeatureEnterpriseHeader(payload) ((TCGS_Level0Discovery_FeatureEnterprise_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_ENTERPRISE))

#define TCGS_GetLevel0DiscoveryFeatureOpal1Header(payload) ((TCGS_Level0Discovery_FeatureOpal1_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_OPAL1))

#define TCGS_GetLevel0DiscoveryFeatureOpal2Header(payload) ((TCGS_Level0Discovery_FeatureOpal1_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_OPAL2))


#endif /* TCGS_PARSER_H_ */
