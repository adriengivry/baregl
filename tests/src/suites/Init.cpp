/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <catch2/catch_test_macros.hpp>

#include <common/BareGL.h>

TEST_CASE( "Context and window created", "[init]" ) {
	tests::baregl::RunInContext([](GLFWwindow* p_window) {
		baregl::Context context;
	});
}
