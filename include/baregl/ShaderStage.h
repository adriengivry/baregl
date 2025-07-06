/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <baregl/types/EGraphicsBackend.h>
#include <baregl/types/EShaderType.h>
#include <baregl/data/ShaderCompilationResult.h>

namespace baregl
{
	/**
	* Represents a part of a shader program that is responsible for a specific stage (vertex, fragment, geometry, etc.).
	*/
	class ShaderStage final
	{
	public:
		/**
		* Creates a shader stage of the given type.
		* @param p_type
		*/
		ShaderStage(types::EShaderType p_type);

		/**
		* Destructor of the shader stage.
		*/
		~ShaderStage();

		/**
		* Uploads the shader source to the graphics backend memory.
		* @param p_source
		*/
		void Upload(const std::string& p_source) const;

		/**
		* Compiles the uploaded shader source.
		* @note Use this method after uploading the shader source.
		* @return The compilation result.
		*/
		data::ShaderCompilationResult Compile() const;

		/**
		* Returns the ID of the shader stage.
		*/
		uint32_t GetID() const;

		/**
		* Returns the type of shader stage.
		*/
		types::EShaderType GetType() const;

	private:
		uint32_t m_id;
		types::EShaderType m_type;
	};
}
