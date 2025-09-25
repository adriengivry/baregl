/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/debug/Log.h>
#include <baregl/debug/Assert.h>
#include <baregl/detail/Types.h>
#include <baregl/detail/glad/glad.h>
#include <baregl/Context.h>
#include <baregl/math/Conversions.h>
#include <baregl/utils/BitmaskOperators.h>

namespace
{
	void GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string output;

		output += "OpenGL Debug Message:\n";
		output += "Debug message (" + std::to_string(id) + "): " + message + "\n";

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:				output += "Source: API";				break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		output += "Source: Window System";		break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	output += "Source: Shader Compiler";	break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		output += "Source: Third Party";		break;
		case GL_DEBUG_SOURCE_APPLICATION:		output += "Source: Application";		break;
		case GL_DEBUG_SOURCE_OTHER:				output += "Source: Other";				break;
		}

		output += "\n";

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               output += "Type: Error";				break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Type: Undefined Behaviour";	break;
		case GL_DEBUG_TYPE_PORTABILITY:         output += "Type: Portability";			break;
		case GL_DEBUG_TYPE_PERFORMANCE:         output += "Type: Performance";			break;
		case GL_DEBUG_TYPE_MARKER:              output += "Type: Marker";				break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Type: Push Group";			break;
		case GL_DEBUG_TYPE_POP_GROUP:           output += "Type: Pop Group";			break;
		case GL_DEBUG_TYPE_OTHER:               output += "Type: Other";				break;
		}

		output += "\n";

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			output += "Severity: High";				break;
		case GL_DEBUG_SEVERITY_MEDIUM:			output += "Severity: Medium";			break;
		case GL_DEBUG_SEVERITY_LOW:				output += "Severity: Low";				break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	output += "Severity: Notification";		break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			BAREGL_LOG_ERROR(output);	break;
		case GL_DEBUG_SEVERITY_MEDIUM:			BAREGL_LOG_WARNING(output);	break;
		case GL_DEBUG_SEVERITY_LOW:				BAREGL_LOG_INFO(output);		break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	BAREGL_LOG_INFO(output);		break;
		}
	}

	bool GetBool(uint32_t p_parameter)
	{
		GLboolean result;
		glGetBooleanv(p_parameter, &result);
		return static_cast<bool>(result);
	}

	bool GetBool(uint32_t p_parameter, uint32_t p_index)
	{
		GLboolean result;
		glGetBooleani_v(p_parameter, p_index, &result);
		return static_cast<bool>(result);
	}

	int GetInt(uint32_t p_parameter)
	{
		GLint result;
		glGetIntegerv(p_parameter, &result);
		return static_cast<int>(result);
	}

	int GetInt(uint32_t p_parameter, uint32_t p_index)
	{
		GLint result;
		glGetIntegeri_v(p_parameter, p_index, &result);
		return static_cast<int>(result);
	}

	float GetFloat(uint32_t p_parameter)
	{
		GLfloat result;
		glGetFloatv(p_parameter, &result);
		return static_cast<float>(result);
	}

	float GetFloat(uint32_t p_parameter, uint32_t p_index)
	{
		GLfloat result;
		glGetFloati_v(p_parameter, p_index, &result);
		return static_cast<float>(result);
	}

	double GetDouble(uint32_t p_parameter)
	{
		GLdouble result;
		glGetDoublev(p_parameter, &result);
		return static_cast<double>(result);
	}

	double GetDouble(uint32_t p_parameter, uint32_t p_index)
	{
		GLdouble result;
		glGetDoublei_v(p_parameter, p_index, &result);
		return static_cast<double>(result);
	}

	int64_t GetInt64(uint32_t p_parameter)
	{
		GLint64 result;
		glGetInteger64v(p_parameter, &result);
		return static_cast<int64_t>(result);
	}

	int64_t GetInt64(uint32_t p_parameter, uint32_t p_index)
	{
		GLint64 result;
		glGetInteger64i_v(p_parameter, p_index, &result);
		return static_cast<int64_t>(result);
	}

	std::string GetString(uint32_t p_parameter)
	{
		const GLubyte* result = glGetString(p_parameter);
		return result ? reinterpret_cast<const char*>(result) : std::string();
	}

	std::string GetString(uint32_t p_parameter, uint32_t p_index)
	{
		const GLubyte* result = glGetStringi(p_parameter, p_index);
		return result ? reinterpret_cast<const char*>(result) : std::string();
	}
}

