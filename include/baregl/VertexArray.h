/**
* @project: baregl
* @author: Adrien Givry
* @licence: MIT
*/

#pragma once

#include <baregl/Buffer.h>
#include <baregl/types/EDataType.h>
#include <baregl/types/EGraphicsBackend.h>
#include <baregl/data/VertexAttribute.h>

namespace baregl
{
	/**
	* Represents a vertex array, used to describe a vertex layout to the graphics backend.
	*/
	class VertexArray final
	{
	public:
		/**
		* Creates the vertex array.
		*/
		VertexArray();

		/**
		* Destroys the vertex array.
		*/
		~VertexArray();

		/**
		* Returns true if the vertex array is valid (non-empty layout).
		*/
		bool IsValid() const;

		/**
		* Sets the vertex attribute layout.
		* @param p_attributes
		* @param p_vertexBuffer
		* @param p_indexBuffer
		*/
		void SetLayout(
			data::VertexAttributeLayout p_attributes,
			Buffer& p_vertexBuffer,
			Buffer& p_indexBuffer
		);

		/**
		* Resets the vertex attribute layout.
		*/
		void ResetLayout();

		/**
		* Binds the vertex array.
		*/
		void Bind() const;

		/**
		* Unbinds the vertex array.
		*/
		void Unbind() const;

		/**
		* Returns the vertex array ID.
		*/
		uint32_t GetID() const;

	private:
		uint32_t m_id = 0;
		uint32_t m_attributeCount = 0;
	};
}
