#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <list>
#include <map>

#include <spdlog/spdlog.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/noise.hpp>

/////////////////////////
typedef uint8_t   UInt8;
typedef uint16_t  UInt16;
typedef uint32_t  UInt32;
typedef uint64_t  UInt64;
typedef int8_t    SInt8;
typedef int16_t   SInt16;
typedef int32_t   SInt32;
typedef int64_t   SInt64;
typedef float     Float32;
typedef double    Float64;
/////////////////////////

#ifndef VAU
  #define VAU (6,2831853071795864)
#endif

#ifndef PI
  #define PI (3.1415926535897932)
#endif

#endif