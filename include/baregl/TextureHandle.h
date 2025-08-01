/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <optional>

#include <baregl/detail/NativeObject.h>
#include <baregl/types/EGraphicsBackend.h>
#include <baregl/types/ETextureType.h>

namespace baregl
{
	/**
	* Represents a texture handle, acts as a view to the texture.
	*/
	class TextureHandle : public detail::NativeObject
	{
	public:
		/**
		* Binds the texture to the given slot.
		* @param p_slot Optional slot to bind the texture to.
		*/
		void Bind(std::optional<uint32_t> p_slot = std::nullopt) const;
		
		/**
		* Binds the texture to the given slot as an image texture.
		* @param p_slot Slot to bind the texture to.
		* @param p_access The level of access the shader has to the image.
		* @param p_format The format that the shader will treat the image as.
		* @param p_level Mipmap level of the texture that will be bound.
		* @param p_layer Optional layer of the texture to bind.
		*/
		void Bind(uint32_t p_slot, types::EImageAccessSpecifier p_access, types::EInternalFormat p_format, uint32_t p_level = 0, std::optional<uint32_t> p_layer = std::nullopt) const;

		/**
		* Unbinds the texture.
		*/
		void Unbind() const;

		/**
		* Returns the texture type
		*/
		types::ETextureType GetType() const;

	protected:
		TextureHandle(types::ETextureType p_type);
		TextureHandle(types::ETextureType p_type, uint32_t p_id);

	protected:
		const uint32_t m_type;
	};
}
