#pragma once

#include <glad/glad.h>
#include "akila/core/math/math.hpp"
#include <vector>

namespace akila {
	class BufferObject {
	public:
		enum class Usage: GLenum {
			STATIC = GL_STATIC_DRAW,
			DYNAMIC = GL_DYNAMIC_DRAW
		};

		enum class Type: GLenum {
			FLOAT = GL_FLOAT,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			INT = GL_INT
		};

		BufferObject(unsigned int kind, Usage usage);
		virtual ~BufferObject();

		BufferObject(BufferObject &&other) noexcept;
		virtual BufferObject &operator=(BufferObject &&other) noexcept;

		BufferObject(BufferObject const &other) = delete;
		virtual BufferObject &operator=(BufferObject const &other) = delete;

		virtual void bind() const;
		virtual void setRawData(void const *data, int size, int offset = 0, int typeSize = sizeof(char));
		virtual int getLength() const;

		void setDataType(Type type);
		Type getDataType() const;

	protected:
		GLuint id;
		GLenum kind;
		int length;
		GLenum usage;
		GLenum dataType;
	};

	class VBO: public BufferObject {
	public:
		VBO(int tupleSize, unsigned int attributeLocation, Usage usage = Usage::STATIC);

		VBO(VBO &&other) noexcept;
		VBO &operator=(VBO &&other) noexcept;

		template<typename T>
		void setData(std::vector<T> const &data);

		int getTupleSize() const;
		unsigned int getLocation() const;

		void bindToArrayBuffer() const;

	private:
		int tupleSize;
		GLuint location;
	};

	class IBO: public BufferObject {
	public:
		IBO(Usage usage = Usage::STATIC);

		template<typename T>
		void setData(std::vector<T> const &data);
	};

	class UBO: public BufferObject {
	public:
		UBO(unsigned int bindingPoint, unsigned int size, BufferObject::Usage usage = Usage::DYNAMIC);

		UBO(UBO &&other) noexcept;
		UBO &operator=(UBO &&other) noexcept;

		void setData(void const *data);
		unsigned int getBindingPoint();

	private:
		unsigned int bindingPoint;
	};
}
