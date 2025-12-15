/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <baregl/types/EDataType.h>

#include <initializer_list>

namespace baregl::data
{
	/**
	* Structure that holds information about a uniform
	*/
	struct VertexAttribute
	{
		types::EDataType type = types::EDataType::FLOAT;
		uint8_t count = 4;
		bool normalized = false;
	};

	using VertexAttributeLayout = std::initializer_list<const data::VertexAttribute>;
}
