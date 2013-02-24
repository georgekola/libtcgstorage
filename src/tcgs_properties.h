/////////////////////////////////////////////////////////////////////////////
/// tcgs_properties.h
///
/// Properties management
///
/// (c) Artem Zankovich, 2013
/////////////////////////////////////////////////////////////////////////////

#ifndef _TCGS_PROPERTIES_H
#define _TCGS_PROPERTIES_H

#include "tcgs_types.h"

#define MAX_PROPERTIES_NAME_LENGHT 32 + 1

typedef uint32 TCGS_Property_Value_t;
typedef char TCGS_Property_Name_t[MAX_PROPERTIES_NAME_LENGHT];

typedef struct TCGS_Property_t
{
	TCGS_Property_Name_t	name;
	TCGS_Property_Value_t	value;
} TCGS_Property;

typedef struct TCGS_Properties_t {
	void    *properties;
    void    *iterator;
	uint32  size;
} TCGS_Properties;

/*
 * \brief Init known host properties lists with default values
 *
 * \returns STATUS_OK if initialization completes successfully, STATUS_ERROR otherwise
 */
TCGS_Error_t TCGS_init_properties();

/*
 * \brief Get current host properties list.
 *
 * \returns properties list
 */
TCGS_Properties *TCGS_get_properties_host();

/*
 * \brief Get a property value from a properties list. Property name is used as a search key.
 *
 * \returns property if it is found, NULL otherwise
 */
TCGS_Property *TCGS_get_property_by_name(TCGS_Properties * const properties, const TCGS_Property_Name_t name);

/*
 * \brief Adds new property into the properties list
 *
 * \returns STATUS_OK if append completes successfully, STATUS_ERROR otherwise
 */
TCGS_Error_t TCGS_append_property(TCGS_Properties *properties, TCGS_Property *property);

/*
 * \brief Sets iterator to the first property in the list
 *
 * \returns the first property in the list, NULL if properties list is empty
 */
TCGS_Property *TCGS_get_property_first(TCGS_Properties *properties);

/*
 * \brief Sets iterator to the next property in the list
 *
 * \returns current property in the list, NULL if there is no more properties in the list
 */
TCGS_Property *TCGS_get_property_next(TCGS_Properties *properties);

/*
 * \brief Destroy properties list
 */
void TCGS_free_properties(TCGS_Properties *properties);

#endif //_TCGS_PROPERTIES_H
