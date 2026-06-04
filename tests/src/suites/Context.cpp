/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <catch2/catch_test_macros.hpp>

#include <common/BareGL.h>

using namespace baregl;

TEST_CASE( "Context can be created", "[context]" ) {
	tests::baregl::RunInContext([](GLFWwindow* p_window) {
		Context context;
	});
}

TEST_CASE( "Context is alone", "[context]" ) {
	tests::baregl::RunInContext([](GLFWwindow* p_window) {
		Context context;
		REQUIRE_THROWS( std::make_unique<baregl::Context>() );
	});
}

TEST_CASE( "Context can be recreated", "[context]" ) {
	tests::baregl::RunInContext([](GLFWwindow* p_window) {
		auto context = std::make_unique<Context>();
		context.reset();
		context = std::make_unique<Context>();
	});
}

