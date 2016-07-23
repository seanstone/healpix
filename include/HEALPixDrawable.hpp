#ifndef HEALPIXDRAWABLE_HPP
#define HEALPIXDRAWABLE_HPP

#include "HEALPix.hpp"
#include "GLAttribute.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

struct HEALPixDrawable : public HEALPix<100>
{
    // Attributes
    Attribute<GLfloat>  vertexPosition;
    Attribute<GLfloat>  vertexTextureUV;

    // Indices
    Index               meshIndex;

    // Uniforms
    Uniform<GLint>      sampler;
    Uniform<mat4>       MVP;

    // Texture
    GLuint heightMapTexture;

    // Program
    GLuint program;

    HEALPixDrawable()
    {
        //HEALPix();
        //vertexPosition = Attribute<GLfloat>((float*)Vertices, 4 * NumVertex(), GL_STATIC_DRAW);
        //vertexTextureUV = Attribute<GLfloat>(NULL, 2 * NumVertex(), GL_DYNAMIC_DRAW);
        //meshIndex = Index(Indices, NumIndex(), GL_STATIC_DRAW);
    }

    void init()
    {
        program                 = loadShaders("glsl/vertex.glsl", "glsl/fragment.glsl");
        sampler.id              = glGetUniformLocation  (program, "Sampler");
        MVP.id                  = glGetUniformLocation  (program, "MVP");
        vertexPosition.id       = glGetAttribLocation   (program, "VertexXYZ");
        vertexTextureUV.id      = glGetAttribLocation   (program, "VertexUV");

        // Use shader
        glUseProgram(program);
        initTexture();
    }

    ~HEALPixDrawable()
    {
        glDeleteTextures(1, &heightMapTexture);
        glDeleteProgram(program);
    }

    void initTexture()
    {
        heightMapTexture = loadBMP("textures/earth.bmp");
        glActiveTexture(GL_TEXTURE0); // Bind our texture in Texture Unit 0
        glBindTexture(GL_TEXTURE_2D, heightMapTexture); // Set our sampler to use Texture Unit 0
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glUniform1i(sampler.id, 0);
    }

    void draw()
    {
        // Enable attributes
        vertexPosition.enable();
        vertexTextureUV.enable();

        // Send VertexXYZ to OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, vertexPosition.vbo);
        glVertexAttribPointer(vertexPosition.id, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Send VertexUV to OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, vertexTextureUV.vbo);
        glVertexAttribPointer(vertexTextureUV.id, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndex.vbo);
        glDrawElements(GL_TRIANGLES, NumIndex(), GL_UNSIGNED_INT, (void*)(0));

        // Disable attributes
        vertexPosition.disable();
        vertexTextureUV.disable();
    }

};

#endif
