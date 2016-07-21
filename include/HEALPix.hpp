#ifndef HEALPIX_H_INCLUDED
#define HEALPIX_H_INCLUDED

#include "Quad.hpp"

template <int Dim, int H = 4, int K = 3>
struct HEALPix
{
    // Data arrays
    float4*         Vertices;
    int*            Indices;
    float4*         Normals;
    inline int NumVertex()      { return HK * Facet[0].NumVertex(); }
    inline int NumIndex()       { return HK * Facet[0].NumIndex(); }

    // Facets
    Quad<Dim>*      Facet;
    const int       HK          = H*K;

    // Parameters
    float           Radius;
    float4          Origin;

    HEALPix(float Radius = 1.f, float4 Origin = float4(0), float ScaleP = 1.f, float ScaleQ = 1.f) : Radius(Radius), Origin(Origin)
    {
        Facet = new Quad<Dim>[HK];

        Vertices    = new float4    [Facet[0].NumVertex()*HK];
        Indices     = new int       [Facet[0].NumIndex()*HK];
        Normals     = new float4    [Facet[0].NumVertex()*HK];

        for (int f=0; f<HK; f++)
        {
            Facet[f].Vertices   = Vertices + f * Facet[0].NumVertex();
            Facet[f].Indices    = Indices + f * Facet[0].NumIndex();
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
                Facet[f].Vertices[n] = Origin + Radius * float4(costht * cos(phi), costht * sin(phi), sintht, 1);

                // For testing
                //Facet[f].Vertices[n] = float4(-2, 0, 0, 0) + float4(x, y, -3, 1);
                //Facet[f].Vertices[n] = float4(-2, 0, 0, 0) + float4(phi, asin(sintht), -3, 1);
            }

        // For testing
        /*
        for(int n=0; n<NumVertex(); n++)
        {
            Vertices[n]            += Origin;
            glm::mat4 rotation = glm::rotate((float) M_PI_2/2.f*4.f, float3(0,1,0));
            //Vertices[n] = rotation * Vertices[n];
        }
        for(int f=0; f<HK; f++)
            for(int n=0; n<Facet[f].NumVertex(); n++)
                if ( f/H != 0 ) Facet[f].Vertices[n] = float4(0);
        */
    }

    void genNormals()
    {
        for(int n=0; n<NumVertex(); n++)
            Normals[n] = Vertices[n] - Origin;
    }
};

#endif // HEALPIX_H_INCLUDED
