#ifndef HEALPIX_H_INCLUDED
#define HEALPIX_H_INCLUDED

#include "Quad.hpp"
#include <noise/noise.h>

template <int Dim, int H = 4, int K = 3>
struct HEALPix
{
    // Data arrays
    float3*         Vertices;
    unsigned int*   Indices;
    float3*         Normals;
    inline int NumVertex()      { return HK * Facet[0].NumVertex(); }
    inline int NumIndex()       { return HK * Facet[0].NumIndex(); }

    // Facets
    Quad<Dim>*      Facet;
    const int       HK          = H*K;

    // Parameters
    float           Radius;
    float3          Origin;

    HEALPix(float Radius = 1.f, float3 Origin = float3(0), float ScaleP = 1.f, float ScaleQ = 1.f) : Radius(Radius), Origin(Origin)
    {
        Facet = new Quad<Dim>[HK];

        Vertices    = new float3    [Facet[0].NumVertex()*HK];
        Indices     = new unsigned int [Facet[0].NumIndex()*HK];
        Normals     = new float3    [Facet[0].NumVertex()*HK];

        for (int f=0; f<HK; f++)
        {
            Facet[f].Vertices   = Vertices + f * Facet[0].NumVertex();
            Facet[f].Indices    = Indices + f * Facet[0].NumIndex();
            Facet[f].Normals    = Normals + f * Facet[0].NumVertex();
            Facet[f].genIndices();
            for(int n=0; n<Facet[0].NumIndex(); n++)
                Facet[f].Indices[n] += f * Facet[0].NumVertex();
        }
        genVertices();
        genNormals();
    }
    ~HEALPix()
    {
        delete [] Vertices;
        delete [] Indices;
        delete [] Normals;
        delete [] Facet;
    }

    // TODO: Fix for other H, K
    inline float facet_xwidth_2()   {   return M_PI / H;                                    }
    inline float facet_yheight_2()  {   return M_PI / (K+1);                                }
    inline float facet_xwidth()     {   return 2 * facet_xwidth_2();                        }
    inline float facet_yheight()    {   return 2 * facet_yheight_2();                       }
    inline float x_c(int h, int k)  {   return h * facet_xwidth() + k * facet_xwidth_2();   }
    inline float y_c(int k)         {   return (k - (K-1)/2.0f) * facet_yheight_2();        }
    inline float y_x()              {   return M_PI_2 * (K-1) / H;                          }
    inline float sigma(float y)     {   return (K+1)/2.0f - fabs(y*H)/M_PI;                 }

    void genVertices()
    {
        for(int f=0; f<HK; f++) // loop over facets
            for(int n=0; n<Facet[f].NumVertex(); n++) // loop over all vertices in each facet
            {
                int h = f%H, k = f/H;

                // Generate coordinates relative to unit square centered at origin
                float2 xy = Facet[f].IJ(n)/(float)Dim - float2(0.5);

                // Rotate and scale square such that the width and height is one after rotation
                xy = mat2({-1, 1, -1, -1}) / 2.0f * xy;

                // Scale square
                xy *= float2(facet_xwidth(), facet_yheight());

                // Displace square
                xy += float2(x_c(h, k), y_c(k));

                // Convert to spherical coordinates
                float x = xy.x, y = xy.y;
                float phi       = fabs(y) <= y_x() ?   x : x_c(h, k) + (x-x_c(h, k)) / sigma(y);
                float sintht    = fabs(y) <= y_x() ?   y * H / M_PI_2 / K : (y > 0 ? 1:-1) * ( 1 - sigma(y)*sigma(y)/K );
                float costht    = sqrt(1 - sintht*sintht);

                // Assign vertex coordinates
                Facet[f].Vertices[n] = Origin + Radius * float3(costht * cos(phi), costht * sin(phi), sintht);

                // For testing
                //Facet[f].Vertices[n] = float3(-2, 0, 0, 0) + float3(x, y, -3, 1);
                //Facet[f].Vertices[n] = float3(-2, 0, 0, 0) + float3(phi, asin(sintht), -3, 1);
            }

        // For testing
        /*
        for(int n=0; n<NumVertex(); n++)
        {
            Vertices[n]            += Origin;
            glm::mat4 rotation = glm::rotate((float) M_PI_2/2.f*4.f, float3(0,1,0));
            //Vertices[n] = rotation * Vertices[n];
        }*/
        /*for(int f=0; f<HK; f++)
            for(int n=0; n<Facet[f].NumVertex(); n++)
                if ( f != 7 ) Facet[f].Vertices[n] = float3(0);*/

    }

    void genNormals()
    {
        for(int n=0; n<NumVertex(); n++)
            Normals[n] = Vertices[n] - Origin;
    }

    void genTerrain(float2* uv)
    {
        //int rr = (rand()%100)/10;

        static noise::module::Perlin terrainModule;
        terrainModule.SetFrequency (0.9);
        terrainModule.SetPersistence (0.6);

        for(int i=0; i<NumVertex(); i++)
        {
            float3 xyz = Vertices[i];
            uv[i].x = terrainModule.GetValue((xyz.x+1.52)*0.50, (xyz.y+1.52)*0.50, xyz.z*0.50) *0.25 + 0.35;
            uv[i].y = uv[i].x;
        }
    }

    void genTextureUV(float2* uv)
    {
        float u0;
        for (int f=0; f<H*K; f++)
            for (int i=0; i<Facet[f].NumVertex(); i++)
            {
                float3 xyz = -Facet[f].Vertices[i];
                float tht = asin(xyz.y/Radius), phi = -atan2(xyz.z, xyz.x);
                uv[f * Facet[0].NumVertex() + i].x = phi / 2 / M_PI + 0.5;
                uv[f * Facet[0].NumVertex() + i].y = 0.5 + sin(tht) / 2; //0.5 + tht / M_PI;
                if (i == 0) u0 = uv[i].x;
                if (uv[i].x - u0 > 0.5) uv[i].x -= 1;
                else if (uv[i].x - u0 < -0.5) uv[i].x += 1;
            }
    }
};

#endif // HEALPIX_H_INCLUDED
