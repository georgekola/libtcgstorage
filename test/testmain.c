#include "libtcgstorage.h" 

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
   
 
/**
 * \brief Test for the base TCG Host interface functions
 */
void test_tcgs_host_basic_api(void **state)
{
	TCGS_InitHost();
	TCGS_ResetHost();
	TCGS_DestroyHost();  
	assert(false);
}                     


int main(int argc, char* argv[]) {
    const UnitTest tests[] = {
        unit_test(test_tcgs_host_basic_api),
    };
    return run_tests(tests);
}