/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#include <baregl/VertexArray.h>

#include <baregl/debug/Assert.h>
#include <baregl/detail/glad/glad.h>
#include <baregl/detail/Types.h>

namespace
{
	uint32_t GetDataTypeSizeInBytes(baregl::types::EDataType p_type)
	{
		switch (p_type)
		{
		case baregl::types::EDataType::BYTE: return sizeof(GLbyte);
		case baregl::types::EDataType::UNSIGNED_BYTE: return sizeof(GLubyte);
		case baregl::types::EDataType::SHORT: return sizeof(GLshort);
		case baregl::types::EDataType::UNSIGNED_SHORT: return sizeof(GLushort);
		case baregl::types::EDataType::INT: return sizeof(GLint);
		case baregl::types::EDataType::UNSIGNED_INT: return sizeof(GLuint);
		case baregl::types::EDataType::FLOAT: return sizeof(GLfloat);
		case baregl::types::EDataType::DOUBLE: return sizeof(GLdouble);
		default: return 0;
		}
	}

	uint32_t CalculateTotalVertexSize(std::span<const baregl::data::VertexAttribute> p_attributes)
	{
		uint32_t result = 0;

		for (const auto& attribute : p_attributes)
		{
			result += GetDataTypeSizeInBytes(attribute.type) * attribute.count;
		}

		return result;
	}
}

namespace baregl
{
	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	bool VertexArray::IsValid() const
	{
		return m_attributeCount > 0;
	}

	void VertexArray::SetLayout(
		data::VertexAttributeLayout p_attributes,
		Buffer& p_vertexBuffer,
		Buffer& p_indexBuffer
	)
	{
		BAREGL_ASSERT(!IsValid(), "Vertex array layout already set");

		Bind();
		p_indexBuffer.Bind(types::EBufferType::INDEX);
		p_vertexBuffer.Bind(types::EBufferType::VERTEX);

		uint32_t attributeIndex = 0;

		const uint32_t totalSize = CalculateTotalVertexSize(p_attributes);
		intptr_t currentOffset = 0;

		for (const auto& attribute : p_attributes)
		{
			BAREGL_ASSERT(attribute.count >= 1 && attribute.count <= 4, "Attribute count must be between 1 and 4");

			glEnableVertexAttribArray(attributeIndex);

			glVertexAttribPointer(
				static_cast<GLuint>(attributeIndex),
				static_cast<GLint>(attribute.count),
				utils::EnumToValue<GLenum>(attribute.type),
				static_cast<GLboolean>(attribute.normalized),
				static_cast<GLsizei>(totalSize),
				reinterpret_cast<const GLvoid*>(currentOffset)
			);

			const uint64_t typeSize = GetDataTypeSizeInBytes(attribute.type);
			const uint64_t attributeSize = typeSize * attribute.count;
			currentOffset += attributeSize;
			++attributeIndex;
			++m_attributeCount;
		}

		Unbind();
		p_indexBuffer.Unbind();
		p_vertexBuffer.Unbind();
	}

	void VertexArray::ResetLayout()
	{
		BAREGL_ASSERT(IsValid(), "Vertex array layout not already set");

		Bind();
		for (uint32_t i = 0; i < m_attributeCount; ++i)
		{
			glDisableVertexAttribArray(i);
		}
		m_attributeCount = 0;
		Unbind();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
