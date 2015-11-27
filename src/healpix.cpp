#include "healpix.h"
#include <stdlib.h>

float HEALPix::P0(int k){return (k&1)*M_PI/H;}
float HEALPix::Q0(int k){return -M_PI/2 + (k+2)*M_PI/(K+1);}
float HEALPix::Sigma(float q){return (K+1)/2 - std::abs(q*H)/M_PI;}
float HEALPix::SinTht(float q){return std::abs(q) <= M_PI_2*(K-1)/H  ?  q*H/K/M_PI_2 : (((q>=0)<<1)-1)*(1-Sigma(q)*Sigma(q)/K);}
float HEALPix::Phi(float p, float q){return std::abs(q) <= M_PI_2*(K-1)/H  ?  p : p/Sigma(q);}

void HEALPix::genVertexXYZ()
{
    for(int k=0; k<=K/2; k++)
    {
        for(int n=0; n<Facet[k].NumVertex(); n++)
        {
            vec2 pq = PQ_EN * mat2({-1, 1, -1, -1}) * Facet[k].IJ(n);
            float tht = Q0(k) + pq.y, phi = P0(k) + Phi(pq.x,tht);
            float z = SinTht(tht), costht = sqrt(1-z*z);
            quat xyz0 = quat(0, R*costht*cos(phi), R*costht*sin(phi), R*z);

            for(int f=k; f<HK(); f+=K)
            {
                quat Rotor = rotor(float(f/K*2*M_PI/H), quat(0,0,1));
                quat xyz = Rotor * xyz0 * ~Rotor;
                Facet[f].VertexXYZ[n*3]     = xyz.x;
                Facet[f].VertexXYZ[n*3+1]   = xyz.y;
                Facet[f].VertexXYZ[n*3+2]   = xyz.z;

                Facet[f+K-2*k-1].VertexXYZ[n*3]     = xyz.x;
                Facet[f+K-2*k-1].VertexXYZ[n*3+1]   = xyz.y;
                Facet[f+K-2*k-1].VertexXYZ[n*3+2]   = -xyz.z;
            }
        }
    }
}

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

void HEALPix::update()
{
    for(int f=0; f<HK(); f++)
    {
        //Facet[f].genVertexUV();
        /*for(int n=0; n<Facet[f].NumVertex(); n++)
        {
            if(E(f,n)>0 && E(f,n)<Facet[f].NumVertex())
            {
                float c = float(rand()%100-49)/10000.0;
                Facet[f].VertexUV[n*2] = Facet[f].VertexUV[E(f,n)*2] + c;
                Facet[f].VertexUV[n*2+1] = Facet[f].VertexUV[n*2];
            }
            else if(f+1<HK())
            {
                float c = float(rand()%100-49)/10000.0;
                Facet[f+1].VertexUV[n*2] = Facet[f].VertexUV[E(f,n)*2] + c;
                Facet[f+1].VertexUV[n*2+1] = Facet[f].VertexUV[n*2];
            }
        }
        Facet[f].updateVBO_VertexUV();*/
    }
}
