//#include "Terrain.hpp"
#include <noise/noise.h>

void genVertexUV()
{
    //int rr = (rand()%100)/10;

    static noise::module::Perlin myModule;
    myModule.SetFrequency (0.9);
    myModule.SetPersistence (0.6);

    /*for(int n=0; n<NumVertex(); n++)
    {
        quat xyz = quat(VertexXYZ[n*3], VertexXYZ[n*3+1], VertexXYZ[n*3+2]);
        VertexUV[n*2] = myModule.GetValue((xyz.x+1.52)*0.50, (xyz.y+1.52)*0.50, xyz.z*0.50)*0.25 + 0.57;
        VertexUV[n*2+1] = VertexUV[n*2];
    }*/
}
