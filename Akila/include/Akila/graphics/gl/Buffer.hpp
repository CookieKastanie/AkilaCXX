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

        public:
            enum Usage: GLenum {
                STATIC = GL_STATIC_DRAW,
                DYNAMIC = GL_DYNAMIC_DRAW
            };

            enum Type: GLenum {
                FLOAT = GL_FLOAT,
                UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
                INT = GL_INT
            };

            Buffer(unsigned int kind, unsigned int usage);
            virtual ~Buffer();
            virtual void bind() const;
            virtual void setRawData(const void *data, int size, int offset = 0);
            virtual int getLength() const;
    };

    class VBO: public Buffer {
        private:
            int tupleSize;
            GLuint location;

        public:
            VBO(int tupleSize, unsigned int attributeLocation, Usage usage = STATIC);

            template<typename T>
            void setData(const std::vector<T> &data);

            int getTupleSize() const;
            unsigned int getLocation() const;

            void bindToArrayBuffer(Type dataType = FLOAT) const;
    };

    class UBO: public Buffer {
        private:
            static unsigned int nextBindingPoint;

            unsigned int bindingPoint;

        public:
            UBO(unsigned int size, Buffer::Usage usage = DYNAMIC);

            void setData(const void *data);
            unsigned int getBindingPoint();
    };
}
