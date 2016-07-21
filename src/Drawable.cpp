#include "Drawable.hpp"

void Drawable::updateVertexPosition()
{
    glBindBuffer(GL_ARRAY_BUFFER, VertexTextureXYZ_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*NumVertex()*3, VertexTextureXYZ_Data, GL_STATIC_DRAW);
}

void Drawable::updateVertexTextureUV()
{
    glBindBuffer(GL_ARRAY_BUFFER, VertexTextureUV_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*NumVertex()*2, VertexTextureUV_Data, GL_DYNAMIC_DRAW);
}
