/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <cstdint>

namespace baregl::types
{
	/**
	* Parameter that can be use to retrieve associated values through GetX functions.
	*/
	enum class EGetParameter : uint16_t
	{
		// Current Values and Associated Data
		PATCH_VERTICES,
		PATCH_DEFAULT_OUTER_LEVEL,
		PATCH_DEFAULT_INNER_LEVEL,

		// Vertex Array Object State (cont.?)
		// VERTEX_ATTRIB_ARRAY_ENABLED, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_SIZE, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_STRIDE, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_TYPE, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_NORMALIZED, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_INTEGER, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_LONG, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_DIVISOR, // GetVertexAttribiv
		// VERTEX_ATTRIB_ARRAY_POINTER, // GetVertexAttribiv
		// GetObjectLabel (string)
		ELEMENT_ARRAY_BUFFER_BINDING,                   // 1 int
		// VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, // GetVertexAttribiv
		// VERTEX_ATTRIB_BINDING, // GetVertexAttribiv
		// VERTEX_ATTRIB_RELATIVE_OFFSET, // GetVertexAttribiv
		VERTEX_BINDING_OFFSET,                          // 1 int64 (indexed)
		VERTEX_BINDING_STRIDE,                          // 1 int (indexed)
		VERTEX_BINDING_DIVISOR,                         // 1 int (indexed)
		VERTEX_BINDING_BUFFER,                          // 1 int (indexed)

		// Vertex Array Data (not in Vertex Array objects)
		ARRAY_BUFFER_BINDING,                           // 1 int
		DRAW_INDIRECT_BUFFER_BINDING,                   // 1 int
		VERTEX_ARRAY_BINDING,                           // 1 int
		PRIMITIVE_RESTART,                              // 1 boolean
		PRIMITIVE_RESTART_FIXED_INDEX,                  // 1 boolean
		PRIMITIVE_RESTART_INDEX,                        // 1 int

		// Buffer Object State
		// GetBufferSubData(),


    // ...


		// Program Interface State
		// ACTIVE_RESOURCES,                               // n int (GetProgramInterfaceiv)
		// MAX_NAME_LENGTH,                                // n int (GetProgramInterfaceiv)
		// MAX_NUM_ACTIVE_VARIABLES,                       // n int (GetProgramInterfaceiv)
		// MAX_NUM_COMPATIBLE_SUBROUTINES,                 // n int (GetProgramInterfaceiv)

		// Program Object Resource State
		// ACTIVE_VARIABLES,                               // 1 int (GetProgramResourceiv)
		// ARRAY_SIZE,                                     // 1 int (GetProgramResourceiv)
		// ARRAY_STRIDE,                                   // 1 int (GetProgramResourceiv)
		// ATOMIC_COUNTER_BUFFER_INDEX,                    // 1 int (GetProgramResourceiv)
		// BLOCK_INDEX,                                    // 1 int (GetProgramResourceiv)
		// BUFFER_BINDING,                                 // 1 int (GetProgramResourceiv)
		// BUFFER_DATA_SIZE,                               // 1 int (GetProgramResourceiv)
		// COMPATIBLE_SUBROUTINES,                         // 1 int (GetProgramResourceiv)
		// IS_PER_PATCH,                                   // 1 int (GetProgramResourceiv)
		// IS_ROW_MAJOR,                                   // 1 int (GetProgramResourceiv)
		// LOCATION,                                       // 1 int (GetProgramResourceiv)
		// LOCATION_COMPONENT,                             // 1 int (GetProgramResourceiv)
		// LOCATION_INDEX,                                 // 1 int (GetProgramResourceiv)
		// MATRIX_STRIDE,                                  // 1 int (GetProgramResourceiv)
		// NAME_LENGTH,                                    // 1 int (GetProgramResourceiv)
		// NUM_ACTIVE_VARIABLES,                           // 1 int (GetProgramResourceiv)
		// NUM_COMPATIBLE_SUBROUTINES,                     // 1 int (GetProgramResourceiv)
		// OFFSET,                                         // 1 int (GetProgramResourceiv)
		// REFERENCED_BY_VERTEX_SHADER,                    // 1 int (GetProgramResourceiv)
		// REFERENCED_BY_TESS_CONTROL_SHADER,              // 1 int (GetProgramResourceiv)
		// REFERENCED_BY_TESS_EVALUATION_SHADER,           // 1 int (GetProgramResourceiv)
		// REFERENCED_BY_GEOMETRY_SHADER,                  // 1 int (GetProgramResourceiv)
		// REFERENCED_BY_FRAGMENT_SHADER,                  // 1 int (GetProgramResourceiv)
		// REFERENCED_BY_COMPUTE_SHADER,                   // 1 int (GetProgramResourceiv)
		// TOP_LEVEL_ARRAY_SIZE,                           // 1 int (GetProgramResourceiv)
		// TOP_LEVEL_ARRAY_STRIDE,                         // 1 int (GetProgramResourceiv)
		// TYPE,                                           // 1 int (GetProgramResourceiv)


