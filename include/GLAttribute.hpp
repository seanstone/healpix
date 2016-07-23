#ifndef GLATTRIBUTE_HPP
#define GLATTRIBUTE_HPP

// GLEW
#include <GL/glew.h>

template <typename GLtype>
struct Uniform
{
    GLuint  id;
    GLtype  data;
    void update();
};

template <GLenum target, typename GLtype>
struct GLattribute
{
    GLtype* data;
    int     n_elements;
    GLuint  id;
    GLuint  vbo;
    GLenum  usage;

    GLattribute() {}
    GLattribute(GLtype* data, int n_elements, GLenum usage = GL_STATIC_DRAW) : data(data), n_elements(n_elements), usage(usage) {}

    void init()     { glGenBuffers(1, &vbo); }
    void update()
    {
        glBindBuffer(target, vbo);
        glBufferData(target, sizeof(GLtype) * n_elements, data, usage);
    }
    void enable()   { glEnableVertexAttribArray(id); }
    void disable()  { glDisableVertexAttribArray(id); }
    ~GLattribute()  { glDeleteBuffers(1, &vbo); }
};

// Attribute
template<typename GLtype>
using Attribute = GLattribute<GL_ARRAY_BUFFER, GLtype>;

// Index
typedef GLattribute<GL_ELEMENT_ARRAY_BUFFER, GLuint> Index;

#endif
