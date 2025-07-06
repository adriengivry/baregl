/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <optional>

#include <baregl/types/EGraphicsBackend.h>
#include <baregl/types/ETextureType.h>

namespace baregl
{
	/**
	* Represents a texture handle, acts as a view to the texture.
	*/
	class TextureHandle
	{
	public:
		/**
		* Binds the texture to the given slot.
		* @param p_slot Optional slot to bind the texture to.
		*/
		void Bind(std::optional<uint32_t> p_slot = std::nullopt) const;

		/**
		* Unbinds the texture.
		*/
		void Unbind() const;

		/**
		* Returns the ID associated with the texture.
		* @return The texture ID.
		*/
		uint32_t GetID() const;

		/**
		* Returns the texture type
		*/
		types::ETextureType GetType() const;

	protected:
		TextureHandle(types::ETextureType p_type);
		TextureHandle(types::ETextureType p_type, uint32_t p_id);

	protected:
		uint32_t m_id;
		const uint32_t m_type;
	};
}