		// Vertex and Geometry Shader State (not part of program objects)
		// CURRENT_VERTEX_ATTRIB,                          // ? floats (GetVertexAttribfv)
		PROGRAM_POINT_SIZE,                             // 1 boolean

		// Query Object State
		QUERY_RESULT,                                   // 1 int (GetQueryObjectuiv)
		QUERY_RESULT_AVAILABLE,                         // 1 int (GetQueryObjectiv)
		// GetObjectLabel,
		QUERY_TARGET,                                   // 1 int (GLenum) (GetQueryObjectiv)

		// Image State (state per image unit)
		IMAGE_BINDING_NAME,                             // 1 int (indexed)
		IMAGE_BINDING_LEVEL,                            // 1 int (indexed)
		IMAGE_BINDING_LAYERED,                          // 1 boolean (indexed)
		IMAGE_BINDING_LAYER,                            // 1 int (indexed)
		IMAGE_BINDING_ACCESS,                           // 1 int (GLenum)(indexed)
		IMAGE_BINDING_FORMAT,                           // 1 int (GLenum)(indexed)

		// Atomic Counter Buffer Binding State
		ATOMIC_COUNTER_BUFFER_BINDING,                  // 1 int (or indexed)
		ATOMIC_COUNTER_BUFFER_START,                    // 1 int64 (indexed)
		ATOMIC_COUNTER_BUFFER_SIZE,                     // 1 int64 (indexed)

		// Shader Storage Buffer Binding State
		SHADER_STORAGE_BUFFER_BINDING,                  // 1 int (or indexed)
		SHADER_STORAGE_BUFFER_START,                    // 1 int64 (indexed)
		SHADER_STORAGE_BUFFER_SIZE,                     // 1 int64 (indexed)

		// Transform Feedback State
		TRANSFORM_FEEDBACK_BUFFER_BINDING,              // 1 int (or indexed)
		TRANSFORM_FEEDBACK_BUFFER_START,                // 1 int64 (indexed)
		TRANSFORM_FEEDBACK_BUFFER_SIZE,                 // 1 int64 (indexed)
		TRANSFORM_FEEDBACK_PAUSED,                      // 1 boolean
		TRANSFORM_FEEDBACK_ACTIVE,                      // 1 boolean
		// GetObjectLabel,

		// Uniform Buffer Binding State
		UNIFORM_BUFFER_BINDING,                         // 1 int (or indexed)
		UNIFORM_BUFFER_START,                           // 1 int64 (indexed)
		UNIFORM_BUFFER_SIZE,                            // 1 int64 (indexed)

