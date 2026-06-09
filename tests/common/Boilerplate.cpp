/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <common/Boilerplate.h>

#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <stdexcept>

namespace
{
	void LogInfo(const std::string_view p_message)
	{
		fprintf(
			stdout,
			"[baregl::tests] <info> %s\n",
			p_message.data()
		);
	}

	void LogWarning(const std::string_view p_message)
	{
		fprintf(
			stdout,
			"[baregl::tests] <warning> %s\n",
			p_message.data()
		);
	}

	void LogError(const std::string_view p_message)
	{
		fprintf(
			stderr,
			"[baregl::tests] <error> %s\n",
			p_message.data()
		);
	}
}

namespace tests::common::boilerplate
{
	class ThrowingLogHandler final : public baregl::debug::ILogHandler
	{
	public:
		virtual void LogInfo(const std::string_view p_message) override
		{
			::LogInfo(p_message);
		}

		virtual void LogWarning(const std::string_view p_message) override
		{
			::LogWarning(p_message);
		}

		virtual void LogError(const std::string_view p_message) override
		{
			::LogError(p_message);
			throw std::runtime_error(std::string{p_message});
		}
	};

	void RunInWindow(std::function<void(GLFWwindow*)> p_callback)
	{
		if (!glfwInit())
		{
			::LogError("GLFW init failed");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		GLFWwindow* window = glfwCreateWindow(256, 256, "test", nullptr, nullptr);
		if (!window)
		{
			::LogError(std::format("GLFW window creation failed {}", glfwGetError(nullptr)));
		}

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

