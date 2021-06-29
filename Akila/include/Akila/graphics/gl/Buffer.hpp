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
            static unsigned int const STATIC = GL_STATIC_DRAW;
            static unsigned int const DYNAMIC = GL_DYNAMIC_DRAW;

            static unsigned int const FLOAT = GL_FLOAT;
            static unsigned int const UNSIGNED_SHORT = GL_UNSIGNED_SHORT;
            static unsigned int const INT = GL_INT;
            

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
            VBO(int tupleSize, unsigned int attributeLocation, unsigned int usage = STATIC);

            template<typename T>
            void setData(const std::vector<T> &data);

            int getTupleSize() const;
            unsigned int getLocation() const;

            void bindToArrayBuffer(unsigned int dataType = FLOAT) const;
    };

    class UBO: public Buffer {
        private:
            static unsigned int nextBindingPoint;

            unsigned int bindingPoint;

        public:
            UBO(unsigned int size, unsigned int usage = DYNAMIC);

            void setData(const void *data);
            unsigned int getBindingPoint();
    };
}
