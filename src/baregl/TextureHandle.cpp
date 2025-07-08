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

	void TextureHandle::Unbind() const
	{
		glBindTexture(m_type, 0);
	}

	types::ETextureType TextureHandle::GetType() const
	{
		return utils::ValueToEnum<types::ETextureType>(m_type);
	}
}
