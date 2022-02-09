#include "SurfaceHeight.h"
#include <iostream>
#include <vector>
#include "glm/gtx/intersect.hpp"

using namespace glm;
static int ht = 512;

struct triangle
{
	vec3 p1, p2, p3;
	vec3 unitNormal()
	{
		return glm::normalize(glm::cross(p1 - p2, p3 - p2));
	}
};

float surfaceHeight(const char* data, vec2 p1, vec2 p2)
{
	if ((p1.x > ht) | (p1.y > ht) | (p2.x > ht) | (p2.y > ht))
	{
		std::cout << "points out of bounds" << std::endl;
		return 0.0f;
	}

	float dist;

	vec2 cp = p1;
	vec3 cp3 = vec3(p1, 0.0f);
	triangle t;

	// get the current triangle;
	int x = (int)cp.x;
	int y = (int)cp.y;
	if (cp.x - (float)x + cp.y - (float)y < 2.0f)
	{
		t.p1 = vec3(x, y, getHeight(data, x, y));
		t.p2 = vec3(x + 1, y, getHeight(data, x + 1, y));
		t.p3 = vec3(x, y + 1, getHeight(data, x, y + 1));
	}
	else
	{
		t.p1 = vec3(x + 1, y + 1, getHeight(data, x + 1, y + 1));
		t.p2 = vec3(x, y + 1, getHeight(data, x, y + 1));
		t.p2 = vec3(x + 1, y, getHeight(data, x + 1, y));
	}
	
	cp3.z = glm::dot(t.unitNormal(), t.p1 - cp3);

	// this is as far as I got, but I know how to finish:
	// calculate the next point by using parametric line intersection
	// handle special cases (slope is 0 or inf)
	// handle cases where slope is 0 or inf and the start point is on a vertex in geometry
	// finish the loop by setting cp to the next critical point in the geometry, and checking 
	// whether we're close to p2

	// maybe there's an easier way to do this by doing one big plane intersection on the surface,
	// but I think that would require an actual mesh. This method makes the mesh as I go.

}

float getHeight(const char* data, int x, int y)
{
	return data[y * ht + x];
}

vec2 lineIntersection(vec2 p1, vec2 d1, vec2 p2, vec2 d2)
{
	if (glm::dot(d1 - p1, d2 - p2) == 0)
		return vec2(99999999, 99999999); // (infinity)
	// x1 = p1 + x1' * d1
	// x2 = p2 + x2' * d2
	// x1' = (p2 + x2' * d2 - p1) / d1
}