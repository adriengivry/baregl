/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <baregl/math/Vec4.h>
#include <baregl/types/EGetParameter.h>

namespace baregl::data
{
	/**
	* Result for a Get query
	*/
	template<auto PName> struct QueryResult;

	// Specialise per query constant:
	template<> struct QueryResult<types::EGetParameter::ACTIVE_TEXTURE>   { using type = int; };
	template<> struct QueryResult<types::EGetParameter::BLEND_COLOR> { using type = math::Vec4; };

	template<auto P>
	using QueryResultType = typename QueryResult<P>::type;
}

