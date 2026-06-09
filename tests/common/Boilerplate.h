/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <functional>

#include <baregl/BareGL.h>

typedef struct GLFWwindow GLFWwindow;

namespace tests::common::boilerplate
{
	void RunInWindow(std::function<void(GLFWwindow*)> p_callback);
	void RunInContext(std::function<void(GLFWwindow*, baregl::Context&)> p_callback);
}

