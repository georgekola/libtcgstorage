#include "libtcgstorage.h"
#include "utest.h"
    
/**
 * \brief Test for the base TCG Host interface functions
 */
void test_tcgs_host_basic(void)
{
	TCGS_InitHost();
	TCGS_ResetHost();
	TCGS_DestroyHost();
}                     

int main(void)
{
	run_test("Basic TCG Storage Host API test:", assert(true));
	run_end();
}