		// Sync Object State
		// OBJECT_TYPE,                                    // 1 int (GLenum) (GetSynciv)
		// SYNC_STATUS,                                    // 1 int (GLenum) (GetSynciv)
		// SYNC_CONDITION,                                 // 1 int (GLenum) (GetSynciv)
		// SYNC_FLAGS,                                     // 1 int (GetSynciv)
		// GetObjectPtrLabel,

		// Hints
		LINE_SMOOTH_HINT,                               // 1 int (GLenum)
		POLYGON_SMOOTH_HINT,                            // 1 int (GLenum)
		TEXTURE_COMPRESSION_HINT,                       // 1 int (GLenum)
		FRAGMENT_SHADER_DERIVATIVE_HINT,                // 1 int (GLenum)

		// Compute Dispatch State
		DISPATCH_INDIRECT_BUFFER_BINDING,               // 1 int

		// Implementation Dependent Values
		CONTEXT_RELEASE_BEHAVIOR,
		MAX_CLIP_DISTANCES,                             // 1 int
		MAX_CULL_DISTANCES,                             // 1 int
		MAX_COMBINED_CLIP_AND_CULL_DISTANCES,           // 1 int
		SUBPIXEL_BITS,                                  // 1 int
		MAX_ELEMENT_INDEX,                              // 1 int64
		PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED,        // 1 bool
		MAX_3D_TEXTURE_SIZE,                            // 1 int
		MAX_TEXTURE_SIZE,                               // 1 int
		MAX_ARRAY_TEXTURE_LAYERS,                       // 1 int
		MAX_TEXTURE_LOD_BIAS,                           // 1 float
		MAX_CUBE_MAP_TEXTURE_SIZE,                      // 1 int
		MAX_RENDERBUFFER_SIZE,                          // 1 int
		MAX_VIEWPORT_DIMS,                              // 2 ints
		MAX_VIEWPORTS,                                  // 1 int
		VIEWPORT_SUBPIXEL_BITS,                         // 1 int
		VIEWPORT_BOUNDS_RANGE,                          // 2 ints
		LAYER_PROVOKING_VERTEX,                         // 1 int (GLenum)
		VIEWPORT_INDEX_PROVOKING_VERTEX,                // 1 int (GLenum)
		POINT_SIZE_RANGE,                               // 2 floats
		POINT_SIZE_GRANULARITY,                         // 1 float
		ALIASED_LINE_WIDTH_RANGE,                       // 2 floats
		SMOOTH_LINE_WIDTH_RANGE,                        // 2 floats
		SMOOTH_LINE_WIDTH_GRANULARITY,                  // 1 float
		MAX_ELEMENTS_INDICES,                           // 1 int
		MAX_ELEMENTS_VERTICES,                          // 1 int
		MAX_VERTEX_ATTRIB_RELATIVE_OFFSET,              // 1 int
		MAX_VERTEX_ATTRIB_BINDINGS,                     // 1 int
		MAX_VERTEX_ATTRIB_STRIDE,                       // 1 int
		NUM_COMPRESSED_TEXTURE_FORMATS,                 // 1 int
		COMPRESSED_TEXTURE_FORMATS,                     // n ints (GL_NUM_COMPRESSED_TEXTURE_FORMATS)
		MAX_TEXTURE_BUFFER_SIZE,                        // 1 int
		MAX_RECTANGLE_TEXTURE_SIZE,                     // 1 int
		NUM_PROGRAM_BINARY_FORMATS,                     // 1 int
		PROGRAM_BINARY_FORMATS,                         // n ints (GL_NUM_PROGRAM_BINARY_FORMATS)
		NUM_SHADER_BINARY_FORMATS,                      // 1 int
		SHADER_BINARY_FORMATS,                          // n ints (GL_NUM_SHADER_BINARY_FORMATS)
		SHADER_COMPILER,                                // 1 boolean
		MIN_MAP_BUFFER_ALIGNMENT,                       // 1 int
		TEXTURE_BUFFER_OFFSET_ALIGNMENT,                // 1 int
		
