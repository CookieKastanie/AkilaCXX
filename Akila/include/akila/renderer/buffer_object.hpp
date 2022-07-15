#pragma once

#include <glad/glad.h>
#include "akila/math/math.hpp"
#include <vector>

namespace akila {
	class BufferObject {
    protected:
        GLuint id;
        GLenum kind;
        int length;
        GLenum usage;
        GLenum dataType;

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
        virtual void bind() const;
        virtual void setRawData(void const *data, int size, int offset = 0, int typeSize = sizeof(char));
        virtual int getLength() const;

        void setDataType(Type type);
        Type getDataType() const;
    };

    class VBO: public BufferObject {
    private:
        int tupleSize;
        GLuint location;

    public:
        VBO(int tupleSize, unsigned int attributeLocation, Usage usage = Usage::STATIC);

        template<typename T>
        void setData(std::vector<T> const &data);

        int getTupleSize() const;
        unsigned int getLocation() const;

        void bindToArrayBuffer() const;
    };

    class IBO: public BufferObject {
    public:
        IBO(Usage usage = Usage::STATIC);

        template<typename T>
        void setData(std::vector<T> const &data);
    };

    class UBO: public BufferObject {
    private:
        static unsigned int nextBindingPoint;

        unsigned int bindingPoint;

    public:
        UBO(unsigned int size, BufferObject::Usage usage = Usage::DYNAMIC);

        void setData(void const *data);
        unsigned int getBindingPoint();
    };
}
