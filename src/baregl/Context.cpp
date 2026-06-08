/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/Context.h>

#include <concepts>
#include <span>
#include <vector>

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

	template<typename T>
	concept SupportedQueryType =
		std::same_as<T, int> || 
		std::same_as<T, int64_t> || 
		std::same_as<T, bool> || 
		std::same_as<T, float> || 
		std::same_as<T, double> ||
		std::same_as<T, std::string>;


	template<SupportedQueryType T>
	void GetValue(
		baregl::types::EGetParameter p_param,
		std::span<T> p_out
	);

	template<SupportedQueryType T>
	void GetValueIndexed(
		baregl::types::EGetParameter p_param,
		std::span<T> p_out,
		uint32_t p_index
	);

	template<>
	void GetValue<int>(
		baregl::types::EGetParameter p_param,
		std::span<int> p_out
	)
	{
		glGetIntegerv(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void GetValueIndexed<int>(
		baregl::types::EGetParameter p_param,
		std::span<int> p_out,
		uint32_t p_index
	)
	{
		glGetIntegeri_v(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void GetValue<int64_t>(
		baregl::types::EGetParameter p_param,
		std::span<int64_t> p_out
	)
	{
		glGetInteger64v(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void GetValueIndexed<int64_t>(
		baregl::types::EGetParameter p_param,
		std::span<int64_t> p_out,
		uint32_t p_index
	)
	{
		glGetInteger64i_v(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void GetValue<bool>(
		baregl::types::EGetParameter p_param,
		std::span<bool> p_out
	)
	{
		std::vector<GLboolean> values(p_out.size());
		glGetBooleanv(
			baregl::utils::EnumToValue<GLenum>(p_param),
			values.data()
		);

		for (size_t i = 0; i < p_out.size(); ++i)
		{
			p_out[i] = values[i] != GL_FALSE;
		}
	}

	template<>
	void GetValueIndexed<bool>(
		baregl::types::EGetParameter p_param,
		std::span<bool> p_out,
		uint32_t p_index
	)
	{
		std::vector<GLboolean> values(p_out.size());
		glGetBooleani_v(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_index,
			values.data()
		);

		for (size_t i = 0; i < p_out.size(); ++i)
		{
			p_out[i] = values[i] != GL_FALSE;
		}
	}

	template<>
	void GetValue<float>(
		baregl::types::EGetParameter p_param,
		std::span<float> p_out
	)
	{
		glGetFloatv(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void GetValueIndexed<float>(
		baregl::types::EGetParameter p_param,
		std::span<float> p_out,
		uint32_t p_index
	)
	{
		glGetFloati_v(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void GetValue<double>(
		baregl::types::EGetParameter p_param,
		std::span<double> p_out
	)
	{
		glGetDoublev(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_out.data()
		);
	}

	template<>
	void GetValueIndexed<double>(
		baregl::types::EGetParameter p_param,
		std::span<double> p_out,
		uint32_t p_index
	)
	{
		glGetDoublei_v(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_index,
			p_out.data()
		);
	}

	template<>
	void GetValue<std::string>(
		baregl::types::EGetParameter p_param,
		std::span<std::string> p_out
	)
	{
		p_out[0] = GetString(
			baregl::utils::EnumToValue<GLenum>(p_param)
		);
	}

	template<>
	void GetValueIndexed<std::string>(
		baregl::types::EGetParameter p_param,
		std::span<std::string> p_out,
		uint32_t p_index
	)
	{
		p_out[0] = GetString(
			baregl::utils::EnumToValue<GLenum>(p_param),
			p_index
		);
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
		
	template<auto PName>
	data::QueryResultType<PName> Context::Get()
	{
		using R = data::QueryResult<PName>;
		using Elem = typename R::query_type;

		if constexpr (R::dynamic_count)
		{
			using CountQuery = data::QueryResult<R::dynamic_count_parameter>;
			using CountElem = typename CountQuery::query_type;
			std::array<CountElem, CountQuery::count> countRaw;
			GetValue<CountElem>(R::dynamic_count_parameter, countRaw);

			const auto elementCount = static_cast<size_t>(countRaw[0] > 0 ? countRaw[0] : 0);
			std::vector<Elem> raw(elementCount);
			GetValue<Elem>(PName, raw);

			if constexpr (std::same_as<typename R::type, std::vector<Elem>>)
			{
				return raw;
			}

			static_assert(std::same_as<typename R::type, std::vector<Elem>>, "Unsupported dynamic query result type");
		}
		else
		{
			constexpr size_t N = R::count;
			std::array<Elem, N> raw;
			GetValue<Elem>(PName, raw);

			if constexpr (std::same_as<typename R::type, Elem>)
				return raw[0];
			else if constexpr (std::same_as<typename R::type, std::array<Elem, N>>)
				return raw;
			else if constexpr (std::integral<typename R::type> && std::integral<Elem>)
				return static_cast<typename R::type>(raw[0]);
			else if constexpr (std::is_enum_v<typename R::type>)
				return utils::ValueToEnum<typename R::type>(static_cast<GLenum>(raw[0]));

			static_assert(
				std::same_as<typename R::type, Elem> ||
				std::same_as<typename R::type, std::array<Elem, N>> ||
				(std::integral<typename R::type> && std::integral<Elem>) ||
				std::is_enum_v<typename R::type>,
				"Unsupported query result type"
			);
		}
	}

	template<auto PName>
		requires (data::QueryResult<PName>::indexed)
	data::QueryResultType<PName> Context::Get(uint32_t p_index)
	{
		using R = data::QueryResult<PName>;
		using Elem = typename R::query_type;

		if constexpr (R::dynamic_count)
		{
			using CountQuery = data::QueryResult<R::dynamic_count_parameter>;
			using CountElem = typename CountQuery::query_type;
			using FetchQuery = data::QueryResult<R::dynamic_query_parameter>;
			using FetchElem = typename FetchQuery::query_type;

			std::array<CountElem, CountQuery::count> countRaw;
			GetValue<CountElem>(R::dynamic_count_parameter, countRaw);

			const auto elementCount = static_cast<size_t>(countRaw[0] > 0 ? countRaw[0] : 0);
			std::vector<FetchElem> raw(elementCount);
			GetValueIndexed<FetchElem>(R::dynamic_query_parameter, raw, p_index);

			if constexpr (std::same_as<typename R::type, std::vector<Elem>>)
			{
				return raw;
			}

			static_assert(std::same_as<typename R::type, std::vector<Elem>>, "Unsupported dynamic indexed query result type");
		}
		else
		{
			constexpr size_t N = R::count;
			std::array<Elem, N> raw;
			GetValueIndexed<Elem>(PName, raw, p_index);

			if constexpr (std::same_as<typename R::type, Elem>)
				return raw[0];
			else if constexpr (std::same_as<typename R::type, std::array<Elem, N>>)
				return raw;
			else if constexpr (std::integral<typename R::type> && std::integral<Elem>)
				return static_cast<typename R::type>(raw[0]);
			else if constexpr (std::is_enum_v<typename R::type>)
				return utils::ValueToEnum<typename R::type>(static_cast<GLenum>(raw[0]));

			static_assert(
				std::same_as<typename R::type, Elem> ||
				std::same_as<typename R::type, std::array<Elem, N>> ||
				(std::integral<typename R::type> && std::integral<Elem>) ||
				std::is_enum_v<typename R::type>,
				"Unsupported indexed query result type"
			);
		}
	}

#define INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(getType, indexed) \
	template data::QueryResultType<baregl::types::EGetParameter::getType> \
	Context::Get<baregl::types::EGetParameter::getType>(); \
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION_INDEXED_##indexed(getType)

#define INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION_INDEXED_true(getType) \
	template data::QueryResultType<baregl::types::EGetParameter::getType> \
	Context::Get<baregl::types::EGetParameter::getType>(uint32_t);

#define INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION_INDEXED_false(getType)

#define INSTANTIATE_CUSTOM_QUERY_SPECIALIZATION(getType) \
	template<> \
	data::QueryResultType<baregl::types::EGetParameter::getType> \
	Context::Get<baregl::types::EGetParameter::getType>()

	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PATCH_VERTICES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PATCH_DEFAULT_OUTER_LEVEL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PATCH_DEFAULT_INNER_LEVEL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ELEMENT_ARRAY_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VERTEX_BINDING_OFFSET, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VERTEX_BINDING_STRIDE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VERTEX_BINDING_DIVISOR, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VERTEX_BINDING_BUFFER, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ARRAY_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_INDIRECT_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VERTEX_ARRAY_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PRIMITIVE_RESTART, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PRIMITIVE_RESTART_FIXED_INDEX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PRIMITIVE_RESTART_INDEX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VIEWPORT, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEPTH_RANGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE0, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE1, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE2, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE3, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE4, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE5, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE6, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DISTANCE7, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_ORIGIN, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLIP_DEPTH_MODE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEPTH_CLAMP, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TRANSFORM_FEEDBACK_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CLAMP_READ_COLOR, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PROVOKING_VERTEX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(RASTERIZER_DISCARD, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POINT_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POINT_FADE_THRESHOLD_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POINT_SPRITE_COORD_ORIGIN, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(LINE_WIDTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(LINE_SMOOTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CULL_FACE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CULL_FACE_MODE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(FRONT_FACE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_SMOOTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_MODE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_OFFSET_FACTOR, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_OFFSET_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_OFFSET_POINT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_OFFSET_LINE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_OFFSET_FILL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MULTISAMPLE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_ALPHA_TO_COVERAGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_ALPHA_TO_ONE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_COVERAGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_COVERAGE_VALUE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_COVERAGE_INVERT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_SHADING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MIN_SAMPLE_SHADING_VALUE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_MASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_MASK_VALUE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_1D, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_2D, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_3D, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_1D_ARRAY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_2D_ARRAY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_CUBE_MAP_ARRAY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_RECTANGLE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_BUFFER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_CUBE_MAP, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_2D_MULTISAMPLE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ACTIVE_TEXTURE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SCISSOR_TEST, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SCISSOR_BOX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_TEST, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_FUNC, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_VALUE_MASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_REF, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_FAIL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_PASS_DEPTH_FAIL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_PASS_DEPTH_PASS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_FUNC, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_VALUE_MASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_REF, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_FAIL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_PASS_DEPTH_FAIL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_PASS_DEPTH_PASS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEPTH_TEST, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEPTH_FUNC, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_SRC_RGB, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_SRC_ALPHA, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_DST_RGB, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_DST_ALPHA, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_EQUATION_RGB, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_EQUATION_ALPHA, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(BLEND_COLOR, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(FRAMEBUFFER_SRGB, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DITHER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(COLOR_LOGIC_OP, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(LOGIC_OP_MODE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(COLOR_WRITEMASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEPTH_WRITEMASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_WRITEMASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_BACK_WRITEMASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(COLOR_CLEAR_VALUE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEPTH_CLEAR_VALUE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STENCIL_CLEAR_VALUE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_FRAMEBUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(READ_FRAMEBUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER0, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER1, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER2, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER3, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER4, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER5, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER6, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER7, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER8, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER9, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER10, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER11, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER12, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER13, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER14, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DRAW_BUFFER15, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(READ_BUFFER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(RENDERBUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_SWAP_BYTES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_LSB_FIRST, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_IMAGE_HEIGHT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_SKIP_IMAGES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_ROW_LENGTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_SKIP_ROWS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_SKIP_PIXELS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_ALIGNMENT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_COMPRESSED_BLOCK_WIDTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_COMPRESSED_BLOCK_HEIGHT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_COMPRESSED_BLOCK_DEPTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNPACK_COMPRESSED_BLOCK_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PIXEL_UNPACK_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_SWAP_BYTES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_LSB_FIRST, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_IMAGE_HEIGHT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_SKIP_IMAGES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_ROW_LENGTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_SKIP_ROWS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_SKIP_PIXELS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_ALIGNMENT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_COMPRESSED_BLOCK_WIDTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_COMPRESSED_BLOCK_HEIGHT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_COMPRESSED_BLOCK_DEPTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PACK_COMPRESSED_BLOCK_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PIXEL_PACK_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CURRENT_PROGRAM, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PROGRAM_PIPELINE_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PROGRAM_POINT_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMAGE_BINDING_NAME, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMAGE_BINDING_LEVEL, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMAGE_BINDING_LAYERED, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMAGE_BINDING_LAYER, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMAGE_BINDING_ACCESS, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMAGE_BINDING_FORMAT, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ATOMIC_COUNTER_BUFFER_BINDING, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ATOMIC_COUNTER_BUFFER_START, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ATOMIC_COUNTER_BUFFER_SIZE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADER_STORAGE_BUFFER_BINDING, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADER_STORAGE_BUFFER_START, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADER_STORAGE_BUFFER_SIZE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TRANSFORM_FEEDBACK_BUFFER_BINDING, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TRANSFORM_FEEDBACK_BUFFER_START, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TRANSFORM_FEEDBACK_BUFFER_SIZE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TRANSFORM_FEEDBACK_PAUSED, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TRANSFORM_FEEDBACK_ACTIVE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNIFORM_BUFFER_BINDING, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNIFORM_BUFFER_START, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNIFORM_BUFFER_SIZE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(LINE_SMOOTH_HINT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POLYGON_SMOOTH_HINT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_COMPRESSION_HINT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(FRAGMENT_SHADER_DERIVATIVE_HINT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DISPATCH_INDIRECT_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CONTEXT_RELEASE_BEHAVIOR, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_CLIP_DISTANCES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_CULL_DISTANCES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_CLIP_AND_CULL_DISTANCES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SUBPIXEL_BITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_ELEMENT_INDEX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_3D_TEXTURE_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TEXTURE_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_ARRAY_TEXTURE_LAYERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TEXTURE_LOD_BIAS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_CUBE_MAP_TEXTURE_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_RENDERBUFFER_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VIEWPORT_DIMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VIEWPORTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VIEWPORT_SUBPIXEL_BITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VIEWPORT_BOUNDS_RANGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(LAYER_PROVOKING_VERTEX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VIEWPORT_INDEX_PROVOKING_VERTEX, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POINT_SIZE_RANGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(POINT_SIZE_GRANULARITY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(ALIASED_LINE_WIDTH_RANGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SMOOTH_LINE_WIDTH_RANGE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SMOOTH_LINE_WIDTH_GRANULARITY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_ELEMENTS_INDICES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_ELEMENTS_VERTICES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_ATTRIB_BINDINGS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_ATTRIB_STRIDE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(NUM_COMPRESSED_TEXTURE_FORMATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(COMPRESSED_TEXTURE_FORMATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TEXTURE_BUFFER_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_RECTANGLE_TEXTURE_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(NUM_PROGRAM_BINARY_FORMATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(PROGRAM_BINARY_FORMATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(NUM_SHADER_BINARY_FORMATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADER_BINARY_FORMATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADER_COMPILER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MIN_MAP_BUFFER_ALIGNMENT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BUFFER_OFFSET_ALIGNMENT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAJOR_VERSION, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MINOR_VERSION, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CONTEXT_FLAGS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(CONTEXT_PROFILE_MASK, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(EXTENSIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(NUM_EXTENSIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(RENDERER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADING_LANGUAGE_VERSION, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADING_LANGUAGE_VERSIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(NUM_SHADING_LANGUAGE_VERSIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VENDOR, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(VERSION, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_ATTRIBS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_UNIFORM_VECTORS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_OUTPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_GEN_LEVEL, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_PATCH_VERTICES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_OUTPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_PATCH_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_INPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_INPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_INPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_OUTPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_OUTPUT_VERTICES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_SHADER_INVOCATIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_STREAMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_UNIFORM_VECTORS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_INPUT_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MIN_PROGRAM_TEXTURE_GATHER_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_PROGRAM_TEXTURE_GATHER_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_WORK_GROUP_COUNT, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_WORK_GROUP_SIZE, true);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_WORK_GROUP_INVOCATIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_SHARED_MEMORY_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMPUTE_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MIN_PROGRAM_TEXEL_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_PROGRAM_TEXEL_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_UNIFORM_BUFFER_BINDINGS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_UNIFORM_BLOCK_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(UNIFORM_BUFFER_OFFSET_ALIGNMENT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_UNIFORM_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VARYING_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VARYING_VECTORS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VARYING_FLOATS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_TEXTURE_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SUBROUTINES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SUBROUTINE_UNIFORM_LOCATIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_UNIFORM_LOCATIONS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_ATOMIC_COUNTER_BUFFER_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_ATOMIC_COUNTER_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_ATOMIC_COUNTERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SHADER_STORAGE_BUFFER_BINDINGS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SHADER_STORAGE_BLOCK_SIZE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_SHADER_STORAGE_BLOCKS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_IMAGE_UNITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_SHADER_OUTPUT_RESOURCES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_IMAGE_SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_VERTEX_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_CONTROL_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TESS_EVALUATION_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_GEOMETRY_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_IMAGE_UNIFORMS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEBUG_LOGGED_MESSAGES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEBUG_NEXT_LOGGED_MESSAGE_LENGTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEBUG_OUTPUT_SYNCHRONOUS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEBUG_GROUP_STACK_DEPTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DEBUG_OUTPUT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_DEBUG_MESSAGE_LENGTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_DEBUG_LOGGED_MESSAGES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_DEBUG_GROUP_STACK_DEPTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_LABEL_LENGTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAMEBUFFER_WIDTH, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAMEBUFFER_HEIGHT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAMEBUFFER_LAYERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAMEBUFFER_SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SAMPLE_MASK_WORDS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COLOR_TEXTURE_SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_DEPTH_TEXTURE_SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_INTEGER_SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_SERVER_WAIT_TIMEOUT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MIN_FRAGMENT_INTERPOLATION_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_FRAGMENT_INTERPOLATION_OFFSET, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(FRAGMENT_INTERPOLATION_OFFSET_BITS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_DRAW_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_DUAL_SOURCE_DRAW_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_COLOR_ATTACHMENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(MAX_TRANSFORM_FEEDBACK_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(DOUBLEBUFFER, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(STEREO, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLE_BUFFERS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(SAMPLES, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMPLEMENTATION_COLOR_READ_FORMAT, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(IMPLEMENTATION_COLOR_READ_TYPE, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(QUERY_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(COPY_READ_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(COPY_WRITE_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(RESET_NOTIFICATION_STRATEGY, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_BUFFER_BINDING, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TEXTURE_CUBE_MAP_SEAMLESS, false);
	INSTANTIATE_DEFAULT_QUERY_SPECIALIZATION(TIMESTAMP, false);

}
