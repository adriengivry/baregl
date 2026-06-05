/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <common/Boilerplate.h>

#include <GLFW/glfw3.h>

namespace tests::common::boilerplate
{
	void RunInWindow(std::function<void(GLFWwindow*)> p_callback)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(256, 256, "test", nullptr, nullptr);
		glfwMakeContextCurrent(window);

		p_callback(window);

		glfwTerminate();
	}

	void RunInContext(std::function<void(GLFWwindow*, baregl::Context&)> p_callback)
	{
		RunInWindow([&](GLFWwindow* p_window){
			baregl::Context context(
				baregl::data::ContextDesc{
					.debug = true
				}
			);

			p_callback(p_window, context);
		});
	}
}

