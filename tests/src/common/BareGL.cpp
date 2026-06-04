/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <common/BareGL.h>

#include <GLFW/glfw3.h>

namespace tests::baregl
{
	void RunInContext(std::function<void(GLFWwindow*)> p_callback)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(256, 256, "test", nullptr, nullptr);
		glfwMakeContextCurrent(window);

		class TestAssertHandler : public ::baregl::debug::IAssertHandler
		{
			void OnAssert(const std::string_view p_message) override
			{
				throw std::runtime_error(std::string{p_message});
			}
		};

		::baregl::debug::SetAssertHandler(std::make_unique<TestAssertHandler>());

		p_callback(window);

		glfwTerminate();
	}
}

