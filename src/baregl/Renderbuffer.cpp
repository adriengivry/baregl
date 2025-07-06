/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/debug/Assert.h>
#include <baregl/details/glad/glad.h>
#include <baregl/details/Types.h>
#include <baregl/Renderbuffer.h>

namespace baregl
{
	Renderbuffer::Renderbuffer()
	{
		glCreateRenderbuffers(1, &m_id);
	}

	Renderbuffer::~Renderbuffer()
	{
		glDeleteRenderbuffers(1, &m_id);
	}

	void Renderbuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	}

	void Renderbuffer::Unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	uint32_t Renderbuffer::GetID() const
	{
		return m_id;
	}

	void Renderbuffer::Allocate(uint16_t p_width, uint16_t p_height, types::EInternalFormat p_format)
	{
		m_width = p_width;
		m_height = p_height;
		m_format = p_format;

		glNamedRenderbufferStorage(m_id, utils::EnumToValue<GLenum>(m_format), m_width, m_height);

		m_allocated = true;
	}

	bool Renderbuffer::IsValid() const
	{
		return m_allocated;
	}

	void Renderbuffer::Resize(uint16_t p_width, uint16_t p_height)
	{
		BAREGL_ASSERT(IsValid(), "Cannot resize a renderbuffer that has not been allocated");
		Allocate(p_width, p_height, m_format);
	}


	uint16_t Renderbuffer::GetWidth() const
	{
		BAREGL_ASSERT(IsValid(), "Cannot get width of an invalid renderbuffer");
		return m_width;
	}

	uint16_t Renderbuffer::GetHeight() const
	{
		BAREGL_ASSERT(IsValid(), "Cannot get height of an invalid renderbuffer");
		return m_height;
	}
}
