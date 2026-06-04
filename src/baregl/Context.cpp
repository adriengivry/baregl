/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/Context.h>

#include <baregl/debug/Assert.h>
#include <baregl/debug/Log.h>
#include <baregl/detail/Types.h>
#include <baregl/detail/glad/glad.h>
#include <baregl/math/Conversions.h>
#include <baregl/utils/BitmaskOperators.h>
#include <baregl/utils/EnumMapper.h>

namespace
{
	uint32_t g_contextCount = 0u;

	void GLDebugMessageCallback(uint32_t p_source, uint32_t p_type, uint32_t p_id, uint32_t p_severity, int32_t p_length, const char* p_message, const void* p_userParam)
	{
		// Ignore non-significant error/warning codes
		if (p_id == 131169 || p_id == 131185 || p_id == 131218 || p_id == 131204)
		{
			return;
		}

		std::string output = "OpenGL Debug Message:\n";
		output += "Debug message (" + std::to_string(p_id) + "): " + p_message + "\n";

		switch (p_source)
		{
		case GL_DEBUG_SOURCE_API:				output += "Source: API";				break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		output += "Source: Window System";		break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	output += "Source: Shader Compiler";	break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		output += "Source: Third Party";		break;
		case GL_DEBUG_SOURCE_APPLICATION:		output += "Source: Application";		break;
		case GL_DEBUG_SOURCE_OTHER:				output += "Source: Other";				break;
		}

		output += "\n";

		switch (p_type)
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

		switch (p_severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			output += "Severity: High";				BAREGL_LOG_ERROR(output);	break;
		case GL_DEBUG_SEVERITY_MEDIUM:			output += "Severity: Medium";			BAREGL_LOG_WARNING(output);	break;
		case GL_DEBUG_SEVERITY_LOW:				output += "Severity: Low";				BAREGL_LOG_INFO(output);	break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	output += "Severity: Notification";		BAREGL_LOG_INFO(output);	break;
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
	Context::Context(const baregl::data::ContextDesc& p_desc)
	{
		BAREGL_ASSERT(g_contextCount == 0, "A context already exists. BareGL currently only supports a single context.");

		++g_contextCount;

		const int error = gladLoadGL();

		if (error == 0)
		{
			BAREGL_LOG_ERROR("GLAD failed to initialize");
			return;
		}

		BAREGL_LOG_INFO("OpenGL context initialized.");

		if (p_desc.debug)
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

	Context::~Context()
	{
		BAREGL_ASSERT(g_contextCount > 0, "No context to destroy, did you modify g_contextCount manually?");

		--g_contextCount;
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
	
	template<>
	void Context::GetValue<int>(
		types::EGetParameter p_param,
		std::span<int> p_out
	)
	{
		glGetIntegerv(
			utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void Context::GetValueIndexed<int>(
		types::EGetParameter p_param,
		std::span<int> p_out,
		uint32_t p_index
	)
	{
		glGetIntegeri_v(
			utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void Context::GetValue<int64_t>(
		types::EGetParameter p_param,
		std::span<int64_t> p_out
	)
	{
		glGetInteger64v(
			utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void Context::GetValueIndexed<int64_t>(
		types::EGetParameter p_param,
		std::span<int64_t> p_out,
		uint32_t p_index
	)
	{
		glGetInteger64i_v(
			utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void Context::GetValue<bool>(
		types::EGetParameter p_param,
		std::span<bool> p_out
	)
	{
		glGetBooleanv(
			utils::EnumToValue<GLenum>(p_param),
			reinterpret_cast<GLboolean*>(p_out.data())
		);
	}

	template<>
	void Context::GetValueIndexed<bool>(
		types::EGetParameter p_param,
		std::span<bool> p_out,
		uint32_t p_index
	)
	{
		glGetBooleani_v(
			utils::EnumToValue<GLenum>(p_param),
			p_index,
			reinterpret_cast<GLboolean*>(p_out.data())
		);
	}

	template<>
	void Context::GetValue<float>(
		types::EGetParameter p_param,
		std::span<float> p_out
	)
	{
		glGetFloatv(
			utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void Context::GetValueIndexed<float>(
		types::EGetParameter p_param,
		std::span<float> p_out,
		uint32_t p_index
	)
	{
		glGetFloati_v(
			utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void Context::GetValue<double>(
		types::EGetParameter p_param,
		std::span<double> p_out
	)
	{
		glGetDoublev(
			utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void Context::GetValueIndexed<double>(
		types::EGetParameter p_param,
		std::span<double> p_out,
		uint32_t p_index
	)
	{
		glGetDoublei_v(
			utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

}
template<>
data::QueryResultType<types::EGetParameter::ACTIVE_TEXTURE> Context::Get<types::EGetParameter::ACTIVE_TEXTURE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::ACTIVE_TEXTURE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::ALIASED_LINE_WIDTH_RANGE> Context::Get<types::EGetParameter::ALIASED_LINE_WIDTH_RANGE>()
{
std::array<float, 2> values;
GetValue<float>(types::EGetParameter::ALIASED_LINE_WIDTH_RANGE, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::ARRAY_BUFFER_BINDING> Context::Get<types::EGetParameter::ARRAY_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::ARRAY_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::BLEND> Context::Get<types::EGetParameter::BLEND>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::BLEND, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_COLOR> Context::Get<types::EGetParameter::BLEND_COLOR>()
{
std::array<float, 4> values;
GetValue<float>(types::EGetParameter::BLEND_COLOR, values);
return math::Vec4(values[0], values[1], values[2], values[3]);
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_DST_ALPHA> Context::Get<types::EGetParameter::BLEND_DST_ALPHA>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::BLEND_DST_ALPHA, values);
return utils::ValueToEnum<EBlendingFactor>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_DST_RGB> Context::Get<types::EGetParameter::BLEND_DST_RGB>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::BLEND_DST_RGB, values);
return utils::ValueToEnum<EBlendingFactor>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_EQUATION_ALPHA> Context::Get<types::EGetParameter::BLEND_EQUATION_ALPHA>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::BLEND_EQUATION_ALPHA, values);
return utils::ValueToEnum<EBlendingEquation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_EQUATION_RGB> Context::Get<types::EGetParameter::BLEND_EQUATION_RGB>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::BLEND_EQUATION_RGB, values);
return utils::ValueToEnum<EBlendingEquation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_SRC_ALPHA> Context::Get<types::EGetParameter::BLEND_SRC_ALPHA>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::BLEND_SRC_ALPHA, values);
return utils::ValueToEnum<EBlendingFactor>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::BLEND_SRC_RGB> Context::Get<types::EGetParameter::BLEND_SRC_RGB>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::BLEND_SRC_RGB, values);
return utils::ValueToEnum<EBlendingFactor>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::COLOR_CLEAR_VALUE> Context::Get<types::EGetParameter::COLOR_CLEAR_VALUE>()
{
std::array<float, 4> values;
GetValue<float>(types::EGetParameter::COLOR_CLEAR_VALUE, values);
return math::Vec4(values[0], values[1], values[2], values[3]);
}

template<>
data::QueryResultType<types::EGetParameter::COLOR_LOGIC_OP> Context::Get<types::EGetParameter::COLOR_LOGIC_OP>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::COLOR_LOGIC_OP, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::COLOR_WRITEMASK> Context::Get<types::EGetParameter::COLOR_WRITEMASK>()
{
std::array<bool, 4> values;
GetValue<bool>(types::EGetParameter::COLOR_WRITEMASK, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::COMPRESSED_TEXTURE_FORMATS> Context::Get<types::EGetParameter::COMPRESSED_TEXTURE_FORMATS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::COMPRESSED_TEXTURE_FORMATS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::CONTEXT_FLAGS> Context::Get<types::EGetParameter::CONTEXT_FLAGS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::CONTEXT_FLAGS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::CULL_FACE> Context::Get<types::EGetParameter::CULL_FACE>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::CULL_FACE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::CULL_FACE_MODE> Context::Get<types::EGetParameter::CULL_FACE_MODE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::CULL_FACE_MODE, values);
return utils::ValueToEnum<ECullFace>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::CURRENT_PROGRAM> Context::Get<types::EGetParameter::CURRENT_PROGRAM>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::CURRENT_PROGRAM, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DEBUG_GROUP_STACK_DEPTH> Context::Get<types::EGetParameter::DEBUG_GROUP_STACK_DEPTH>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DEBUG_GROUP_STACK_DEPTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DEPTH_CLEAR_VALUE> Context::Get<types::EGetParameter::DEPTH_CLEAR_VALUE>()
{
std::array<double, 1> values;
GetValue<double>(types::EGetParameter::DEPTH_CLEAR_VALUE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DEPTH_FUNC> Context::Get<types::EGetParameter::DEPTH_FUNC>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DEPTH_FUNC, values);
return utils::ValueToEnum<EComparaisonAlgorithm>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::DEPTH_RANGE> Context::Get<types::EGetParameter::DEPTH_RANGE>()
{
std::array<double, 2> values;
GetValue<double>(types::EGetParameter::DEPTH_RANGE, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::DEPTH_TEST> Context::Get<types::EGetParameter::DEPTH_TEST>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::DEPTH_TEST, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DEPTH_WRITEMASK> Context::Get<types::EGetParameter::DEPTH_WRITEMASK>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::DEPTH_WRITEMASK, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DISPATCH_INDIRECT_BUFFER_BINDING> Context::Get<types::EGetParameter::DISPATCH_INDIRECT_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DISPATCH_INDIRECT_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DITHER> Context::Get<types::EGetParameter::DITHER>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::DITHER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DOUBLEBUFFER> Context::Get<types::EGetParameter::DOUBLEBUFFER>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::DOUBLEBUFFER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER> Context::Get<types::EGetParameter::DRAW_BUFFER>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER0> Context::Get<types::EGetParameter::DRAW_BUFFER0>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER0, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER1> Context::Get<types::EGetParameter::DRAW_BUFFER1>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER1, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER10> Context::Get<types::EGetParameter::DRAW_BUFFER10>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER10, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER11> Context::Get<types::EGetParameter::DRAW_BUFFER11>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER11, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER12> Context::Get<types::EGetParameter::DRAW_BUFFER12>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER12, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER13> Context::Get<types::EGetParameter::DRAW_BUFFER13>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER13, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER14> Context::Get<types::EGetParameter::DRAW_BUFFER14>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER14, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER15> Context::Get<types::EGetParameter::DRAW_BUFFER15>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER15, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER2> Context::Get<types::EGetParameter::DRAW_BUFFER2>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER2, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER3> Context::Get<types::EGetParameter::DRAW_BUFFER3>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER3, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER4> Context::Get<types::EGetParameter::DRAW_BUFFER4>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER4, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER5> Context::Get<types::EGetParameter::DRAW_BUFFER5>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER5, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER6> Context::Get<types::EGetParameter::DRAW_BUFFER6>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER6, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER7> Context::Get<types::EGetParameter::DRAW_BUFFER7>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER7, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER8> Context::Get<types::EGetParameter::DRAW_BUFFER8>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER8, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_BUFFER9> Context::Get<types::EGetParameter::DRAW_BUFFER9>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_BUFFER9, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::DRAW_FRAMEBUFFER_BINDING> Context::Get<types::EGetParameter::DRAW_FRAMEBUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::DRAW_FRAMEBUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::ELEMENT_ARRAY_BUFFER_BINDING> Context::Get<types::EGetParameter::ELEMENT_ARRAY_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::ELEMENT_ARRAY_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::FRAGMENT_SHADER_DERIVATIVE_HINT> Context::Get<types::EGetParameter::FRAGMENT_SHADER_DERIVATIVE_HINT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::FRAGMENT_SHADER_DERIVATIVE_HINT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::IMPLEMENTATION_COLOR_READ_FORMAT> Context::Get<types::EGetParameter::IMPLEMENTATION_COLOR_READ_FORMAT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::IMPLEMENTATION_COLOR_READ_FORMAT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::IMPLEMENTATION_COLOR_READ_TYPE> Context::Get<types::EGetParameter::IMPLEMENTATION_COLOR_READ_TYPE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::IMPLEMENTATION_COLOR_READ_TYPE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::LAYER_PROVOKING_VERTEX> Context::Get<types::EGetParameter::LAYER_PROVOKING_VERTEX>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::LAYER_PROVOKING_VERTEX, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::LINE_SMOOTH> Context::Get<types::EGetParameter::LINE_SMOOTH>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::LINE_SMOOTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::LINE_SMOOTH_HINT> Context::Get<types::EGetParameter::LINE_SMOOTH_HINT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::LINE_SMOOTH_HINT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::LINE_WIDTH> Context::Get<types::EGetParameter::LINE_WIDTH>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::LINE_WIDTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::LOGIC_OP_MODE> Context::Get<types::EGetParameter::LOGIC_OP_MODE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::LOGIC_OP_MODE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAJOR_VERSION> Context::Get<types::EGetParameter::MAJOR_VERSION>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAJOR_VERSION, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_3D_TEXTURE_SIZE> Context::Get<types::EGetParameter::MAX_3D_TEXTURE_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_3D_TEXTURE_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_ARRAY_TEXTURE_LAYERS> Context::Get<types::EGetParameter::MAX_ARRAY_TEXTURE_LAYERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_ARRAY_TEXTURE_LAYERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_CLIP_DISTANCES> Context::Get<types::EGetParameter::MAX_CLIP_DISTANCES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_CLIP_DISTANCES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COLOR_TEXTURE_SAMPLES> Context::Get<types::EGetParameter::MAX_COLOR_TEXTURE_SAMPLES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COLOR_TEXTURE_SAMPLES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_COMBINED_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_COMBINED_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_TEXTURE_IMAGE_UNITS> Context::Get<types::EGetParameter::MAX_COMBINED_TEXTURE_IMAGE_UNITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_TEXTURE_IMAGE_UNITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_UNIFORM_BLOCKS> Context::Get<types::EGetParameter::MAX_COMBINED_UNIFORM_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_UNIFORM_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS> Context::Get<types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_COMPUTE_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_TEXTURE_IMAGE_UNITS> Context::Get<types::EGetParameter::MAX_COMPUTE_TEXTURE_IMAGE_UNITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_TEXTURE_IMAGE_UNITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_UNIFORM_BLOCKS> Context::Get<types::EGetParameter::MAX_COMPUTE_UNIFORM_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_UNIFORM_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_COMPUTE_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_COUNT> Context::Get<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_COUNT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_WORK_GROUP_COUNT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_INVOCATIONS> Context::Get<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_INVOCATIONS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_WORK_GROUP_INVOCATIONS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_SIZE> Context::Get<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_COMPUTE_WORK_GROUP_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_CUBE_MAP_TEXTURE_SIZE> Context::Get<types::EGetParameter::MAX_CUBE_MAP_TEXTURE_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_CUBE_MAP_TEXTURE_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_DEBUG_GROUP_STACK_DEPTH> Context::Get<types::EGetParameter::MAX_DEBUG_GROUP_STACK_DEPTH>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_DEBUG_GROUP_STACK_DEPTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_DEPTH_TEXTURE_SAMPLES> Context::Get<types::EGetParameter::MAX_DEPTH_TEXTURE_SAMPLES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_DEPTH_TEXTURE_SAMPLES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_DRAW_BUFFERS> Context::Get<types::EGetParameter::MAX_DRAW_BUFFERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_DRAW_BUFFERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_DUAL_SOURCE_DRAW_BUFFERS> Context::Get<types::EGetParameter::MAX_DUAL_SOURCE_DRAW_BUFFERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_DUAL_SOURCE_DRAW_BUFFERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_ELEMENTS_INDICES> Context::Get<types::EGetParameter::MAX_ELEMENTS_INDICES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_ELEMENTS_INDICES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_ELEMENTS_VERTICES> Context::Get<types::EGetParameter::MAX_ELEMENTS_VERTICES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_ELEMENTS_VERTICES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAGMENT_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_FRAGMENT_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAGMENT_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAGMENT_INPUT_COMPONENTS> Context::Get<types::EGetParameter::MAX_FRAGMENT_INPUT_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAGMENT_INPUT_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAGMENT_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_FRAGMENT_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAGMENT_UNIFORM_BLOCKS> Context::Get<types::EGetParameter::MAX_FRAGMENT_UNIFORM_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAGMENT_UNIFORM_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAGMENT_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_FRAGMENT_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAGMENT_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAGMENT_UNIFORM_VECTORS> Context::Get<types::EGetParameter::MAX_FRAGMENT_UNIFORM_VECTORS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAGMENT_UNIFORM_VECTORS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAMEBUFFER_HEIGHT> Context::Get<types::EGetParameter::MAX_FRAMEBUFFER_HEIGHT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAMEBUFFER_HEIGHT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAMEBUFFER_LAYERS> Context::Get<types::EGetParameter::MAX_FRAMEBUFFER_LAYERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAMEBUFFER_LAYERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAMEBUFFER_SAMPLES> Context::Get<types::EGetParameter::MAX_FRAMEBUFFER_SAMPLES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAMEBUFFER_SAMPLES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_FRAMEBUFFER_WIDTH> Context::Get<types::EGetParameter::MAX_FRAMEBUFFER_WIDTH>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_FRAMEBUFFER_WIDTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_GEOMETRY_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_INPUT_COMPONENTS> Context::Get<types::EGetParameter::MAX_GEOMETRY_INPUT_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_INPUT_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_OUTPUT_COMPONENTS> Context::Get<types::EGetParameter::MAX_GEOMETRY_OUTPUT_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_OUTPUT_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_GEOMETRY_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_TEXTURE_IMAGE_UNITS> Context::Get<types::EGetParameter::MAX_GEOMETRY_TEXTURE_IMAGE_UNITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_UNIFORM_BLOCKS> Context::Get<types::EGetParameter::MAX_GEOMETRY_UNIFORM_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_UNIFORM_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_GEOMETRY_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_GEOMETRY_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_GEOMETRY_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_INTEGER_SAMPLES> Context::Get<types::EGetParameter::MAX_INTEGER_SAMPLES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_INTEGER_SAMPLES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_LABEL_LENGTH> Context::Get<types::EGetParameter::MAX_LABEL_LENGTH>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_LABEL_LENGTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_PROGRAM_TEXEL_OFFSET> Context::Get<types::EGetParameter::MAX_PROGRAM_TEXEL_OFFSET>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_PROGRAM_TEXEL_OFFSET, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_RECTANGLE_TEXTURE_SIZE> Context::Get<types::EGetParameter::MAX_RECTANGLE_TEXTURE_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_RECTANGLE_TEXTURE_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_RENDERBUFFER_SIZE> Context::Get<types::EGetParameter::MAX_RENDERBUFFER_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_RENDERBUFFER_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_SAMPLE_MASK_WORDS> Context::Get<types::EGetParameter::MAX_SAMPLE_MASK_WORDS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_SAMPLE_MASK_WORDS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_SERVER_WAIT_TIMEOUT> Context::Get<types::EGetParameter::MAX_SERVER_WAIT_TIMEOUT>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::MAX_SERVER_WAIT_TIMEOUT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_SHADER_STORAGE_BUFFER_BINDINGS> Context::Get<types::EGetParameter::MAX_SHADER_STORAGE_BUFFER_BINDINGS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_SHADER_STORAGE_BUFFER_BINDINGS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TESS_CONTROL_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_TESS_CONTROL_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TESS_CONTROL_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TESS_EVALUATION_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_TESS_EVALUATION_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TESS_EVALUATION_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TEXTURE_BUFFER_SIZE> Context::Get<types::EGetParameter::MAX_TEXTURE_BUFFER_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TEXTURE_BUFFER_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TEXTURE_IMAGE_UNITS> Context::Get<types::EGetParameter::MAX_TEXTURE_IMAGE_UNITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TEXTURE_IMAGE_UNITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TEXTURE_LOD_BIAS> Context::Get<types::EGetParameter::MAX_TEXTURE_LOD_BIAS>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::MAX_TEXTURE_LOD_BIAS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_TEXTURE_SIZE> Context::Get<types::EGetParameter::MAX_TEXTURE_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_TEXTURE_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_UNIFORM_BLOCK_SIZE> Context::Get<types::EGetParameter::MAX_UNIFORM_BLOCK_SIZE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_UNIFORM_BLOCK_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_UNIFORM_BUFFER_BINDINGS> Context::Get<types::EGetParameter::MAX_UNIFORM_BUFFER_BINDINGS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_UNIFORM_BUFFER_BINDINGS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_UNIFORM_LOCATIONS> Context::Get<types::EGetParameter::MAX_UNIFORM_LOCATIONS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_UNIFORM_LOCATIONS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VARYING_COMPONENTS> Context::Get<types::EGetParameter::MAX_VARYING_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VARYING_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VARYING_FLOATS> Context::Get<types::EGetParameter::MAX_VARYING_FLOATS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VARYING_FLOATS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VARYING_VECTORS> Context::Get<types::EGetParameter::MAX_VARYING_VECTORS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VARYING_VECTORS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_ATOMIC_COUNTERS> Context::Get<types::EGetParameter::MAX_VERTEX_ATOMIC_COUNTERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_ATOMIC_COUNTERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_ATTRIBS> Context::Get<types::EGetParameter::MAX_VERTEX_ATTRIBS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_ATTRIBS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_ATTRIB_BINDINGS> Context::Get<types::EGetParameter::MAX_VERTEX_ATTRIB_BINDINGS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_ATTRIB_BINDINGS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_ATTRIB_RELATIVE_OFFSET> Context::Get<types::EGetParameter::MAX_VERTEX_ATTRIB_RELATIVE_OFFSET>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_OUTPUT_COMPONENTS> Context::Get<types::EGetParameter::MAX_VERTEX_OUTPUT_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_OUTPUT_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_SHADER_STORAGE_BLOCKS> Context::Get<types::EGetParameter::MAX_VERTEX_SHADER_STORAGE_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_SHADER_STORAGE_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_TEXTURE_IMAGE_UNITS> Context::Get<types::EGetParameter::MAX_VERTEX_TEXTURE_IMAGE_UNITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_TEXTURE_IMAGE_UNITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_UNIFORM_BLOCKS> Context::Get<types::EGetParameter::MAX_VERTEX_UNIFORM_BLOCKS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_UNIFORM_BLOCKS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_UNIFORM_COMPONENTS> Context::Get<types::EGetParameter::MAX_VERTEX_UNIFORM_COMPONENTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_UNIFORM_COMPONENTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VERTEX_UNIFORM_VECTORS> Context::Get<types::EGetParameter::MAX_VERTEX_UNIFORM_VECTORS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VERTEX_UNIFORM_VECTORS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VIEWPORTS> Context::Get<types::EGetParameter::MAX_VIEWPORTS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MAX_VIEWPORTS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MAX_VIEWPORT_DIMS> Context::Get<types::EGetParameter::MAX_VIEWPORT_DIMS>()
{
std::array<int, 2> values;
GetValue<int>(types::EGetParameter::MAX_VIEWPORT_DIMS, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::MINOR_VERSION> Context::Get<types::EGetParameter::MINOR_VERSION>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MINOR_VERSION, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MIN_MAP_BUFFER_ALIGNMENT> Context::Get<types::EGetParameter::MIN_MAP_BUFFER_ALIGNMENT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MIN_MAP_BUFFER_ALIGNMENT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::MIN_PROGRAM_TEXEL_OFFSET> Context::Get<types::EGetParameter::MIN_PROGRAM_TEXEL_OFFSET>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::MIN_PROGRAM_TEXEL_OFFSET, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::NUM_COMPRESSED_TEXTURE_FORMATS> Context::Get<types::EGetParameter::NUM_COMPRESSED_TEXTURE_FORMATS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::NUM_COMPRESSED_TEXTURE_FORMATS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::NUM_EXTENSIONS> Context::Get<types::EGetParameter::NUM_EXTENSIONS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::NUM_EXTENSIONS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::NUM_PROGRAM_BINARY_FORMATS> Context::Get<types::EGetParameter::NUM_PROGRAM_BINARY_FORMATS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::NUM_PROGRAM_BINARY_FORMATS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::NUM_SHADER_BINARY_FORMATS> Context::Get<types::EGetParameter::NUM_SHADER_BINARY_FORMATS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::NUM_SHADER_BINARY_FORMATS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_ALIGNMENT> Context::Get<types::EGetParameter::PACK_ALIGNMENT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PACK_ALIGNMENT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_IMAGE_HEIGHT> Context::Get<types::EGetParameter::PACK_IMAGE_HEIGHT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PACK_IMAGE_HEIGHT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_LSB_FIRST> Context::Get<types::EGetParameter::PACK_LSB_FIRST>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::PACK_LSB_FIRST, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_ROW_LENGTH> Context::Get<types::EGetParameter::PACK_ROW_LENGTH>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PACK_ROW_LENGTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_SKIP_IMAGES> Context::Get<types::EGetParameter::PACK_SKIP_IMAGES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PACK_SKIP_IMAGES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_SKIP_PIXELS> Context::Get<types::EGetParameter::PACK_SKIP_PIXELS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PACK_SKIP_PIXELS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_SKIP_ROWS> Context::Get<types::EGetParameter::PACK_SKIP_ROWS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PACK_SKIP_ROWS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PACK_SWAP_BYTES> Context::Get<types::EGetParameter::PACK_SWAP_BYTES>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::PACK_SWAP_BYTES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PIXEL_PACK_BUFFER_BINDING> Context::Get<types::EGetParameter::PIXEL_PACK_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PIXEL_PACK_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PIXEL_UNPACK_BUFFER_BINDING> Context::Get<types::EGetParameter::PIXEL_UNPACK_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PIXEL_UNPACK_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POINT_FADE_THRESHOLD_SIZE> Context::Get<types::EGetParameter::POINT_FADE_THRESHOLD_SIZE>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::POINT_FADE_THRESHOLD_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POINT_SIZE> Context::Get<types::EGetParameter::POINT_SIZE>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::POINT_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POINT_SIZE_GRANULARITY> Context::Get<types::EGetParameter::POINT_SIZE_GRANULARITY>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::POINT_SIZE_GRANULARITY, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POINT_SIZE_RANGE> Context::Get<types::EGetParameter::POINT_SIZE_RANGE>()
{
std::array<float, 2> values;
GetValue<float>(types::EGetParameter::POINT_SIZE_RANGE, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_OFFSET_CLAMP> Context::Get<types::EGetParameter::POLYGON_OFFSET_CLAMP>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::POLYGON_OFFSET_CLAMP, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_OFFSET_FACTOR> Context::Get<types::EGetParameter::POLYGON_OFFSET_FACTOR>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::POLYGON_OFFSET_FACTOR, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_OFFSET_FILL> Context::Get<types::EGetParameter::POLYGON_OFFSET_FILL>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::POLYGON_OFFSET_FILL, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_OFFSET_LINE> Context::Get<types::EGetParameter::POLYGON_OFFSET_LINE>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::POLYGON_OFFSET_LINE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_OFFSET_POINT> Context::Get<types::EGetParameter::POLYGON_OFFSET_POINT>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::POLYGON_OFFSET_POINT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_OFFSET_UNITS> Context::Get<types::EGetParameter::POLYGON_OFFSET_UNITS>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::POLYGON_OFFSET_UNITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_SMOOTH> Context::Get<types::EGetParameter::POLYGON_SMOOTH>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::POLYGON_SMOOTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::POLYGON_SMOOTH_HINT> Context::Get<types::EGetParameter::POLYGON_SMOOTH_HINT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::POLYGON_SMOOTH_HINT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PRIMITIVE_RESTART_INDEX> Context::Get<types::EGetParameter::PRIMITIVE_RESTART_INDEX>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PRIMITIVE_RESTART_INDEX, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PROGRAM_BINARY_FORMATS> Context::Get<types::EGetParameter::PROGRAM_BINARY_FORMATS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PROGRAM_BINARY_FORMATS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PROGRAM_PIPELINE_BINDING> Context::Get<types::EGetParameter::PROGRAM_PIPELINE_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PROGRAM_PIPELINE_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PROGRAM_POINT_SIZE> Context::Get<types::EGetParameter::PROGRAM_POINT_SIZE>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::PROGRAM_POINT_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::PROVOKING_VERTEX> Context::Get<types::EGetParameter::PROVOKING_VERTEX>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::PROVOKING_VERTEX, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::READ_BUFFER> Context::Get<types::EGetParameter::READ_BUFFER>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::READ_BUFFER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::READ_FRAMEBUFFER_BINDING> Context::Get<types::EGetParameter::READ_FRAMEBUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::READ_FRAMEBUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::RENDERBUFFER_BINDING> Context::Get<types::EGetParameter::RENDERBUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::RENDERBUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SAMPLER_BINDING> Context::Get<types::EGetParameter::SAMPLER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SAMPLER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SAMPLES> Context::Get<types::EGetParameter::SAMPLES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SAMPLES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SAMPLE_BUFFERS> Context::Get<types::EGetParameter::SAMPLE_BUFFERS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SAMPLE_BUFFERS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SAMPLE_COVERAGE_INVERT> Context::Get<types::EGetParameter::SAMPLE_COVERAGE_INVERT>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::SAMPLE_COVERAGE_INVERT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SAMPLE_COVERAGE_VALUE> Context::Get<types::EGetParameter::SAMPLE_COVERAGE_VALUE>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::SAMPLE_COVERAGE_VALUE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SAMPLE_MASK_VALUE> Context::Get<types::EGetParameter::SAMPLE_MASK_VALUE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SAMPLE_MASK_VALUE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SCISSOR_BOX> Context::Get<types::EGetParameter::SCISSOR_BOX>()
{
std::array<int, 4> values;
GetValue<int>(types::EGetParameter::SCISSOR_BOX, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::SCISSOR_TEST> Context::Get<types::EGetParameter::SCISSOR_TEST>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::SCISSOR_TEST, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SHADER_COMPILER> Context::Get<types::EGetParameter::SHADER_COMPILER>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::SHADER_COMPILER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SHADER_STORAGE_BUFFER_BINDING> Context::Get<types::EGetParameter::SHADER_STORAGE_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SHADER_STORAGE_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT> Context::Get<types::EGetParameter::SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SHADER_STORAGE_BUFFER_SIZE> Context::Get<types::EGetParameter::SHADER_STORAGE_BUFFER_SIZE>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::SHADER_STORAGE_BUFFER_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SHADER_STORAGE_BUFFER_START> Context::Get<types::EGetParameter::SHADER_STORAGE_BUFFER_START>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::SHADER_STORAGE_BUFFER_START, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SMOOTH_LINE_WIDTH_GRANULARITY> Context::Get<types::EGetParameter::SMOOTH_LINE_WIDTH_GRANULARITY>()
{
std::array<float, 1> values;
GetValue<float>(types::EGetParameter::SMOOTH_LINE_WIDTH_GRANULARITY, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SMOOTH_LINE_WIDTH_RANGE> Context::Get<types::EGetParameter::SMOOTH_LINE_WIDTH_RANGE>()
{
std::array<float, 2> values;
GetValue<float>(types::EGetParameter::SMOOTH_LINE_WIDTH_RANGE, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_FAIL> Context::Get<types::EGetParameter::STENCIL_BACK_FAIL>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_FAIL, values);
return utils::ValueToEnum<EOperation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_FUNC> Context::Get<types::EGetParameter::STENCIL_BACK_FUNC>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_FUNC, values);
return utils::ValueToEnum<EComparaisonAlgorithm>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_PASS_DEPTH_FAIL> Context::Get<types::EGetParameter::STENCIL_BACK_PASS_DEPTH_FAIL>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_PASS_DEPTH_FAIL, values);
return utils::ValueToEnum<EOperation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_PASS_DEPTH_PASS> Context::Get<types::EGetParameter::STENCIL_BACK_PASS_DEPTH_PASS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_PASS_DEPTH_PASS, values);
return utils::ValueToEnum<EOperation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_REF> Context::Get<types::EGetParameter::STENCIL_BACK_REF>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_REF, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_VALUE_MASK> Context::Get<types::EGetParameter::STENCIL_BACK_VALUE_MASK>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_VALUE_MASK, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_BACK_WRITEMASK> Context::Get<types::EGetParameter::STENCIL_BACK_WRITEMASK>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_BACK_WRITEMASK, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_CLEAR_VALUE> Context::Get<types::EGetParameter::STENCIL_CLEAR_VALUE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_CLEAR_VALUE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_FAIL> Context::Get<types::EGetParameter::STENCIL_FAIL>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_FAIL, values);
return utils::ValueToEnum<EOperation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_FUNC> Context::Get<types::EGetParameter::STENCIL_FUNC>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_FUNC, values);
return utils::ValueToEnum<EComparaisonAlgorithm>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_PASS_DEPTH_FAIL> Context::Get<types::EGetParameter::STENCIL_PASS_DEPTH_FAIL>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_PASS_DEPTH_FAIL, values);
return utils::ValueToEnum<EOperation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_PASS_DEPTH_PASS> Context::Get<types::EGetParameter::STENCIL_PASS_DEPTH_PASS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_PASS_DEPTH_PASS, values);
return utils::ValueToEnum<EOperation>(static_cast<GLenum>(values.at(0)));
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_REF> Context::Get<types::EGetParameter::STENCIL_REF>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_REF, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_TEST> Context::Get<types::EGetParameter::STENCIL_TEST>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::STENCIL_TEST, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_VALUE_MASK> Context::Get<types::EGetParameter::STENCIL_VALUE_MASK>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_VALUE_MASK, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STENCIL_WRITEMASK> Context::Get<types::EGetParameter::STENCIL_WRITEMASK>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::STENCIL_WRITEMASK, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::STEREO> Context::Get<types::EGetParameter::STEREO>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::STEREO, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::SUBPIXEL_BITS> Context::Get<types::EGetParameter::SUBPIXEL_BITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::SUBPIXEL_BITS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_1D> Context::Get<types::EGetParameter::TEXTURE_BINDING_1D>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_1D, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_1D_ARRAY> Context::Get<types::EGetParameter::TEXTURE_BINDING_1D_ARRAY>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_1D_ARRAY, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_2D> Context::Get<types::EGetParameter::TEXTURE_BINDING_2D>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_2D, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_2D_ARRAY> Context::Get<types::EGetParameter::TEXTURE_BINDING_2D_ARRAY>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_2D_ARRAY, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE> Context::Get<types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY> Context::Get<types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_3D> Context::Get<types::EGetParameter::TEXTURE_BINDING_3D>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_3D, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_BUFFER> Context::Get<types::EGetParameter::TEXTURE_BINDING_BUFFER>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_BUFFER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_CUBE_MAP> Context::Get<types::EGetParameter::TEXTURE_BINDING_CUBE_MAP>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_CUBE_MAP, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BINDING_RECTANGLE> Context::Get<types::EGetParameter::TEXTURE_BINDING_RECTANGLE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BINDING_RECTANGLE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_BUFFER_OFFSET_ALIGNMENT> Context::Get<types::EGetParameter::TEXTURE_BUFFER_OFFSET_ALIGNMENT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_BUFFER_OFFSET_ALIGNMENT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TEXTURE_COMPRESSION_HINT> Context::Get<types::EGetParameter::TEXTURE_COMPRESSION_HINT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TEXTURE_COMPRESSION_HINT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TIMESTAMP> Context::Get<types::EGetParameter::TIMESTAMP>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::TIMESTAMP, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_BINDING> Context::Get<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_SIZE> Context::Get<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_SIZE>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_START> Context::Get<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_START>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_START, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNIFORM_BUFFER_BINDING> Context::Get<types::EGetParameter::UNIFORM_BUFFER_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNIFORM_BUFFER_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNIFORM_BUFFER_OFFSET_ALIGNMENT> Context::Get<types::EGetParameter::UNIFORM_BUFFER_OFFSET_ALIGNMENT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNIFORM_BUFFER_OFFSET_ALIGNMENT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNIFORM_BUFFER_SIZE> Context::Get<types::EGetParameter::UNIFORM_BUFFER_SIZE>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::UNIFORM_BUFFER_SIZE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNIFORM_BUFFER_START> Context::Get<types::EGetParameter::UNIFORM_BUFFER_START>()
{
std::array<int64_t, 1> values;
GetValue<int64_t>(types::EGetParameter::UNIFORM_BUFFER_START, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_ALIGNMENT> Context::Get<types::EGetParameter::UNPACK_ALIGNMENT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNPACK_ALIGNMENT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_IMAGE_HEIGHT> Context::Get<types::EGetParameter::UNPACK_IMAGE_HEIGHT>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNPACK_IMAGE_HEIGHT, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_LSB_FIRST> Context::Get<types::EGetParameter::UNPACK_LSB_FIRST>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::UNPACK_LSB_FIRST, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_ROW_LENGTH> Context::Get<types::EGetParameter::UNPACK_ROW_LENGTH>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNPACK_ROW_LENGTH, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_SKIP_IMAGES> Context::Get<types::EGetParameter::UNPACK_SKIP_IMAGES>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNPACK_SKIP_IMAGES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_SKIP_PIXELS> Context::Get<types::EGetParameter::UNPACK_SKIP_PIXELS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNPACK_SKIP_PIXELS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_SKIP_ROWS> Context::Get<types::EGetParameter::UNPACK_SKIP_ROWS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::UNPACK_SKIP_ROWS, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::UNPACK_SWAP_BYTES> Context::Get<types::EGetParameter::UNPACK_SWAP_BYTES>()
{
std::array<bool, 1> values;
GetValue<bool>(types::EGetParameter::UNPACK_SWAP_BYTES, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VERTEX_ARRAY_BINDING> Context::Get<types::EGetParameter::VERTEX_ARRAY_BINDING>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VERTEX_ARRAY_BINDING, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VERTEX_BINDING_BUFFER> Context::Get<types::EGetParameter::VERTEX_BINDING_BUFFER>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VERTEX_BINDING_BUFFER, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VERTEX_BINDING_DIVISOR> Context::Get<types::EGetParameter::VERTEX_BINDING_DIVISOR>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VERTEX_BINDING_DIVISOR, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VERTEX_BINDING_OFFSET> Context::Get<types::EGetParameter::VERTEX_BINDING_OFFSET>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VERTEX_BINDING_OFFSET, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VERTEX_BINDING_STRIDE> Context::Get<types::EGetParameter::VERTEX_BINDING_STRIDE>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VERTEX_BINDING_STRIDE, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VIEWPORT> Context::Get<types::EGetParameter::VIEWPORT>()
{
std::array<int, 4> values;
GetValue<int>(types::EGetParameter::VIEWPORT, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::VIEWPORT_BOUNDS_RANGE> Context::Get<types::EGetParameter::VIEWPORT_BOUNDS_RANGE>()
{
std::array<int, 2> values;
GetValue<int>(types::EGetParameter::VIEWPORT_BOUNDS_RANGE, values);
return values;
}

template<>
data::QueryResultType<types::EGetParameter::VIEWPORT_INDEX_PROVOKING_VERTEX> Context::Get<types::EGetParameter::VIEWPORT_INDEX_PROVOKING_VERTEX>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VIEWPORT_INDEX_PROVOKING_VERTEX, values);
return values.at(0);
}

template<>
data::QueryResultType<types::EGetParameter::VIEWPORT_SUBPIXEL_BITS> Context::Get<types::EGetParameter::VIEWPORT_SUBPIXEL_BITS>()
{
std::array<int, 1> values;
GetValue<int>(types::EGetParameter::VIEWPORT_SUBPIXEL_BITS, values);
return values.at(0);
}
