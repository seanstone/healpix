#ifndef HEALPIX_H_INCLUDED
#define HEALPIX_H_INCLUDED

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "quaternion.h"
#include "quad.h"

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

class HEALPix
{
    private:

        float P0(int k);
        float Q0(int k);
        float Sigma(float q);
        float SinTht(float q);
        float Phi(float p, float q);

    public:

        int H = 4, K = 3;   int HK(){return H*K;}
        float ScaleP = 1.0, ScaleQ = 1.0;
        int Depth;          int Dim(){return 1<<Depth;}
        float R;
        quad* Facet;
        mat2 PQ_EN;

        HEALPix(int depth, float r): HEALPix(4, 3, 1.0, 1.0, depth, r){}
        HEALPix(int h, int k, float scaleP, float scaleQ, int depth, float r)
        {
            H = h; K = k; ScaleP = scaleP; ScaleQ = scaleQ;
            Depth = depth; if(quad::maxDim < Dim()) quad::maxDim = Dim();
            R = r;

            PQ_EN = float(M_PI/Dim()) * mat2({ScaleP/H, 0, 0, ScaleQ/(K+1)});

            Facet = new quad[HK()]; for(int f=0; f<HK(); f++) Facet[f].init(Dim());
            genVertexXYZ();
            for(int f=0; f<HK(); f++) Facet[f].genVertexUV();
        }
        ~HEALPix()
        {
            delete[] Facet;

            // Delete VBOs
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

#endif // HEALPIX_H_INCLUDED
