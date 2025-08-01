/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/detail/glad/glad.h>
#include <baregl/detail/Types.h>
#include <baregl/TextureHandle.h>

namespace baregl
{
	TextureHandle::TextureHandle(types::ETextureType p_type) :
		m_type{ utils::EnumToValue<GLenum>(p_type) }
	{
	}

	TextureHandle::TextureHandle(types::ETextureType p_type, uint32_t p_id) : 
		detail::NativeObject(p_id),
		m_type{ utils::EnumToValue<GLenum>(p_type) }
	{
	}

	void TextureHandle::Bind(std::optional<uint32_t> p_slot) const
	{
		if (p_slot.has_value())
		{
			glBindTextureUnit(p_slot.value(), m_id);
		}
		else
		{
			glBindTexture(m_type, m_id);
		}
	}
	
	void TextureHandle::Bind(uint32_t p_slot, types::EImageAccessSpecifier p_access, types::EInternalFormat p_format, uint32_t p_level, std::optional<uint32_t> p_layer) const
	{
		const bool layered = !p_layer.has_value();
		const uint32_t layer = layered ? 0 : p_layer.value();

		glBindImageTexture(p_slot, m_id, p_level, layered, layer, utils::EnumToValue<GLenum>(p_access), utils::EnumToValue<GLenum>(p_format));
	}

	void TextureHandle::Unbind() const
	{
		glBindTexture(m_type, 0);
	}

	types::ETextureType TextureHandle::GetType() const
	{
		return utils::ValueToEnum<types::ETextureType>(m_type);
	}
}
