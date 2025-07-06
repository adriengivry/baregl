/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/debug/Assert.h>
#include <baregl/details/glad/glad.h>
#include <baregl/details/Types.h>
#include <baregl/Buffer.h>

namespace baregl
{
	Buffer::Buffer(types::EBufferType p_type) :
		m_type{ p_type }
	{
		glCreateBuffers(1, &m_id);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	uint64_t Buffer::Allocate(uint64_t p_size, types::EAccessSpecifier p_usage)
	{
		BAREGL_ASSERT(IsValid(), "Cannot allocate memory for an invalid buffer");
		glNamedBufferData(m_id, p_size, nullptr, utils::EnumToValue<GLenum>(p_usage));
		return m_allocatedBytes = p_size;
	}

	void Buffer::Upload(const void* p_data, std::optional<data::BufferMemoryRange> p_range)
	{
		BAREGL_ASSERT(IsValid(), "Trying to upload data to an invalid buffer");
		BAREGL_ASSERT(!IsEmpty(), "Trying to upload data to an empty buffer");

		glNamedBufferSubData(
			m_id,
			p_range ? p_range->offset : 0,
			p_range ? p_range->size : m_allocatedBytes,
			p_data
		);
	}

	void Buffer::Bind(std::optional<uint32_t> p_index) const
	{
		BAREGL_ASSERT(IsValid(), "Cannot bind an invalid buffer");

		if (p_index.has_value())
		{
			glBindBufferBase(utils::EnumToValue<GLenum>(m_type), p_index.value(), m_id);
		}
		else
		{
			glBindBuffer(utils::EnumToValue<GLenum>(m_type), m_id);
		}
	}

	void Buffer::Unbind() const
	{
		BAREGL_ASSERT(IsValid(), "Cannot unbind an invalid buffer");
		glBindBuffer(utils::EnumToValue<GLenum>(m_type), 0);
	}

	bool Buffer::IsValid() const
	{
		return
			m_id != 0 &&
			m_type != types::EBufferType::UNKNOWN;
	}

	bool Buffer::IsEmpty() const
	{
		return GetSize() == 0;
	}

	uint64_t Buffer::GetSize() const
	{
		BAREGL_ASSERT(IsValid(), "Cannot get size of an invalid buffer");
		return m_allocatedBytes;
	}

	uint32_t Buffer::GetID() const
	{
		BAREGL_ASSERT(IsValid(), "Cannot get ID of an invalid buffer");
		return m_id;
	}
}
