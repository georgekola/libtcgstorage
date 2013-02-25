/////////////////////////////////////////////////////////////////////////////
/// testmain.c
///
/// Main file for UTs of lidtcgstorage
///
/// (c) Artem Zankovich, 2012
//////////////////////////////////////////////////////////////////////////////

//header to be included before cmockery.h
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <setjmp.h>
#include <google/cmockery.h>
#include <assert.h>

// If unit testing is enabled override assert with mock_assert().
#if UNIT_TESTING
extern void mock_assert(const int result, const char* const expression, 
                        const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif // UNIT_TESTING

#include "libtcgstorage.h"
#include "tcgs_stream.h"
#include "tcgs_builder.h"
#include "tcgs_parser.h"
#include "tcgs_interface.h"
#include "tcgs_interface_vtper.h"
#include "tcgs_interface_encode.h"
#include "tcgs_command.h"
#include "tcgs_properties.h"

/**
 * \brief Test base types size
 */
void test_tcgs_basetypes_size(void **state)
{
    assert_int_equal(sizeof(uint8), 1);
    assert_int_equal(sizeof(uint16), 2);
    assert_int_equal(sizeof(uint32), 4);
    assert_int_equal(sizeof(uint64), 8);
}
/**
 * \brief Test for Level0Discovery command
 */
void test_tcgs_host_level0discovery(void **state)
{
	TCGS_CommandBlock_t commandBlock;

	TCGS_PrepareInterfaceCommand(LEVEL0_DISCOVERY, NULL, &commandBlock, NULL);
	assert_int_equal(commandBlock.command,    IF_RECV);
	assert_int_equal(commandBlock.protocolId, 0x01);
	assert_int_equal(commandBlock.length,     0x200);
	assert_int_equal(commandBlock.comId,      0x01);
}                     

/**
 * \brief Test for Level0Discovery command send to transport interface
 */
void test_tcgs_host_level0discovery_virtual(void **state)
{
	TCGS_CommandBlock_t commandBlock;
	TCGS_InterfaceError_t error;
	TCGS_Error_t status;
	uint8 output[512];
	TCGS_Level0Discovery_Header_t *header;
	TCGS_Level0Discovery_FeatureTper_t *headerTper;

    TCGS_PrepareInterfaceCommand(LEVEL0_DISCOVERY, NULL, &commandBlock, NULL);
    status = TCGS_Interface_IoCommand(&commandBlock, NULL, &error, &output);
    assert_int_equal(error, INTERFACE_ERROR_GOOD);
    assert_int_equal(status, ERROR_SUCCESS);
    header = TCGS_DecodeLevel0Discovery(&output);
    headerTper = TCGS_GetLevel0DiscoveryFeatureTperHeader(header);
    assert(header != NULL);
    assert_int_equal(header->versionMajor, 0);
    assert_int_equal(header->versionMinor, 1);
    assert(headerTper != NULL);
    assert_int_equal(headerTper->code, FEATURE_TPER);
    assert_int_equal(headerTper->length, 12);
}

void test_command(void **state)
{
    TCGS_Token_t *rootToken;

    rootToken = get_new_int_token(555);
}

void test_properties(void **state)
{
    TCGS_Properties *properties_host;
    TCGS_Property *property;
    TCGS_Property property_check = {"test", 1111};
    int count = 0;

    assert_int_equal(TCGS_init_properties(), ERROR_SUCCESS);

    properties_host = TCGS_get_properties_host();
    property = TCGS_get_property_by_name(properties_host, "test");
    assert_int_equal((int)property, (int)NULL);
    TCGS_append_property(properties_host, &property_check);

    property = TCGS_get_property_first(properties_host);
    while (property != NULL) {
        count++;
        property = TCGS_get_property_next(properties_host);
    }
    assert_int_equal(count, properties_host->size);

    property = TCGS_get_property_by_name(properties_host, "test");
    assert_int_not_equal((int)property, (int)NULL);
    assert_string_equal("test", property->name);
    assert_int_equal(1111, property->value);

    TCGS_free_properties(properties_host);
}

int main(int argc, char* argv[]) {
    const UnitTest tests[] = {
        unit_test(test_properties),
        unit_test(test_tcgs_basetypes_size),
        unit_test(test_tcgs_host_level0discovery),
        unit_test(test_tcgs_host_level0discovery_virtual),
        unit_test(test_command),
    };
	int result;

	TCGS_InitHost(&TCGS_VTper_InterfaceDescriptor);
	TCGS_ResetHost();
    result = run_tests(tests);
	TCGS_DestroyHost();
	return result;
}
