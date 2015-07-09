#ifndef STDAFX_H
#define STDAFX_H

#ifdef _DEBUG
#define ASSERT_METHOD(left, compare_op, right) assert((left) compare_op (right))
#else
#define ASSERT_METHOD(left, compare_op, right) left
#endif

#define _USE_MATH_DEFINES

#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "GL/glew.h"

/* Define this to tell GLFW to include glu.h */
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"

#include "libjpeg-turbo/turbojpeg.h"
#include "png/png.h"

#endif