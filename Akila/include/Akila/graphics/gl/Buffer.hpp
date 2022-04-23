#pragma once

#include <glad/glad.h>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Akila {
    class Buffer {
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

            Buffer(unsigned int kind, Usage usage);
            virtual ~Buffer();
            virtual void bind() const;
            virtual void setRawData(const void *data, int size, int offset = 0, int typeSize = sizeof(char));
            virtual int getLength() const;

            void setDataType(Type type);
            Type getDataType() const;
    };

    class VBO: public Buffer {
        private:
            int tupleSize;
            GLuint location;

        public:
            VBO(int tupleSize, unsigned int attributeLocation, Usage usage = Usage::STATIC);

            template<typename T>
            void setData(const std::vector<T> &data);

            int getTupleSize() const;
            unsigned int getLocation() const;

            void bindToArrayBuffer() const;
    };

    class IBO: public Buffer {
    public:
        IBO(Usage usage = Usage::STATIC);

        template<typename T>
        void setData(const std::vector<T> &data);
    };

    class UBO: public Buffer {
        private:
            static unsigned int nextBindingPoint;

            unsigned int bindingPoint;

        public:
            UBO(unsigned int size, Buffer::Usage usage = Usage::DYNAMIC);

            void setData(const void *data);
            unsigned int getBindingPoint();
    };
}
