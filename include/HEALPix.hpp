#ifndef HEALPIX_H_INCLUDED
#define HEALPIX_H_INCLUDED

#include "Quad.hpp"
#include "Sphere.hpp"
#include <noise/noise.h>

struct Pixel
{
    int f;
    int2 ij;
};

template <int Dim, int H = 4, int K = 3>
struct HEALPix : public Sphere
{
    // Data arrays
    floatx*         Vertices;
    unsigned int*   Indices;
    floatx*         Normals;
    inline int NumVertex()      { return HK * Facet[0].NumVertex(); }
    inline int NumIndex()       { return HK * Facet[0].NumIndex(); }

    // Facets
    Quad<Dim>*      Facet;
    const int       HK          = H*K;

    HEALPix(float radius = 1.f, floatx origin = floatx(0)) // float ScaleP = 1.f, float ScaleQ = 1.f
    {
        Radius = radius;
        Origin = origin;
        Facet = new Quad<Dim>[HK];

        Vertices    = new floatx        [Facet[0].NumVertex()*HK];
        Indices     = new unsigned int  [Facet[0].NumIndex()*HK];
        Normals     = new floatx        [Facet[0].NumVertex()*HK];

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
    inline float2 facet_xydim()     {   return float2(facet_xwidth(), facet_yheight());     }
    inline float x_c(int h, int k)  {   return h * facet_xwidth() + k * facet_xwidth_2();   }
    inline float y_c(int k)         {   return (k - (K-1)/2.0f) * facet_yheight_2();        }
    inline float2 xy_c(int f)       {   return float2(x_c(f%H,f/H), y_c(f/H));              }
    inline float y_x()              {   return M_PI_2 * (K-1) / H;                          }
    inline float sigma(float y)     {   return (K+1)/2.0f - fabs(y*H)/M_PI;                 }

    float xy_c(float2 xy)
    {
        float2 xyc = xy / facet_xydim();
        xyc = mat2({-1, -1, 1, -1}) * xyc;
        // TODO
    }

    float2 XY(Pixel p)
    {
        // Generate coordinates relative to unit square centered at origin
        float2 xy = (float2)p.ij/(float)Dim - float2(0.5);

        // Displace square
        xy += mat2({-1, -1, 1, -1}) * (xy_c(p.f) / facet_xydim());

        // Rotate and scale square such that the width and height is one after rotation
        xy = mat2({-1, 1, -1, -1}) / 2.0f * xy;

        // Scale square
        xy *= facet_xydim();

        return xy;
    }

    float2 PhiSintht(float2 xy, int f)
    {
        int h = f%H, k = f/H;
        float x = xy.x, y = xy.y;
        float phi       = fabs(y) <= y_x() ?   x : x_c(h, k) + (x-x_c(h, k)) / sigma(y);
        float sintht    = fabs(y) <= y_x() ?   y * H / M_PI_2 / K : (y > 0 ? 1:-1) * ( 1 - sigma(y)*sigma(y)/K );
        return float2(phi, sintht);
    }

    void genVertices()
    {
        for(int f=0; f<HK; f++) // loop over facets
            for(int n=0; n<Facet[f].NumVertex(); n++) // loop over all vertices in each facet
            {
                Pixel p = {f, Facet[f].IJ(n)};
                float2 xy = XY(p);

                // Convert to spherical coordinates
                float2 phiSintht = PhiSintht(xy, f);
                float phi       = phiSintht.x;
                float sintht    = phiSintht.y;
                float costht    = sqrt(1 - sintht*sintht);

                // Assign vertex coordinates
                Facet[f].Vertices[n] = Origin + Radius * floatx(costht * cos(phi), costht * sin(phi), sintht);

                //if (phi > M_PI) phi -= M_PI;
                //if (phi < -M_PI) phi += M_PI;

                // For testing
                //Facet[f].Vertices[n] = Origin + floatx(x, y, 0);
                Facet[f].Vertices[n] = Origin + floatx(phi, asin(sintht), 0);
            }

        // For testing
        /*
        for(int n=0; n<NumVertex(); n++)
        {
            Vertices[n]            += Origin;
            glm::mat4 rotation = glm::rotate((float) M_PI_2/2.f*4.f, floatx(0,1,0));
            //Vertices[n] = rotation * Vertices[n];
        }*/
        /*for(int f=0; f<HK; f++)
            for(int n=0; n<Facet[f].NumVertex(); n++)
                if ( f != 7 ) Facet[f].Vertices[n] = floatx(0);*/
    }

    /* For recovering pixel index

    inline float tht_x()              {   return asin((float)(K-1) / K);                          }
    inline float sigma_t(float tht)     {   return sqrt(K * (1 - abs(sin(tht))));                 }
    float tht_c;
    float phi_c;

    float2 XY(float tht, float phi)
    {
        float x, y;
        if (abs(tht) < tht_x())
        {
            x = phi;
            y = M_PI_2 * K / H * sin(tht);
        }
        else
        {
            //float phi_c = 0;
    		for(int h=-H; h<H; h++)
    		{
    			float c = h * 2 * M_PI / H ;
    			if (abs(c - phi) < abs(phi_c - phi)) phi_c = c;
    		}
            for(int k=-K; k<K; k++)
    		{
    			float c = k * M_PI / (K+1) ;
    			if (abs(c - tht) < abs(tht_c - tht)) tht_c = c;
    		}

    		x = phi_c + (phi - phi_c) * sigma_t(tht);
    		if (x < -M_PI) x += M_PI;
    		if (x > M_PI) x -= M_PI;

            y = (tht > 0 ? 1:-1) * M_PI / H * ( (K+1)/2.0 - sigma_t(tht) );
        }
        return float2(x, y);
    }

    float2 IJ(float2 xy)
    {
        float2 ij = xy;

        ij -= float2(phi_c, tht_c);

        // Scale square
        ij /= float2(facet_xwidth(), facet_yheight());

        // Rotate and scale square such that the width and height is one after rotation
        ij = mat2({-1, -1, 1, -1}) * ij;

        // Generate coordinates relative to unit square centered at origin
        return (ij + float2(0.5)) ; //* (float)Dim ;
    }

    */

    void genNormals()
    {
        for(int n=0; n<NumVertex(); n++)
            Normals[n] = normalize(Vertices[n] - Origin);
    }

    void genTerrain(float2* uv)
    {
        //int rr = (rand()%100)/10;

        static noise::module::Perlin terrainModule;
        terrainModule.SetFrequency (0.9);
        terrainModule.SetPersistence (0.6);

        for(int i=0; i<NumVertex(); i++)
        {
            floatx xyz = Vertices[i];
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
                floatx xyz = -Facet[f].Vertices[i];
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
