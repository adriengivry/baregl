/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/debug/Assert.h>

#include <cassert>

void baregl::debug::Assert(bool condition, const std::string_view p_message)
{
	assert(condition && p_message.data());
}
