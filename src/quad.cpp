#include "quad.h"
#include <noise/noise.h>

int quad::NumPixel(int dim) { return dim*dim; }
int quad::NumVertex(int dim) { return (dim+1)*(dim+1); }
int quad::NumTriangulationVertex(int dim) { return 2*NumPixel(dim) + 2*dim; }

int quad::NumPixel() { return NumPixel(Dim); }
int quad::NumVertex() { return NumVertex(Dim); }
int quad::NumTriangulationVertex() { return NumTriangulationVertex(Dim); }

int quad::maxDim;
int quad::maxNumPixel() { return NumPixel(maxDim); }
int quad::maxNumVertex() { return NumVertex(maxDim); }
int quad::maxNumTriangulationVertex() { return NumTriangulationVertex(maxDim); }

vec2 quad::IJ(int n)
{
    int l = int(sqrt(n));   // shell
    int r = n - l*l;        // remainder
    vec2 ij = r<l ? vec2(r,l) : vec2(l,2*l-r); // (i,j) for odd l
    return l&1 ? ij : vec2({ij.y,ij.x});
}

int quad::N(vec2 ij)
{
    int i = ij.x, j = ij.y;
    return i<j ? j*j + !(j&1)*(2*j-i) + (j&1)*i : i*i + !(i&1)*j + (i&1)*(2*i-j);
}

unsigned int* quad::genVertexTriangulationIndex()
{
    unsigned int* index = new unsigned int[maxNumTriangulationVertex()];
    int x = 0;
    for(int n = 0; n<maxNumPixel(); n++)
    {
        int l = int(sqrt(n));   // shell
        int r = n - l*l;        // remainder
        int dir = (2*l + (r>=l) -1)%4; // direction: 0 = (0,-1), 1 = (0,1), 2 = (-1,0), 3 = (1,0)
        switch(dir)
        {
            case 0:
                if (r == l)     index[x++] = N(IJ(n) + vec2(1,0));
                index[x++] = N(IJ(n) + vec2(1,1));
                index[x++] = N(IJ(n) + vec2(0,0));
                if (r == 2*l)   index[x++] = N(IJ(n) + vec2(0,1));
                break;
            case 1:
                if (r == l)     index[x++] = N(IJ(n) + vec2(0,0));
                index[x++] = N(IJ(n) + vec2(0,1));
                index[x++] = N(IJ(n) + vec2(1,0));
                if (r == 2*l)   index[x++] = N(IJ(n) + vec2(1,1));
                break;
            case 2:
                if (r == l)     index[x++] = N(IJ(n) + vec2(0,1));
                index[x++] = N(IJ(n) + vec2(1,1));
                index[x++] = N(IJ(n) + vec2(0,0));
                if (r == 2*l)   index[x++] = N(IJ(n) + vec2(1,0));
                break;
            case 3:
                if (r == l)     index[x++] = N(IJ(n) + vec2(0,0));
                index[x++] = N(IJ(n) + vec2(1,0));
                index[x++] = N(IJ(n) + vec2(0,1));
                if (r == 2*l)   index[x++] = N(IJ(n) + vec2(1,1));
                break;
        }
    }
    return index;
}

void quad::genVertexUV()
{
    //int rr = (rand()%100)/10;

    static noise::module::Perlin myModule;
    myModule.SetFrequency (0.9);
    myModule.SetPersistence (0.6);

    for(int n=0; n<NumVertex(); n++)
    {
        quat xyz = quat(VertexXYZ[n*3], VertexXYZ[n*3+1], VertexXYZ[n*3+2]);
        VertexUV[n*2] = myModule.GetValue((xyz.x+1.52)*0.50, (xyz.y+1.52)*0.50, xyz.z*0.50)*0.25 + 0.57;
        VertexUV[n*2+1] = VertexUV[n*2];
    }
}

void quad::updateVBO_VertexXYZ()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO_VertexXYZ);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*NumVertex()*3, VertexXYZ, GL_STATIC_DRAW);
}

void quad::updateVBO_VertexUV()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO_VertexUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*NumVertex()*2, VertexUV, GL_DYNAMIC_DRAW);
}