		// Implementation Dependent Version and Extension Support
		MAJOR_VERSION,                                  // 1 int
		MINOR_VERSION,                                  // 1 int
		CONTEXT_FLAGS,                                  // 1 int
		CONTEXT_PROFILE_MASK,                           // 1 int
		EXTENSIONS,                                     // n ints (GL_NUM_EXTENSIONS)
		NUM_EXTENSIONS,                                 // 1 int
		RENDERER,                                       // 1 string
		SHADING_LANGUAGE_VERSION,                       // 1 string
		SHADING_LANGUAGE_VERSIONS,                      // (not in specs, added for convenience) n string (GL_NUM_SHADING_LANGUAGE_VERSIONS)
		NUM_SHADING_LANGUAGE_VERSIONS,                  // 1 int
		VENDOR,                                         // 1 string
		VERSION,                                        // 1 string

		// Implementation Dependent Vertex Shader Limits
		MAX_VERTEX_ATTRIBS,                             // 1 int
		MAX_VERTEX_UNIFORM_COMPONENTS,                  // 1 int
		MAX_VERTEX_UNIFORM_VECTORS,                     // 1 int
		MAX_VERTEX_UNIFORM_BLOCKS,                      // 1 int
		MAX_VERTEX_OUTPUT_COMPONENTS,                   // 1 int
		MAX_VERTEX_TEXTURE_IMAGE_UNITS,                 // 1 int
		MAX_VERTEX_ATOMIC_COUNTER_BUFFERS,              // 1 int
		MAX_VERTEX_ATOMIC_COUNTERS,                     // 1 int
		MAX_VERTEX_SHADER_STORAGE_BLOCKS,               // 1 int
		
		// Implementation Dependent Tessellation Shader Limits
		MAX_TESS_GEN_LEVEL,                             // 1 int
		MAX_PATCH_VERTICES,                             // 1 int
		MAX_TESS_CONTROL_UNIFORM_COMPONENTS,            // 1 int
		MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,           // 1 int
		MAX_TESS_CONTROL_OUTPUT_COMPONENTS,             // 1 int
		MAX_TESS_PATCH_COMPONENTS,                      // 1 int
		MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS,       // 1 int
		MAX_TESS_CONTROL_INPUT_COMPONENTS,              // 1 int
		MAX_TESS_CONTROL_UNIFORM_BLOCKS,                // 1 int
		MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS,        // 1 int
		MAX_TESS_CONTROL_ATOMIC_COUNTERS,               // 1 int
		MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS,         // 1 int
		MAX_TESS_EVALUATION_UNIFORM_COMPONENTS,         // 1 int
		MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,        // 1 int
		MAX_TESS_EVALUATION_OUTPUT_COMPONENTS,          // 1 int
		MAX_TESS_EVALUATION_INPUT_COMPONENTS,           // 1 int
		MAX_TESS_EVALUATION_UNIFORM_BLOCKS,             // 1 int
		MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS,     // 1 int
		MAX_TESS_EVALUATION_ATOMIC_COUNTERS,            // 1 int
		MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS,      // 1 int

		// Implementation Dependent Geometry Shader Limits
		MAX_GEOMETRY_UNIFORM_COMPONENTS,                // 1 int
		MAX_GEOMETRY_UNIFORM_BLOCKS,                    // 1 int
		MAX_GEOMETRY_INPUT_COMPONENTS,                  // 1 int
		MAX_GEOMETRY_OUTPUT_COMPONENTS,                 // 1 int
		MAX_GEOMETRY_OUTPUT_VERTICES,                   // 1 int
		MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS,           // 1 int
		MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,               // 1 int
		MAX_GEOMETRY_SHADER_INVOCATIONS,                // 1 int
		MAX_VERTEX_STREAMS,                             // 1 int
		MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS,            // 1 int
		MAX_GEOMETRY_ATOMIC_COUNTERS,                   // 1 int
		MAX_GEOMETRY_SHADER_STORAGE_BLOCKS,             // 1 int

