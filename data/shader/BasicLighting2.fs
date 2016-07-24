#version 330 core
out vec4 color;

in vec3 vertexCoord;
in vec3 normalVec;
in vec2 textureCoord;

//in int clickN;

uniform sampler2D surfaceTexture;

struct PointLight
{
	vec3 position;
	vec3 radiant;
};

uniform PointLight pointLight;

const float M_PI = 3.14159265358979;
const float M_PI_2 = M_PI/2.0;

bool onLine = false;

bool latLine(float lat)
{
	float vlat = asin(vertexCoord.z/5.0f) * 180 / M_PI;
	if (abs (vlat - lat) < 0.1)
	{
		color = vec4(1,0,0,1);
		onLine = true;
		return true;
	}
	return false;
}

bool lonLine(float lon)
{
	float vlon = atan(vertexCoord.y, vertexCoord.x) * 180 / M_PI;
	if (abs (vlon - lon) < 0.1)
	{
		color = vec4(1,0,0,1);
		onLine = true;
		return true;
	}
	return false;
}

/* For recovering pixel index

int K = 3, H = 4;

float tht_x()              {   return asin((K-1) * 1.0f / K);                          }
float sigma_t(float tht)     {   return sqrt(K * (1 - abs(sin(tht))));                 }
float facet_xwidth_2()   {   return M_PI / H;                                    }
float facet_yheight_2()  {   return M_PI / (K+1);                                }
float facet_xwidth()     {   return 2 * facet_xwidth_2();                        }
float facet_yheight()    {   return 2 * facet_yheight_2();                       }
float x_c(int h, int k)  {   return h * facet_xwidth() + k * facet_xwidth_2();   }

vec2 pixel(float tht, float phi)
{
	float x, y;
	if (abs(tht) < tht_x())
	{
		x = phi;
		y = M_PI_2 * K / H * sin(tht);
	}
	else
	{
		float phi_c = 0;
		for(int h=-H; h<H; h++)
		{
			float c = h * 2 * M_PI / H ;
			if (abs(c - phi) < abs(phi_c - phi)) phi_c = c;
		}

		x = phi_c + (phi - phi_c) * sigma_t(tht);
		if (x < -M_PI) x += M_PI;
		if (x > M_PI) x -= M_PI;


		y = (tht > 0 ? 1:-1) * M_PI / H * ( (K+1)/2.0 - sigma_t(tht) );
	}
	return vec2(x, y) * 180 / M_PI;
}

bool yLine(float y)
{
	float tht = asin(vertexCoord.z/5.0f);
	float phi = atan(vertexCoord.y, vertexCoord.x);
	if (abs (pixel(tht,phi).y - y) < 0.1)
	//if (abs(tht) < tht_x())
	{
		color = vec4(1,0,0,1);
		onLine = true;
		return true;
	}
	return false;
}

bool xLine(float x)
{
	float tht = asin(vertexCoord.z/5.0f);
	float phi = atan(vertexCoord.y, vertexCoord.x);
	if (abs (pixel(tht,phi).x - x) < 0.1)
	{
		color = vec4(1,0,0,1);
		onLine = true;
		return true;
	}
	return false;
}

*/

void main()
{
	vec3 diff = pointLight.position-vertexCoord;
	vec3 directionVec = normalize(diff);
	float dist = length(diff);
	vec3 lightStrengthOnFragment = pointLight.radiant*(dot(normalVec,directionVec)/(dist*dist));

	vec4 textureColor = texture(surfaceTexture,textureCoord);

	if(dot(textureColor.xyz,vec3(1)) == 0)//If no texture is applied
		textureColor = vec4(1,1,1,1);

	//Apply lighting
	//lightStrengthOnFragment += vec3(0.5);
	vec3 renderedColor = textureColor.xyz * lightStrengthOnFragment;

	//Gamma correction
	const float gamma = 2.2f;
	const float inverseGamma = 1.0f/gamma;
	renderedColor = max(renderedColor,vec3(0,0,0));
	vec3 correctedColor = pow(renderedColor,vec3(inverseGamma));

	/*for (int lat = 0; lat<=90; lat+=10)
		latLine(lat);
	for (int lat = -180; lat<=180; lat+=10)
		lonLine(lat);*/

	/*for (int y = 0; y<=90; y+=10)
		yLine(y);
	for (int x = -180; x<=180; x+=5)
		xLine(x);*/

	if(!onLine) color = vec4(correctedColor,1);
		//color = vec4(renderedColor,1);
}
