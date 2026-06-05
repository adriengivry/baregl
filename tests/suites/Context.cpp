/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <catch2/catch_test_macros.hpp>

#include <common/Boilerplate.h>

using namespace baregl;
using namespace tests::common::boilerplate;

TEST_CASE( "Context can be created", "[context]" ) {
	RunInWindow([](GLFWwindow* p_window) {
		Context context;
	});
}

TEST_CASE( "Context can be recreated", "[context]" ) {
	RunInWindow([](GLFWwindow* p_window) {
		auto context = std::make_unique<Context>();
		context.reset();
		context = std::make_unique<Context>();
	});
}

