#ifndef HEALPIXDRAWABLE_HPP
#define HEALPIXDRAWABLE_HPP

#include "HEALPix.hpp"

class HEALPixDrawble : public HEALPix
{
    // Data arrays
    GLfloat* VertexPosition_Data;
    GLfloat* VertexTextureUV_Data;

    void genVertexUV();

    // VBOs
    GLuint VertexPosition_VBO;
    GLuint VertexTextureUV_VBO;
    GLuint MeshIndex_VBO;
    void updateVertexPosition();
    void updateVertexTextureUV();

    // Uniforms and attributes
    GLuint Uniform_Rotor;
    GLuint Uniform_Sampler;
    GLuint Uniform_MVP;
    GLuint In_VertexPosition;
    GLuint In_VertexTextureUV;

    void initData()
    {
        VertexTextureUV_Data  = new float[NumVertex()*2];
    }

    void initVBO()
    {
        glGenBuffers(1, &VertexPosition_VBO);
        glGenBuffers(1, &VertexTextureUV_VBO);
        updatePosition();
    }

    ~HEALPixDrawble()
    {
        glDeleteBuffers(1, &VertexPosition_VBO);
        glDeleteBuffers(1, &VertexTextureUV_VBO);
        glDeleteBuffers(1, &VBO_VertexIndex);
        delete[] VertexPosition_Data;
        delete[] VertexTextureUV_Data;
    }

    void draw();
};

#endif
