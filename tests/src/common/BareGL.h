/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <functional>

#include <baregl/BareGL.h>

typedef struct GLFWwindow GLFWwindow;

namespace tests::baregl
{
	void RunInContext(std::function<void(GLFWwindow*)> p_callback);
}

