#ifndef HEALPIXDRAWABLE_HPP
#define HEALPIXDRAWABLE_HPP

#include "HEALPix.hpp"
#include "Drawable.hpp"

class HEALPixDrawble : public HEALPix, public Drawable
{
    HEALPixDrawble(int h, int k, float scaleP, float scaleQ, int depth, float r)
    {
        genVertexXYZ();
        for(int f=0; f<HK(); f++) Facet[f].genVertexUV();
    }
    ~HEALPixDrawble()
    {
        delete[] Facet;
        glDeleteBuffers(1, &VBO_VertexIndex);
    }

    void genVertexXYZ();

    void update();

    GLuint VBO_VertexIndex;
    GLuint Uniform_Rotor;
    GLuint Uniform_Sampler;
    GLuint Uniform_MVP;
    GLuint In_VertexXYZ;
    GLuint In_VertexUV;
    void initVBO();
    void draw();
};

#endif
