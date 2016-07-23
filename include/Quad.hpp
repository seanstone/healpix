#ifndef QUAD_H_INCLUDED
#define QUAD_H_INCLUDED

#include "Vec.hpp"

template <int Dim>
struct Quad
{
    float4*         Vertices;
    unsigned int*   Indices;

    inline int NumPixel()                   { return Dim*Dim;; }
    inline int NumVertex()                  { return (Dim+1)*(Dim+1); }
    inline int NumTriangleStripIndex()      { return 2*NumPixel() + 2*Dim; }
    inline int NumIndex()                   { return 6*NumPixel(); }

    static float2 IJ(int n)
    {
        int l = int(sqrt(n));   // shell
        int r = n - l*l;        // remainder
        float2 ij = r<l ? float2(r,l) : float2(l,2*l-r); // (i,j) for odd l
        return l&1 ? ij : float2({ij.y,ij.x});
    }

    static int N(float2 ij)
    {
        int i = ij.x, j = ij.y;
        return i<j ? j*j + !(j&1)*(2*j-i) + (j&1)*i : i*i + !(i&1)*j + (i&1)*(2*i-j);
    }

    unsigned int* genTriangleStripIndices(unsigned int* index)
    {
        int x = 0;
        for(int n = 0; n<NumPixel(); n++)
        {
            int l = int(sqrt(n));           // shell
            int r = n - l*l;                // remainder
            int dir = (2*l + (r>=l) -1)%4;  // direction: 0 = (0,-1), 1 = (0,1), 2 = (-1,0), 3 = (1,0)
            switch(dir)
            {
                case 0:
                    if (r == l)     index[x++] = N(IJ(n) + float2(1,0));
                    index[x++] = N(IJ(n) + float2(1,1));
                    index[x++] = N(IJ(n) + float2(0,0));
                    if (r == 2*l)   index[x++] = N(IJ(n) + float2(0,1));
                    break;
                case 1:
                    if (r == l)     index[x++] = N(IJ(n) + float2(0,0));
                    index[x++] = N(IJ(n) + float2(0,1));
                    index[x++] = N(IJ(n) + float2(1,0));
                    if (r == 2*l)   index[x++] = N(IJ(n) + float2(1,1));
                    break;
                case 2:
                    if (r == l)     index[x++] = N(IJ(n) + float2(0,1));
                    index[x++] = N(IJ(n) + float2(1,1));
                    index[x++] = N(IJ(n) + float2(0,0));
                    if (r == 2*l)   index[x++] = N(IJ(n) + float2(1,0));
                    break;
                case 3:
                    if (r == l)     index[x++] = N(IJ(n) + float2(0,0));
                    index[x++] = N(IJ(n) + float2(1,0));
                    index[x++] = N(IJ(n) + float2(0,1));
                    if (r == 2*l)   index[x++] = N(IJ(n) + float2(1,1));
                    break;
            }
        }
        return index;
    }

    inline int3 sort(int3 triangle)
    {
        int temp;
        if (triangle[0] > triangle[1]) { temp = triangle[1]; triangle[1] = triangle[0]; triangle[0] = temp; }
        if (triangle[1] > triangle[2]) { temp = triangle[2]; triangle[2] = triangle[1]; triangle[1] = temp; }
        if (triangle[0] > triangle[1]) { temp = triangle[1]; triangle[1] = triangle[0]; triangle[0] = temp; }
        return triangle;
    }

    void genIndices()
    {
        unsigned int* triangleStripIndex = new unsigned int[NumTriangleStripIndex()];
        genTriangleStripIndices(triangleStripIndex);

        int i = 0;
        int3 triangle, lastTraingle;
        float2 side1, side2;
        for (int x=0; x<NumTriangleStripIndex()-2; x++)
        {
            triangle[0] = triangleStripIndex[x];
            triangle[1] = triangleStripIndex[x+1];
            triangle[2] = triangleStripIndex[x+2];
            side1 = IJ(triangle[1]) - IJ(triangle[0]);
            side2 = IJ(triangle[2]) - IJ(triangle[0]);

            if (1 - abs(glm::dot(side1, side2)) / (glm::length(side1) * glm::length(side2)) > 1e-6 && sort(triangle) != sort(lastTraingle))
            {
                Indices[i++] = triangle[0];
                Indices[i++] = triangle[1];
                Indices[i++] = triangle[2];
                lastTraingle = triangle;
            }
        }
        delete [] triangleStripIndex;
    }

    void createBuffers()
    {
        Vertices = new float4 [NumVertex()];
        Indices = new unsigned int [NumIndex()];
    }

    void deleteBuffers()
    {
        delete [] Vertices;
        delete [] Indices;
    }

    void genVertices()
    {
        for (int n=0; n<NumVertex(); n++)
            Vertices[n] = float4(IJ(n).x, IJ(n).y, 0, 1);
    }
};

#endif // QUAD_H_INCLUDED
