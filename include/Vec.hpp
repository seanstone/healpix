#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
    #define M_PI_2 (M_PI/2.0)
    #define M_PI_4 (M_PI/4.0)
#endif

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

#endif // VEC_H_INCLUDED
