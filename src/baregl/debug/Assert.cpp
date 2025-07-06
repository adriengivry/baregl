/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <cassert>

#include <baregl/debug/Assert.h>

void baregl::debug::Assert(bool condition, const std::string_view p_message)
{
	assert(condition && p_message.data());
}
