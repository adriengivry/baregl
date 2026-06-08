/**
 * @project: baregl
 * @author: Adrien Givry
 * @licence: MIT
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <baregl/data/GetResultList.h>
#include <baregl/types/EBlendingEquation.h>
#include <baregl/types/EBlendingFactor.h>
#include <baregl/types/EComparaisonAlgorithm.h>
#include <baregl/types/EContextFlags.h>
#include <baregl/types/ECullFace.h>
#include <baregl/types/EDrawBuffer.h>
#include <baregl/types/EGetParameter.h>
#include <baregl/types/EHint.h>
#include <baregl/types/EImageAccessSpecifier.h>
#include <baregl/types/EInternalFormat.h>
#include <baregl/types/ELogicOperation.h>
#include <baregl/types/ETextureUnit.h>
#include <baregl/types/EPixelDataFormat.h>
#include <baregl/types/EPixelDataType.h>
#include <baregl/types/EOperation.h>
#include <baregl/types/EProvokingVertexConvention.h>

namespace baregl::data
{
	template<auto PName> struct GetResult;

	#define DECLARE_GET_RESULT(PARAM, GET_TYPE, COUNT, INDEXED, ...) \
		template <> struct GetResult<types::EGetParameter::PARAM> { \
			using type = __VA_ARGS__; \
			using get_type = GET_TYPE; \
			static constexpr std::size_t count = COUNT; \
			static constexpr bool indexed = INDEXED; \
			static constexpr bool dynamic_count = false; \
		};

	#define DECLARE_DYNAMIC_GET_RESULT(PARAM, GET_TYPE, INDEXED, GET_PARAM, COUNT_PARAMETER, ...) \
		template <> struct GetResult<types::EGetParameter::PARAM> { \
			using type = __VA_ARGS__; \
			using get_type = GET_TYPE; \
			static constexpr std::size_t count = 0; \
			static constexpr bool indexed = INDEXED; \
			static constexpr bool dynamic_count = true; \
			static constexpr types::EGetParameter dynamic_get_parameter = types::EGetParameter::GET_PARAM; \
			static constexpr types::EGetParameter dynamic_count_parameter = types::EGetParameter::COUNT_PARAMETER; \
		};

	BAREGL_GET_RESULTS(DECLARE_GET_RESULT)
	BAREGL_DYNAMIC_GET_RESULTS(DECLARE_DYNAMIC_GET_RESULT)

	#undef DECLARE_DYNAMIC_GET_RESULT
	#undef DECLARE_GET_RESULT

	template<auto P>
	using GetResultType = typename GetResult<P>::type;
}
