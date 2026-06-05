/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <catch2/catch_test_macros.hpp>

#include <common/Boilerplate.h>

#include <algorithm>

using namespace tests::common::boilerplate;
using namespace baregl;
using enum baregl::types::EGetParameter;
using namespace baregl::types;

namespace
{
	template<class T, size_t N>
	bool IsWithinRange(T p_value, const std::array<T, N>& p_range)
	{
		auto [min_val, max_val] = std::ranges::minmax(p_range);

		return
			p_value >= min_val &&
			p_value <= max_val;
	}
}

#define QUERY(query, ...) p_context.Get<query>(__VA_ARGS__)
#define QUERY_WORKS(query, ...) (QUERY(query, __VA_ARGS__), true)

TEST_CASE( "Queries return the expected values", "[queries]" ) {
	RunInContext([](GLFWwindow* p_window, Context& p_context) {
		REQUIRE( QUERY(ACTIVE_TEXTURE) == ETextureUnit::TEXTURE0 );
		REQUIRE( IsWithinRange(1.0f, QUERY(ALIASED_LINE_WIDTH_RANGE)) );
		Buffer vbo;
		vbo.Bind(EBufferType::VERTEX);
		REQUIRE( QUERY(ARRAY_BUFFER_BINDING) == vbo.GetID() );
		p_context.SetCapability(ERenderingCapability::BLEND, true);
		REQUIRE( QUERY(BLEND) == true );
		REQUIRE( QUERY(BLEND_COLOR) == std::to_array({0.0f,0.0f,0.0f,0.0f}) );
		p_context.SetBlendingFunction(EBlendingFactor::CONSTANT_ALPHA, EBlendingFactor::CONSTANT_ALPHA);
		REQUIRE( QUERY(BLEND_DST_ALPHA) == EBlendingFactor::CONSTANT_ALPHA );
		REQUIRE( QUERY(BLEND_DST_RGB) == EBlendingFactor::CONSTANT_ALPHA );
		p_context.SetBlendingEquation(EBlendingEquation::FUNC_REVERSE_SUBTRACT);
		REQUIRE( QUERY(BLEND_EQUATION_RGB) == EBlendingEquation::FUNC_REVERSE_SUBTRACT );
		REQUIRE( QUERY(BLEND_EQUATION_ALPHA) == EBlendingEquation::FUNC_REVERSE_SUBTRACT );
		REQUIRE( QUERY(BLEND_SRC_ALPHA) == EBlendingFactor::CONSTANT_ALPHA );
		REQUIRE( QUERY(BLEND_SRC_RGB) == EBlendingFactor::CONSTANT_ALPHA );
		REQUIRE( QUERY(COLOR_CLEAR_VALUE) == std::to_array({0.0f,0.0f,0.0f,0.0f}));
		p_context.SetClearColor(0.1f, 0.2f, 0.3f, 0.4f);
		REQUIRE( QUERY(COLOR_CLEAR_VALUE) == std::to_array({0.1f,0.2f,0.3f,0.4f}));
		REQUIRE( QUERY(COLOR_LOGIC_OP) == false );
		REQUIRE( QUERY(COLOR_WRITEMASK) == std::to_array({true,true,true,true}) );
		p_context.SetColorWriting(true, false, true, false);
		REQUIRE( QUERY(COLOR_WRITEMASK) == std::to_array({true,false,true,false}) );
		QUERY(COMPRESSED_TEXTURE_FORMATS); // Could be empty, or not, no particular check to perform here.
		REQUIRE( QUERY(MAX_COMPUTE_SHADER_STORAGE_BLOCKS) >= 4 ); // GL Core 4.5 compliant
		REQUIRE( QUERY(MAX_COMBINED_SHADER_STORAGE_BLOCKS) >= 8 ); // GL Core 4.5 compliant
		REQUIRE( QUERY(MAX_COMPUTE_UNIFORM_BLOCKS) >= 14 );
		REQUIRE( QUERY(MAX_COMPUTE_TEXTURE_IMAGE_UNITS) >= 16 );
		REQUIRE( QUERY(MAX_COMPUTE_UNIFORM_COMPONENTS) >= 1024 );
		REQUIRE( QUERY(MAX_COMPUTE_ATOMIC_COUNTERS) >= 8 ); // GL Core 4.5 compliant
		REQUIRE( QUERY(MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS) >= 1 ); // GL Core 4.5 compliant
		REQUIRE( QUERY(MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS) >= 1 );
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_INVOCATIONS) >= 1 ); // GL Core 4.5 compliant
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_COUNT, 0) >= 1 );
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_COUNT, 1) >= 1 );
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_COUNT, 2) >= 1 );
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_SIZE, 0) >= 1 );
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_SIZE, 1) >= 1 );
		REQUIRE( QUERY(MAX_COMPUTE_WORK_GROUP_SIZE, 2) >= 1 );
		REQUIRE( QUERY(DISPATCH_INDIRECT_BUFFER_BINDING) == 0 );
		REQUIRE( QUERY(MAX_DEBUG_GROUP_STACK_DEPTH) >= 0 );
		REQUIRE( QUERY(DEBUG_GROUP_STACK_DEPTH) >= 0 );
		REQUIRE( QUERY(CONTEXT_FLAGS) == EContextFlags::DEBUG_BIT );
		REQUIRE( QUERY(CULL_FACE) == false );
		p_context.SetCapability(ERenderingCapability::CULL_FACE, true);
		REQUIRE( QUERY(CULL_FACE) == true );
		REQUIRE( QUERY(CULL_FACE_MODE) == ECullFace::BACK );
		p_context.SetCullFace(ECullFace::FRONT_AND_BACK);
		REQUIRE( QUERY(CULL_FACE_MODE) == ECullFace::FRONT_AND_BACK );
		REQUIRE( QUERY(CURRENT_PROGRAM) == 0 );
		REQUIRE( QUERY(CURRENT_PROGRAM) == 0 );
		REQUIRE( QUERY(DEPTH_CLEAR_VALUE) == 1.0 );
		REQUIRE( QUERY(DEPTH_FUNC) == EComparaisonAlgorithm::LESS );
		REQUIRE( QUERY(DEPTH_RANGE) == std::to_array({0.0, 1.0}) );
		REQUIRE( QUERY(DEPTH_TEST) == false );
		p_context.SetCapability(ERenderingCapability::DEPTH_TEST, true);
		REQUIRE( QUERY(DEPTH_TEST) == true );
		REQUIRE( QUERY(DEPTH_WRITEMASK) == true );
		p_context.SetDepthWriting(false);
		REQUIRE( QUERY(DEPTH_WRITEMASK) == false );
		REQUIRE( QUERY(DITHER) == true );
		p_context.SetCapability(ERenderingCapability::DITHER, false);
		REQUIRE( QUERY_WORKS(DOUBLEBUFFER) );
		const auto maxBuffers = QUERY(MAX_DRAW_BUFFERS);
		REQUIRE( QUERY(DRAW_BUFFER) == EDrawBuffer::BACK );
		REQUIRE( QUERY(DRAW_BUFFER0) == EDrawBuffer::BACK );
		REQUIRE( QUERY(DRAW_BUFFER1) == EDrawBuffer::NONE );
		REQUIRE( QUERY(DRAW_BUFFER2) == EDrawBuffer::NONE );
		REQUIRE( QUERY(DRAW_BUFFER3) == EDrawBuffer::NONE );
		REQUIRE( QUERY(DRAW_BUFFER4) == EDrawBuffer::NONE );
		REQUIRE( QUERY(DRAW_BUFFER5) == EDrawBuffer::NONE );
		REQUIRE( QUERY(DRAW_BUFFER6) == EDrawBuffer::NONE );
		REQUIRE( QUERY(DRAW_BUFFER7) == EDrawBuffer::NONE );
		REQUIRE( (maxBuffers < 9 || QUERY(DRAW_BUFFER8) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 10 || QUERY(DRAW_BUFFER9) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 11 || QUERY(DRAW_BUFFER10) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 12 || QUERY(DRAW_BUFFER11) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 13 || QUERY(DRAW_BUFFER12) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 14 || QUERY(DRAW_BUFFER13) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 15 || QUERY(DRAW_BUFFER14) == EDrawBuffer::NONE) );
		REQUIRE( (maxBuffers < 16 || QUERY(DRAW_BUFFER15) == EDrawBuffer::NONE) );
		REQUIRE( QUERY(DRAW_FRAMEBUFFER_BINDING) == 0 );
		REQUIRE( QUERY(READ_FRAMEBUFFER_BINDING) == 0 );
		Framebuffer fbo;
		fbo.Bind();
		REQUIRE( QUERY(DRAW_FRAMEBUFFER_BINDING) == fbo.GetID() );
		REQUIRE( QUERY(READ_FRAMEBUFFER_BINDING) == fbo.GetID() );
		REQUIRE( QUERY(ELEMENT_ARRAY_BUFFER_BINDING) == 0 );
		Buffer ibo;
		ibo.Bind(EBufferType::INDEX);
		REQUIRE( QUERY(ELEMENT_ARRAY_BUFFER_BINDING) == ibo.GetID() );
		REQUIRE( QUERY(FRAGMENT_SHADER_DERIVATIVE_HINT) == EHint::DONT_CARE );
		// TODO: Enable these when our pixel data format is more comprehensive
		// REQUIRE( QUERY_WORKS(IMPLEMENTATION_COLOR_READ_FORMAT) );
		// REQUIRE( QUERY_WORKS(IMPLEMENTATION_COLOR_READ_TYPE) );
		REQUIRE( QUERY(LINE_SMOOTH) == false );
		p_context.SetCapability(ERenderingCapability::LINE_SMOOTH, true);
		REQUIRE( QUERY(LINE_SMOOTH) == true );
		REQUIRE( QUERY(LINE_SMOOTH_HINT) == EHint::DONT_CARE );
		REQUIRE( QUERY(LINE_WIDTH) == 1.0f);
		p_context.SetRasterizationLinesWidth(2.0f);
		REQUIRE( QUERY(LINE_WIDTH) == 2.0f);
		REQUIRE( QUERY(LAYER_PROVOKING_VERTEX) == EProvokingVertexConvention::FIRST_VERTEX_CONVENTION);
	});
}