		// Implementation Dependent Fragment Shader Limits
		MAX_FRAGMENT_UNIFORM_COMPONENTS,                // 1 int
		MAX_FRAGMENT_UNIFORM_VECTORS,                   // 1 int
		MAX_FRAGMENT_UNIFORM_BLOCKS,                    // 1 int
		MAX_FRAGMENT_INPUT_COMPONENTS,                  // 1 int
		MAX_TEXTURE_IMAGE_UNITS,                        // 1 int
		MIN_PROGRAM_TEXTURE_GATHER_OFFSET,              // 1 int
		MAX_PROGRAM_TEXTURE_GATHER_OFFSET,              // 1 int
		MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS,            // 1 int
		MAX_FRAGMENT_ATOMIC_COUNTERS,                   // 1 int
		MAX_FRAGMENT_SHADER_STORAGE_BLOCKS,             // 1 int
		
		// Implementation Dependent Compute Shader Limits
		MAX_COMPUTE_WORK_GROUP_COUNT,                   // 1 int (indexed)
		MAX_COMPUTE_WORK_GROUP_SIZE,                    // 1 int (indexed)
		MAX_COMPUTE_WORK_GROUP_INVOCATIONS,             // 1 int
		MAX_COMPUTE_UNIFORM_BLOCKS,                     // 1 int
		MAX_COMPUTE_TEXTURE_IMAGE_UNITS,                // 1 int
		MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS,             // 1 int
		MAX_COMPUTE_SHARED_MEMORY_SIZE,                 // 1 int
		MAX_COMPUTE_UNIFORM_COMPONENTS,                 // 1 int
		MAX_COMPUTE_IMAGE_UNIFORMS,                     // 1 int
		MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS,        // 1 int
		MAX_COMPUTE_SHADER_STORAGE_BLOCKS,              // 1 int

		// Implementation Dependent Aggregate Shader Limits
		MIN_PROGRAM_TEXEL_OFFSET,                       // 1 int
		MAX_PROGRAM_TEXEL_OFFSET,                       // 1 int
		MAX_UNIFORM_BUFFER_BINDINGS,                    // 1 int
		MAX_UNIFORM_BLOCK_SIZE,                         // 1 int
		UNIFORM_BUFFER_OFFSET_ALIGNMENT,                // 1 int
		MAX_COMBINED_UNIFORM_BLOCKS,                    // 1 int
		MAX_VARYING_COMPONENTS,                         // 1 int
		MAX_VARYING_VECTORS,                            // 1 int
		MAX_COMBINED_TEXTURE_IMAGE_UNITS,               // 1 int
		MAX_SUBROUTINES,                                // 1 int
		MAX_SUBROUTINE_UNIFORM_LOCATIONS,               // 1 int
		MAX_UNIFORM_LOCATIONS,                          // 1 int
		MAX_ATOMIC_COUNTER_BUFFER_BINDINGS,             // 1 int
		MAX_ATOMIC_COUNTER_BUFFER_SIZE,                 // 1 int
		MAX_COMBINED_ATOMIC_COUNTER_BUFFERS,            // 1 int
		MAX_COMBINED_ATOMIC_COUNTERS,                   // 1 int
		MAX_SHADER_STORAGE_BUFFER_BINDINGS,             // 1 int
		MAX_SHADER_STORAGE_BLOCK_SIZE,                  // 1 int64
		MAX_COMBINED_SHADER_STORAGE_BLOCKS,             // 1 int
		SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT,         // 1 int
		MAX_IMAGE_UNITS,                                // 1 int
		MAX_COMBINED_SHADER_OUTPUT_RESOURCES,           // 1 int
		MAX_IMAGE_SAMPLES,                              // 1 int
		MAX_VERTEX_IMAGE_UNIFORMS,                      // 1 int
		MAX_TESS_CONTROL_IMAGE_UNIFORMS,                // 1 int
		MAX_TESS_EVALUATION_IMAGE_UNIFORMS,             // 1 int
		MAX_GEOMETRY_IMAGE_UNIFORMS,                    // 1 int
		MAX_FRAGMENT_IMAGE_UNIFORMS,                    // 1 int
		MAX_COMBINED_IMAGE_UNIFORMS,                    // 1 int
		MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,         // 1 int
		MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,       // 1 int
		MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS,   // 1 int
		MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS,// 1 int
		MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,       // 1 int