namespace baregl
{
	Context::Context(bool debug)
	{
		const int error = gladLoadGL();

		if (error == 0)
		{
			BAREGL_LOG_ERROR("GLAD failed to initialize");
			return;
		}

		BAREGL_LOG_INFO("OpenGL context initialized.");

		if (debug)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugMessageCallback, nullptr);
		}

		// Seamless cubemap (always on)
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
	}

	void Context::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
	{
		GLbitfield clearMask = 0;
		if (p_colorBuffer) clearMask |= GL_COLOR_BUFFER_BIT;
		if (p_depthBuffer) clearMask |= GL_DEPTH_BUFFER_BIT;
		if (p_stencilBuffer) clearMask |= GL_STENCIL_BUFFER_BIT;

		if (clearMask != 0)
		{
			glClear(clearMask);
		}
	}

	void Context::DrawElements(types::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount)
	{
		glDrawElements(utils::EnumToValue<GLenum>(p_primitiveMode), p_indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void Context::DrawElementsInstanced(types::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount, uint32_t p_instances)
	{
		glDrawElementsInstanced(utils::EnumToValue<GLenum>(p_primitiveMode), p_indexCount, GL_UNSIGNED_INT, nullptr, p_instances);
	}

	void Context::DrawArrays(types::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount)
	{
		glDrawArrays(utils::EnumToValue<GLenum>(p_primitiveMode), 0, p_vertexCount);
	}

	void Context::DrawArraysInstanced(types::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount, uint32_t p_instances)
	{
		glDrawArraysInstanced(utils::EnumToValue<GLenum>(p_primitiveMode), 0, p_vertexCount, p_instances);
	}

	void Context::DispatchCompute(uint32_t p_x, uint32_t p_y, uint32_t p_z) const
	{
		BAREGL_ASSERT(
			p_x > 0 && p_y > 0 && p_z > 0,
			"Dispatch work group count cannot be zero"
		);

		glDispatchCompute(p_x, p_y, p_z);
	}

	void Context::MemoryBarrier(types::EMemoryBarrierFlags p_barriers) const
	{
		glMemoryBarrier(
			utils::EnumToValue<GLbitfield>(p_barriers)
		);
	}

	void Context::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
	{
		glClearColor(p_red, p_green, p_blue, p_alpha);
	}

	void Context::SetRasterizationLinesWidth(float p_width)
	{
		glLineWidth(p_width);
	}

	void Context::SetRasterizationMode(types::ERasterizationMode p_rasterizationMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, utils::EnumToValue<GLenum>(p_rasterizationMode));
	}

	void Context::SetCapability(types::ERenderingCapability p_capability, bool p_value)
	{
		(p_value ? glEnable : glDisable)(utils::EnumToValue<GLenum>(p_capability));
	}

	bool Context::GetCapability(types::ERenderingCapability p_capability)
	{
		return glIsEnabled(utils::EnumToValue<GLenum>(p_capability));
	}

	void Context::SetStencilAlgorithm(types::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
	{
		glStencilFunc(utils::EnumToValue<GLenum>(p_algorithm), p_reference, p_mask);
	}

	void Context::SetDepthAlgorithm(types::EComparaisonAlgorithm p_algorithm)
	{
		glDepthFunc(utils::EnumToValue<GLenum>(p_algorithm));
	}

	void Context::SetStencilMask(uint32_t p_mask)
	{
		glStencilMask(p_mask);
	}

	void Context::SetStencilOperations(types::EOperation p_stencilFail, types::EOperation p_depthFail, types::EOperation p_bothPass)
	{
		glStencilOp(
			utils::EnumToValue<GLenum>(p_stencilFail),
			utils::EnumToValue<GLenum>(p_depthFail),
			utils::EnumToValue<GLenum>(p_bothPass)
		);
	}

	void Context::SetBlendingFunction(types::EBlendingFactor p_sourceFactor, types::EBlendingFactor p_destinationFactor)
	{
		glBlendFunc(
			utils::EnumToValue<GLenum>(p_sourceFactor),
			utils::EnumToValue<GLenum>(p_destinationFactor)
		);
	}

	void Context::SetBlendingEquation(types::EBlendingEquation p_equation)
	{
		glBlendEquation(utils::EnumToValue<GLenum>(p_equation));
	}

	void Context::SetCullFace(types::ECullFace p_cullFace)
	{
		glCullFace(utils::EnumToValue<GLenum>(p_cullFace));
	}

	void Context::SetDepthWriting(bool p_enable)
	{
		glDepthMask(p_enable);
	}

	void Context::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
	{
		glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
	}

	void Context::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	std::string Context::GetVendor()
	{
		return GetString(GL_VENDOR);
	}

	std::string Context::GetHardware()
	{
		return GetString(GL_RENDERER);
	}

	std::string Context::GetVersion()
	{
		return GetString(GL_VERSION);
	}

	std::string Context::GetShadingLanguageVersion()
	{
		return GetString(GL_SHADING_LANGUAGE_VERSION);
	}
}
