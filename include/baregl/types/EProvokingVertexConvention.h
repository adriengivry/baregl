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
	* Provoking vertex convention.
	*/
	enum class EProvokingVertexConvention : uint8_t
	{
		FIRST_VERTEX_CONVENTION,
		LAST_VERTEX_CONVENTION,
		UNDEFINED_VERTEX
	};
}
