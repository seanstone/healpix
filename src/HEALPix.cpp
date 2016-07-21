#include "HEALPix.hpp"
#include <stdlib.h>

void HEALPix::initVBO()
{
    glGenBuffers(1, &VBO_VertexIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_VertexIndex);
	GLuint* VertexIndex = quad::genVertexTriangulationIndex();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*quad::maxNumTriangulationVertex(), VertexIndex, GL_STATIC_DRAW);
	delete[] VertexIndex;
    for(int f=0; f<HK(); f++) Facet[f].initVBO();
}

void HEALPix::draw()
{
    // Enable attributes
    glEnableVertexAttribArray(In_VertexXYZ);
    glEnableVertexAttribArray(In_VertexUV);
    for(int f=0; f<HK(); f++)
    {
        // Send VertexXYZ to OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, Facet[f].VBO_VertexXYZ);
        glVertexAttribPointer(In_VertexXYZ,3,GL_FLOAT,GL_FALSE,0,(void*)0);

        // Send VertexUV to OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, Facet[f].VBO_VertexUV);
        glVertexAttribPointer(In_VertexUV,2,GL_FLOAT,GL_FALSE,0,(void*)0);

        // Draw
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_VertexIndex);
        glDrawElements(GL_TRIANGLE_STRIP, Facet[f].NumTriangulationVertex(), GL_UNSIGNED_INT, (void*)(0));
    }
    // Disable attributes
    glDisableVertexAttribArray(In_VertexXYZ);
    glDisableVertexAttribArray(In_VertexUV);
}
