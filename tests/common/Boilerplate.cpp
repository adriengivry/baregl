/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <common/Boilerplate.h>

#include <GLFW/glfw3.h>

namespace tests::common::boilerplate
{
	class ThrowingLogHandler final : public baregl::debug::ILogHandler
	{
	public:
		virtual void LogInfo(const std::string_view p_message) override
		{
			fprintf(
				stdout,
				"[baregl] <info> %s\n",
				p_message.data()
			);
		}

		virtual void LogWarning(const std::string_view p_message) override
		{
			fprintf(
				stdout,
				"[baregl] <warning> %s\n",
				p_message.data()
			);
		}

		virtual void LogError(const std::string_view p_message) override
		{
			fprintf(
				stderr,
				"[baregl] <error> %s\n",
				p_message.data()
			);

			throw std::runtime_error(std::string{p_message});
		}
	};

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
			baregl::debug::SetLogHandler(std::make_unique<ThrowingLogHandler>());

			baregl::Context context(
				baregl::data::ContextDesc{
					.debug = true
				}
			);

			p_callback(p_window, context);
		});
	}
}

