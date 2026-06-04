/**
 * @project: baregl
 * @author: Adrien Givry
 * @licence: MIT
 */

#pragma once

#include <array>
#include <baregl/math/Vec4.h>
#include <baregl/types/EBlendingEquation.h>
#include <baregl/types/EBlendingFactor.h>
#include <baregl/types/EComparaisonAlgorithm.h>
#include <baregl/types/ECullFace.h>
#include <baregl/types/EGetParameter.h>
#include <baregl/types/EOperation.h>

namespace baregl::data
{
	/**
	* Result for a Get query
	*/
	template<auto PName> struct QueryResult;

	// Specialise per query constant:
	template<> struct QueryResult<types::EGetParameter::ACTIVE_TEXTURE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::ALIASED_LINE_WIDTH_RANGE> { using type = std::array<float, 2>; };
	template<> struct QueryResult<types::EGetParameter::ARRAY_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::BLEND> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::BLEND_COLOR> { using type = math::Vec4; };
	template<> struct QueryResult<types::EGetParameter::BLEND_DST_ALPHA> { using type = types::EBlendingFactor; };
	template<> struct QueryResult<types::EGetParameter::BLEND_DST_RGB> { using type = types::EBlendingFactor; };
	template<> struct QueryResult<types::EGetParameter::BLEND_EQUATION_ALPHA> { using type = types::EBlendingEquation; };
	template<> struct QueryResult<types::EGetParameter::BLEND_EQUATION_RGB> { using type = types::EBlendingEquation; };
	template<> struct QueryResult<types::EGetParameter::BLEND_SRC_ALPHA> { using type = types::EBlendingFactor; };
	template<> struct QueryResult<types::EGetParameter::BLEND_SRC_RGB> { using type = types::EBlendingFactor; };
	template<> struct QueryResult<types::EGetParameter::COLOR_CLEAR_VALUE> { using type = math::Vec4; };
	template<> struct QueryResult<types::EGetParameter::COLOR_LOGIC_OP> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::COLOR_WRITEMASK> { using type = std::array<bool, 4>; };
	template<> struct QueryResult<types::EGetParameter::COMPRESSED_TEXTURE_FORMATS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::CONTEXT_FLAGS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::CULL_FACE> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::CULL_FACE_MODE> { using type = types::ECullFace; };
	template<> struct QueryResult<types::EGetParameter::CURRENT_PROGRAM> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DEBUG_GROUP_STACK_DEPTH> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DEPTH_CLEAR_VALUE> { using type = double; };
	template<> struct QueryResult<types::EGetParameter::DEPTH_FUNC> { using type = types::EComparaisonAlgorithm; };
	template<> struct QueryResult<types::EGetParameter::DEPTH_RANGE> { using type = std::array<double, 2>; };
	template<> struct QueryResult<types::EGetParameter::DEPTH_TEST> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::DEPTH_WRITEMASK> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::DISPATCH_INDIRECT_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DITHER> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::DOUBLEBUFFER> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER0> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER1> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER10> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER11> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER12> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER13> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER14> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER15> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER2> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER3> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER4> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER5> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER6> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER7> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER8> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_BUFFER9> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::DRAW_FRAMEBUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::ELEMENT_ARRAY_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::FRAGMENT_SHADER_DERIVATIVE_HINT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::IMPLEMENTATION_COLOR_READ_FORMAT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::IMPLEMENTATION_COLOR_READ_TYPE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::LAYER_PROVOKING_VERTEX> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::LINE_SMOOTH> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::LINE_SMOOTH_HINT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::LINE_WIDTH> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::LOGIC_OP_MODE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAJOR_VERSION> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_3D_TEXTURE_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_ARRAY_TEXTURE_LAYERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_CLIP_DISTANCES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COLOR_TEXTURE_SAMPLES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_TEXTURE_IMAGE_UNITS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_UNIFORM_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_TEXTURE_IMAGE_UNITS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_UNIFORM_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_COUNT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_INVOCATIONS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_COMPUTE_WORK_GROUP_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_CUBE_MAP_TEXTURE_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_DEBUG_GROUP_STACK_DEPTH> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_DEPTH_TEXTURE_SAMPLES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_DRAW_BUFFERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_DUAL_SOURCE_DRAW_BUFFERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_ELEMENTS_INDICES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_ELEMENTS_VERTICES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAGMENT_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAGMENT_INPUT_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAGMENT_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAGMENT_UNIFORM_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAGMENT_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAGMENT_UNIFORM_VECTORS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAMEBUFFER_HEIGHT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAMEBUFFER_LAYERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAMEBUFFER_SAMPLES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_FRAMEBUFFER_WIDTH> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_INPUT_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_OUTPUT_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_TEXTURE_IMAGE_UNITS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_UNIFORM_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_GEOMETRY_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_INTEGER_SAMPLES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_LABEL_LENGTH> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_PROGRAM_TEXEL_OFFSET> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_RECTANGLE_TEXTURE_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_RENDERBUFFER_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_SAMPLE_MASK_WORDS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_SERVER_WAIT_TIMEOUT> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::MAX_SHADER_STORAGE_BUFFER_BINDINGS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TESS_CONTROL_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TESS_EVALUATION_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TEXTURE_BUFFER_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TEXTURE_IMAGE_UNITS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_TEXTURE_LOD_BIAS> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::MAX_TEXTURE_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_UNIFORM_BLOCK_SIZE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_UNIFORM_BUFFER_BINDINGS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_UNIFORM_LOCATIONS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VARYING_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VARYING_FLOATS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VARYING_VECTORS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_ATOMIC_COUNTERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_ATTRIBS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_ATTRIB_BINDINGS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_ATTRIB_RELATIVE_OFFSET> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_OUTPUT_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_SHADER_STORAGE_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_TEXTURE_IMAGE_UNITS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_UNIFORM_BLOCKS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_UNIFORM_COMPONENTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VERTEX_UNIFORM_VECTORS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VIEWPORTS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MAX_VIEWPORT_DIMS> { using type = std::array<int, 2>; };
	template<> struct QueryResult<types::EGetParameter::MINOR_VERSION> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MIN_MAP_BUFFER_ALIGNMENT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::MIN_PROGRAM_TEXEL_OFFSET> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::NUM_COMPRESSED_TEXTURE_FORMATS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::NUM_EXTENSIONS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::NUM_PROGRAM_BINARY_FORMATS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::NUM_SHADER_BINARY_FORMATS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_ALIGNMENT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_IMAGE_HEIGHT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_LSB_FIRST> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::PACK_ROW_LENGTH> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_SKIP_IMAGES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_SKIP_PIXELS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_SKIP_ROWS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PACK_SWAP_BYTES> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::PIXEL_PACK_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PIXEL_UNPACK_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::POINT_FADE_THRESHOLD_SIZE> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::POINT_SIZE> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::POINT_SIZE_GRANULARITY> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::POINT_SIZE_RANGE> { using type = std::array<float, 2>; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_OFFSET_CLAMP> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_OFFSET_FACTOR> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_OFFSET_FILL> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_OFFSET_LINE> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_OFFSET_POINT> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_OFFSET_UNITS> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_SMOOTH> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::POLYGON_SMOOTH_HINT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PRIMITIVE_RESTART_INDEX> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PROGRAM_BINARY_FORMATS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PROGRAM_PIPELINE_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::PROGRAM_POINT_SIZE> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::PROVOKING_VERTEX> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::READ_BUFFER> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::READ_FRAMEBUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::RENDERBUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SAMPLER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SAMPLES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SAMPLE_BUFFERS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SAMPLE_COVERAGE_INVERT> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::SAMPLE_COVERAGE_VALUE> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::SAMPLE_MASK_VALUE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SCISSOR_BOX> { using type = std::array<int, 4>; };
	template<> struct QueryResult<types::EGetParameter::SCISSOR_TEST> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::SHADER_COMPILER> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::SHADER_STORAGE_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::SHADER_STORAGE_BUFFER_SIZE> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::SHADER_STORAGE_BUFFER_START> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::SMOOTH_LINE_WIDTH_GRANULARITY> { using type = float; };
	template<> struct QueryResult<types::EGetParameter::SMOOTH_LINE_WIDTH_RANGE> { using type = std::array<float, 2>; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_FAIL> { using type = types::EOperation; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_FUNC> { using type = types::EComparaisonAlgorithm; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_PASS_DEPTH_FAIL> { using type = types::EOperation; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_PASS_DEPTH_PASS> { using type = types::EOperation; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_REF> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_VALUE_MASK> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_BACK_WRITEMASK> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_CLEAR_VALUE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_FAIL> { using type = types::EOperation; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_FUNC> { using type = types::EComparaisonAlgorithm; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_PASS_DEPTH_FAIL> { using type = types::EOperation; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_PASS_DEPTH_PASS> { using type = types::EOperation; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_REF> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_TEST> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_VALUE_MASK> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STENCIL_WRITEMASK> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::STEREO> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::SUBPIXEL_BITS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_1D> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_1D_ARRAY> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_2D> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_2D_ARRAY> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_3D> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_BUFFER> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_CUBE_MAP> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BINDING_RECTANGLE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_BUFFER_OFFSET_ALIGNMENT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TEXTURE_COMPRESSION_HINT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TIMESTAMP> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_SIZE> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::TRANSFORM_FEEDBACK_BUFFER_START> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::UNIFORM_BUFFER_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNIFORM_BUFFER_OFFSET_ALIGNMENT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNIFORM_BUFFER_SIZE> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::UNIFORM_BUFFER_START> { using type = int64_t; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_ALIGNMENT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_IMAGE_HEIGHT> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_LSB_FIRST> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_ROW_LENGTH> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_SKIP_IMAGES> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_SKIP_PIXELS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_SKIP_ROWS> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::UNPACK_SWAP_BYTES> { using type = bool; };
	template<> struct QueryResult<types::EGetParameter::VERTEX_ARRAY_BINDING> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::VERTEX_BINDING_BUFFER> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::VERTEX_BINDING_DIVISOR> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::VERTEX_BINDING_OFFSET> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::VERTEX_BINDING_STRIDE> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::VIEWPORT> { using type = std::array<int, 4>; };
	template<> struct QueryResult<types::EGetParameter::VIEWPORT_BOUNDS_RANGE> { using type = std::array<int, 2>; };
	template<> struct QueryResult<types::EGetParameter::VIEWPORT_INDEX_PROVOKING_VERTEX> { using type = int; };
	template<> struct QueryResult<types::EGetParameter::VIEWPORT_SUBPIXEL_BITS> { using type = int; };

	template<auto P>
	using QueryResultType = typename QueryResult<P>::type;
}