		// Debug Output State
		DEBUG_CALLBACK_FUNCTION,                        // 1 pointer
		DEBUG_CALLBACK_USER_PARAM,                      // 1 pointer
		DEBUG_LOGGED_MESSAGES,                          // 1 int
		DEBUG_NEXT_LOGGED_MESSAGE_LENGTH,               // 1 int
		DEBUG_OUTPUT_SYNCHRONOUS,                       // 1 boolean
		DEBUG_GROUP_STACK_DEPTH,                        // 1 int
		DEBUG_OUTPUT,                                   // 1 boolean

		// Implementation Dependent Debug Output State
		MAX_DEBUG_MESSAGE_LENGTH,                       // 1 int
		MAX_DEBUG_LOGGED_MESSAGES,                      // 1 int
		MAX_DEBUG_GROUP_STACK_DEPTH,                    // 1 int
		MAX_LABEL_LENGTH,                               // 1 int

		// Implementation Dependent Values (cont.)
		MAX_FRAMEBUFFER_WIDTH,                          // 1 int
		MAX_FRAMEBUFFER_HEIGHT,                         // 1 int
		MAX_FRAMEBUFFER_LAYERS,                         // 1 int
		MAX_FRAMEBUFFER_SAMPLES,                        // 1 int
		MAX_SAMPLE_MASK_WORDS,                          // 1 int
		MAX_SAMPLES,                                    // 1 int
		MAX_COLOR_TEXTURE_SAMPLES,                      // 1 int
		MAX_DEPTH_TEXTURE_SAMPLES,                      // 1 int
		MAX_INTEGER_SAMPLES,                            // 1 int
		// QUERY_COUNTER_BITS,                             // 5 int (GetQueryiv)
		MAX_SERVER_WAIT_TIMEOUT,                        // 1 int64
		MIN_FRAGMENT_INTERPOLATION_OFFSET,              // 1 float
		MAX_FRAGMENT_INTERPOLATION_OFFSET,              // 1 float
		FRAGMENT_INTERPOLATION_OFFSET_BITS,             // 1 int
		MAX_DRAW_BUFFERS,                               // 1 int
		MAX_DUAL_SOURCE_DRAW_BUFFERS,                   // 1 int
		MAX_COLOR_ATTACHMENTS,                          // 1 int

		// Internal Format Dependent Values
		// SAMPLES,                                        // 1 int (GetInternalformativ)
		// NUM_SAMPLE_COUNTS,                              // 1 int (GetInternalformativ)

		// Implementation Dependent Transform Feedback Limits
		MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,  // 1 int
		MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,        // 1 int
		MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS,     // 1 int
		MAX_TRANSFORM_FEEDBACK_BUFFERS,                 // 1 int

		// Framebuffer Dependent Values
		DOUBLEBUFFER,                                   // 1 boolean
		STEREO,                                         // 1 boolean
		SAMPLE_BUFFERS,                                 // 1 int
		SAMPLES,                                        // 1 int
		// SAMPLE_POSITION,                                // ??? (GetMultisamplefv)
		IMPLEMENTATION_COLOR_READ_FORMAT,               // 1 int (GLenum)
		IMPLEMENTATION_COLOR_READ_TYPE,                 // 1 int (GLenum)

