#ifndef QUAD_H_INCLUDED
#define QUAD_H_INCLUDED

#include "quaternion.h"
#include "vec.h"

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

class quad
{
    public:

        //================================================================

        static int NumPixel(int dim);
        static int NumVertex(int dim);
        static int NumTriangulationVertex(int dim);

        static int maxDim;
        static int maxNumPixel();
        static int maxNumVertex();
        static int maxNumTriangulationVertex();

        static vec2 IJ(int n);
        static int N(vec2 ij);

        static unsigned int* genVertexTriangulationIndex();

        //================================================================

        int Dim;
        int NumPixel();
        int NumVertex();
        int NumTriangulationVertex();

        void init(int dim)
        {
            Dim = dim;
            VertexXYZ   = new float[NumVertex()*3];
            VertexUV    = new float[NumVertex()*2];
        }

        void initVBO()
        {
            glGenBuffers(1, &VBO_VertexXYZ);    updateVBO_VertexXYZ();
            glGenBuffers(1, &VBO_VertexUV);     updateVBO_VertexUV();
        }

        ~quad()
        {
            delete[] VertexXYZ;
            delete[] VertexUV;
            glDeleteBuffers(1, &VBO_VertexXYZ);
            glDeleteBuffers(1, &VBO_VertexUV);
        }

        GLfloat* VertexXYZ;
        GLfloat* VertexUV;
        void genVertexUV();

        GLuint VBO_VertexXYZ;
        GLuint VBO_VertexUV;
        void updateVBO_VertexXYZ();
        void updateVBO_VertexUV();

        vec2 translate(int n, vec2 delta)
        {
            vec2 ij = IJ(n) + delta; int side = -1;
            if(ij.y < 0) side = 0;
            if(ij.x > 0) side = 1;
            if(ij.y > 0) side = 2;
            if(ij.x < 0) side = 3;
            return ij;
        }

        int BorderQuad[4]; int BorderNum[4];
};

#endif // QUAD_H_INCLUDED
