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
#include "tcgs_interface.h"

/**
 * \brief Test for Level0Discovery command
 */
void test_tcgs_host_level0discovery(void **state)
{
	TCGS_CommandBlock_t commandBlock;
    TCGS_PrepareInterfaceCommand(LEVEL0_DISCOVERY, NULL, &commandBlock, NULL);
	assert_int_equal(commandBlock.command,   IF_RECV);
	assert_int_equal(commandBlock.protocolId, 0x01);
	assert_int_equal(commandBlock.length,     0x01);
	assert_int_equal(commandBlock.comId,      0x01);
}                     

/**
 * \brief Test for Level0Discovery command send to transport interface
 */
void test_tcgs_host_level0discovery_virtual(void **state)
{
	TCGS_CommandBlock_t commandBlock;
	TCGS_TPerError_t error;
	TCGS_Error_t status;
	uint8 output[200];

    TCGS_PrepareInterfaceCommand(LEVEL0_DISCOVERY, NULL, &commandBlock, NULL);
    TCGS_SetInterface(INTERFACE_VIRTUAL);
    status = TCGS_SendCommand(&commandBlock, NULL, &error, &output);
    assert_int_equal(error, INTERFACE_ERROR_GOOD);
    assert_int_equal(status, ERROR_SUCCESS);
}

int main(int argc, char* argv[]) {
    const UnitTest tests[] = {
        unit_test(test_tcgs_host_level0discovery),
        unit_test(test_tcgs_host_level0discovery_virtual),
    };
	int result;

	TCGS_InitHost();
	TCGS_ResetHost();
    result = run_tests(tests);
	TCGS_DestroyHost();
	return result;
}
