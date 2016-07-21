#version 120

#define PI 3.1415926535897932384626433832795

//uniform vec4 Rotor;
uniform mat4 MVP;
attribute vec3 VertexXYZ;
attribute vec2 VertexUV;

varying vec2 UV;

vec4 conjugate(vec4 Q)
{
    return vec4(-Q.x, -Q.y, -Q.z, Q.w);
}

vec4 product(vec4 L, vec4 R)
{
    float w = L.w*R.w - L.x*R.x - L.y*R.y - L.z*R.z;
    float x = L.w*R.x + L.x*R.w + L.y*R.z - L.z*R.y;
    float y = L.w*R.y + L.y*R.w + L.z*R.x - L.x*R.z;
    float z = L.w*R.z + L.z*R.w + L.x*R.y - L.y*R.x;
    return vec4(x, y, z, w);
}

void main()
{
    //gl_PointSize = 1.0;

	//rotated = Rotor * VertexXYZ * ~Rotor
    //vec4 rotated = product(product(Rotor,vec4(VertexXYZ, 0)),conjugate(Rotor));
    //vec4 rotated = vec4(VertexXYZ, 0);
    //gl_Position = vec4(0.3*rotated.x*768/1024, 0.3*rotated.y, 0.3*rotated.z, 1);
    gl_Position = MVP * vec4(VertexXYZ, 1);
	UV = VertexUV;
}
