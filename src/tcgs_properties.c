/////////////////////////////////////////////////////////////////////////////
/// tcgs_properties.h
///
/// Properties management implementation
///
/// (c) Artem Zankovich, 2013
/////////////////////////////////////////////////////////////////////////////

#include "tcgs_properties.h"
#include "tcgs_memory.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//list of static properties for communication minimum
 const TCGS_Property properties_minimum[] =
{
	{"asdf", 12},
	{"gbk", 46},
	{"jhj", 98},
	{"kj", 44},
};

//element of properties list
typedef struct TCGS_Properties_Element_t {
	TCGS_Property data;
	struct TCGS_Properties_Element_t *next;
} TCGS_Properties_Element;

//terminator-based linked list of TCGS_Properties_Element eliments
static TCGS_Properties properties_host;

//set properties iterator to the first element
static void properties_iterator_set_first(TCGS_Properties_Element **iterator, TCGS_Properties *properties)
{
    //set iterator to the first element
    *iterator = properties->properties;    
}

//move properties iterator forward
static void properties_iterator_move_next(TCGS_Properties_Element **iterator)
{
    *iterator = (*iterator)->next;
}

//check that the list has more elements
static bool properties_iterator_check_last(TCGS_Properties_Element *iterator)
{
    return iterator == NULL || iterator->next == NULL; 
}

//get property currently reffered by properties iterator
static TCGS_Property *properties_iterator_get_current(TCGS_Properties_Element *iterator)
{
    if (properties_iterator_check_last(iterator)) {
        return NULL;
    } else {
        return (TCGS_Property*)&iterator->data;
    }
}

//add new property just before the provided one
static TCGS_Properties_Element *TCGS_add_property(TCGS_Properties_Element *property_element, TCGS_Property *property)
{
	TCGS_Properties_Element *property_element_new = (TCGS_Properties_Element*) allocate(sizeof(TCGS_Properties_Element));
	
	//copy old property data to the new one (it should be list separator)
	memcpy(property_element, property_element_new, sizeof(TCGS_Properties_Element));
    
	//set new value to the old property data
	memcpy(&(property_element->data), property, sizeof(TCGS_Property));
	property_element->next = property_element_new;
    
	return property_element_new;
}

/*
 * \brief Init properties
 *
 * \returns STATUS_OK if initialisation completes successfully, STATUS_ERROR otherwise
 */
TCGS_Error_t TCGS_init_properties()
{
	int i;
	TCGS_Properties_Element *tail;

	// initialise properties lists with terminators
	properties_host.properties = callocate(sizeof(TCGS_Property));
    tail = (TCGS_Properties_Element*) properties_host.properties;

	for (i = 0; i < SIZEOF_ARRAY(properties_minimum); i++) {
		if ((tail = TCGS_add_property(tail, (TCGS_Property*)&properties_minimum[i])) == NULL) {
			return ERROR_TPER;
		}
		properties_host.size++;
	}

	return ERROR_SUCCESS;
}

/*
 * \brief Get current host properties list.
 *
 * \returns properties list
 */
TCGS_Properties *TCGS_get_properties_host()
{
    return &properties_host;
}

/*
 * \brief Get a property value from a properties list. Property name is used as a search key.
 *
 * \returns property if it is found, NULL otherwise
 */
TCGS_Property *TCGS_get_property_by_name(TCGS_Properties * const properties, const TCGS_Property_Name_t name)
{
    TCGS_Properties_Element *iterator;
    
    properties_iterator_set_first(&iterator, properties);    
    while (!properties_iterator_check_last(iterator)) {
        if (strncmp(properties_iterator_get_current(iterator)->name, name, MAX_PROPERTIES_NAME_LENGHT) == 0) {
            return (TCGS_Property*)&iterator->data;
        }
        properties_iterator_move_next(&iterator);
    }
    
    return NULL;
}

/*
 * \brief Adds new property into the properties list
 *
 * \returns STATUS_OK if append completes successfully, STATUS_ERROR otherwise
 */
TCGS_Error_t TCGS_append_property(TCGS_Properties *properties, TCGS_Property *property)
{
    TCGS_Properties_Element *iterator;
    
    properties_iterator_set_first(&iterator, properties);
    if (iterator == NULL) {
        return ERROR_TPER;
    }
    while (!properties_iterator_check_last(iterator)) {
        properties_iterator_move_next(&iterator);
    }
    if (TCGS_add_property(iterator, property) == NULL) {
        return ERROR_TPER;
    }
    properties->size++;
    
    return ERROR_SUCCESS;       
}

/*
 * \brief Sets iterator to the first property in the list
 *
 * \returns the first property in the list, NULL if properties list is empty
 */
TCGS_Property *TCGS_get_property_first(TCGS_Properties *properties)
{
    properties_iterator_set_first((TCGS_Properties_Element**)&properties->iterator, properties);
    return properties_iterator_get_current(properties->iterator);
}

/*
 * \brief Sets iterator to the next property in the list
 *
 * \returns current property in the list, NULL if there is no more properties in the list
 */
TCGS_Property *TCGS_get_property_next(TCGS_Properties *properties)
{
    if (properties_iterator_check_last(properties->iterator)) {
        return NULL;
    }
    properties_iterator_move_next((TCGS_Properties_Element**)&properties->iterator);
    return properties_iterator_get_current(properties->iterator);
}

/*
 * \brief Destroy properties
 */
void TCGS_free_properties(TCGS_Properties *properties)
{
    TCGS_Properties_Element *element;
    properties_iterator_set_first((TCGS_Properties_Element**)&properties->iterator, properties);
    while (properties_iterator_check_last(properties->iterator)) {
        element = properties->iterator;
        properties_iterator_move_next((TCGS_Properties_Element**)&properties->iterator);
        free(element);
    }
}
