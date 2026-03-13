#pragma once
#include <vsg/core/ref_ptr.h>

using vsg::ref_ptr;
using vsg::ushortArray;
using vsg::vec2Array;
using vsg::vec3Array;
using vsg::vec4Array;

#define DELTA_TIME(now, last) std::chrono::duration<double, std::chrono::milliseconds::period>(now - last).count()
