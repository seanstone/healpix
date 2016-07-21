#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <GL/glew.h>

struct Drawable
{
    // Data arrays
    GLfloat* VertexPosition_Data;
    GLfloat* VertexTextureUV_Data;
    void genVertexUV();

    // VBOs
    GLuint VertexPosition_VBO;
    GLuint VertexTextureUV_VBO;
    void updateVertexPosition();
    void updateVertexTextureUV();

    void initData()
    {
        VertexPosition_Data   = new float[NumVertex()*3];
        VertexTextureUV_Data  = new float[NumVertex()*2];
    }

    void initVBO()
    {
        glGenBuffers(1, &VertexPosition_VBO);
        glGenBuffers(1, &VertexTextureUV_VBO);
        updatePosition();
        update();
    }

    ~Drawable()
    {
        glDeleteBuffers(1, &VertexPosition_VBO);
        glDeleteBuffers(1, &VertexTextureUV_VBO);
        delete[] VertexPosition_Data;
        delete[] VertexTextureUV_Data;
    }
};

#endif // HEALPIX_H_INCLUDED
