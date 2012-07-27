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

TCGS_Level0Discovery_Header_t* TCGS_GetLevel0DiscoveryHeader(void* payload);

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryFirstFeatureHeater(void* payload);

TCGS_Level0Discovery_Feature_t* TCGS_GetLevel0DiscoveryNextFeatureHeater(void* payload, TCGS_Level0Discovery_Feature_t* featureHeader);

void* TCGS_GetLevel0DiscoveryFeatureHeader(void* payload, TCGS_Level0Discovery_FeatureCode_t featureCode);

#define TCGS_GetLevel0DiscoveryFeatureTperHeader(payload) ((TCGS_Level0Discovery_FeatureTper_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_TPER))

#define TCGS_GetLevel0DiscoveryFeatureLockingHeader(payload) ((TCGS_Level0Discovery_FeatureLocking_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_LOCKING))

#define TCGS_GetLevel0DiscoveryFeatureGeometryHeader(payload) ((TCGS_Level0Discovery_FeatureGeometry_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_GEOMETRY))

#define TCGS_GetLevel0DiscoveryFeatureEnterpriseHeader(payload) ((TCGS_Level0Discovery_FeatureEnterprise_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_ENTERPRISE))

#define TCGS_GetLevel0DiscoveryFeatureOpal1Header(payload) ((TCGS_Level0Discovery_FeatureOpal1_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_OPAL1))

#define TCGS_GetLevel0DiscoveryFeatureOpal2Header(payload) ((TCGS_Level0Discovery_FeatureOpal1_t*)TCGS_GetLevel0DiscoveryFeatureHeader(payload, FEATURE_OPAL2))


#endif /* TCGS_PARSER_H_ */