		// Miscellaneous
		// CURRENT_QUERY,                                  // 1 int (GetQueryiv)
		QUERY_BUFFER_BINDING,                           // 1 int
		COPY_READ_BUFFER_BINDING,                       // 1 int
		COPY_WRITE_BUFFER_BINDING,                      // 1 int
		RESET_NOTIFICATION_STRATEGY,                    // 1 int
		TEXTURE_BUFFER_BINDING,                         // 1 int
		TEXTURE_CUBE_MAP_SEAMLESS,                      // 1 boolean


		ACTIVE_TEXTURE,                                 // 1 int (GLenum)
		BLEND,                                          // 1 boolean
		BLEND_COLOR,                                    // 4 floats
		BLEND_DST_ALPHA,                                // 1 int (GLenum)
		BLEND_DST_RGB,                                  // 1 int (GLenum)
		BLEND_EQUATION_RGB,                             // 1 int (GLenum)
		BLEND_EQUATION_ALPHA,                           // 1 int (GLenum)
		BLEND_SRC_ALPHA,                                // 1 int (GLenum)
		BLEND_SRC_RGB,                                  // 1 int (GLenum)
		COLOR_CLEAR_VALUE,                              // 4 floats
		COLOR_LOGIC_OP,                                 // 1 boolean
		COLOR_WRITEMASK,                                // 4 booleans
		MAX_COMPUTE_ATOMIC_COUNTERS,                    // 1 int
		CULL_FACE,                                      // 1 boolean
		CULL_FACE_MODE,                                 // 1 int (GLenum)
		CURRENT_PROGRAM,                                // 1 int
		DEPTH_CLEAR_VALUE,                              // 1 double/float
		DEPTH_FUNC,                                     // 1 int (GLenum)
		DEPTH_RANGE,                                    // 2 doubles/floats
		DEPTH_TEST,                                     // 1 boolean
		DEPTH_WRITEMASK,                                // 1 boolean
		DITHER,                                         // 1 boolean
		DRAW_BUFFER,                                    // 1 int (GLenum)
		DRAW_BUFFER0,                                   // 1 int (GLenum)
		DRAW_BUFFER1,                                   // 1 int (GLenum)
		DRAW_BUFFER2,                                   // 1 int (GLenum)
		DRAW_BUFFER3,                                   // 1 int (GLenum)
		DRAW_BUFFER4,                                   // 1 int (GLenum)
		DRAW_BUFFER5,                                   // 1 int (GLenum)
		DRAW_BUFFER6,                                   // 1 int (GLenum)
		DRAW_BUFFER7,                                   // 1 int (GLenum)
		DRAW_BUFFER8,                                   // 1 int (GLenum)
		DRAW_BUFFER9,                                   // 1 int (GLenum)
		DRAW_BUFFER10,                                  // 1 int (GLenum)
		DRAW_BUFFER11,                                  // 1 int (GLenum)
		DRAW_BUFFER12,                                  // 1 int (GLenum)
		DRAW_BUFFER13,                                  // 1 int (GLenum)
		DRAW_BUFFER14,                                  // 1 int (GLenum)
		DRAW_BUFFER15,                                  // 1 int (GLenum)
		DRAW_FRAMEBUFFER_BINDING,                       // 1 int
		READ_FRAMEBUFFER_BINDING,                       // 1 int
		LINE_SMOOTH,                                    // 1 boolean
		LINE_WIDTH,                                     // 1 float
		LOGIC_OP_MODE,                                  // 1 int (GLenum)
		MAX_VARYING_FLOATS,                             // 1 int
		PACK_ALIGNMENT,                                 // 1 int
		PACK_IMAGE_HEIGHT,                              // 1 int
		PACK_LSB_FIRST,                                 // 1 boolean
		PACK_ROW_LENGTH,                                // 1 int
		PACK_SKIP_IMAGES,                               // 1 int
		PACK_SKIP_PIXELS,                               // 1 int
		PACK_SKIP_ROWS,                                 // 1 int
		PACK_SWAP_BYTES,                                // 1 boolean
		PIXEL_PACK_BUFFER_BINDING,                      // 1 int
		PIXEL_UNPACK_BUFFER_BINDING,                    // 1 int
		POINT_FADE_THRESHOLD_SIZE,                      // 1 float
		PROGRAM_PIPELINE_BINDING,                       // 1 int
		PROVOKING_VERTEX,                               // 1 int (GLenum)
		POINT_SIZE,                                     // 1 float
		POLYGON_OFFSET_FACTOR,                          // 1 float
		POLYGON_OFFSET_UNITS,                           // 1 float
		POLYGON_OFFSET_CLAMP,                           // 1 float
		POLYGON_OFFSET_FILL,                            // 1 boolean
		POLYGON_OFFSET_LINE,                            // 1 boolean
		POLYGON_OFFSET_POINT,                           // 1 boolean
		POLYGON_SMOOTH,                                 // 1 boolean
		READ_BUFFER,                                    // 1 int (GLenum)
		RENDERBUFFER_BINDING,                           // 1 int
		SAMPLE_COVERAGE_VALUE,                          // 1 float
		SAMPLE_COVERAGE_INVERT,                         // 1 boolean
		SAMPLE_MASK_VALUE,                              // 1 int
		SAMPLER_BINDING,                                // 1 int
		SCISSOR_BOX,                                    // 4 ints
		SCISSOR_TEST,                                   // 1 boolean
		STENCIL_BACK_FAIL,                              // 1 int (GLenum)
		STENCIL_BACK_FUNC,                              // 1 int (GLenum)
		STENCIL_BACK_PASS_DEPTH_FAIL,                   // 1 int (GLenum)
		STENCIL_BACK_PASS_DEPTH_PASS,                   // 1 int (GLenum)
		STENCIL_BACK_REF,                               // 1 int
		STENCIL_BACK_VALUE_MASK,                        // 1 int
		STENCIL_BACK_WRITEMASK,                         // 1 int
		STENCIL_CLEAR_VALUE,                            // 1 int
		STENCIL_FAIL,                                   // 1 int (GLenum)
		STENCIL_FUNC,                                   // 1 int (GLenum)
		STENCIL_PASS_DEPTH_FAIL,                        // 1 int (GLenum)
		STENCIL_PASS_DEPTH_PASS,                        // 1 int (GLenum)
		STENCIL_REF,                                    // 1 int
		STENCIL_TEST,                                   // 1 boolean
		STENCIL_VALUE_MASK,                             // 1 int
		STENCIL_WRITEMASK,                              // 1 int
		TEXTURE_BINDING_1D,                             // 1 int
		TEXTURE_BINDING_1D_ARRAY,                       // 1 int
		TEXTURE_BINDING_2D,                             // 1 int
		TEXTURE_BINDING_2D_ARRAY,                       // 1 int
		TEXTURE_BINDING_2D_MULTISAMPLE,                 // 1 int
		TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,           // 1 int
		TEXTURE_BINDING_3D,                             // 1 int
		TEXTURE_BINDING_BUFFER,                         // 1 int
		TEXTURE_BINDING_CUBE_MAP,                       // 1 int
		TEXTURE_BINDING_RECTANGLE,                      // 1 int
		TIMESTAMP,                                      // 1 int64
		UNPACK_ALIGNMENT,                               // 1 int
		UNPACK_IMAGE_HEIGHT,                            // 1 int
		UNPACK_LSB_FIRST,                               // 1 boolean
		UNPACK_ROW_LENGTH,                              // 1 int
		UNPACK_SKIP_IMAGES,                             // 1 int
		UNPACK_SKIP_PIXELS,                             // 1 int
		UNPACK_SKIP_ROWS,                               // 1 int
		UNPACK_SWAP_BYTES,                              // 1 boolean
		VIEWPORT,                                       // 4 ints (or indexed)
	};
}
