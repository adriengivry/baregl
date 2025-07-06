/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <any>
#include <string>

#include <baregl/types/EUniformType.h>

namespace baregl::data
{
	/**
	* Structure that holds information about a uniform
	*/
	struct UniformInfo
	{
		types::EUniformType type;
		std::string name;
		std::any defaultValue;
	};
}
