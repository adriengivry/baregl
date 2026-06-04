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
	template<auto PName> struct QueryResult;

	template <> struct QueryResult<types::EGetParameter::BLEND> {
		using type = bool;
		using query_type = bool;
		static constexpr std::size_t count = 1;
		static constexpr bool indexed = false;
	};

	template <> struct QueryResult<types::EGetParameter::CULL_FACE_MODE> {
		using type = types::ECullFace;
		using query_type = int;
		static constexpr std::size_t count = 1;
		static constexpr bool indexed = false;
	};

	template <> struct QueryResult<types::EGetParameter::ACTIVE_TEXTURE> {
		using type = uint32_t;
		static constexpr bool indexed = false;
	};

	template<auto P>
	using QueryResultType = typename QueryResult<P>::type;
}
