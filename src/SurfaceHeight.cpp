#include "SurfaceHeight.h"
#include <iostream>
#include <vector>
#include "glm/gtx/intersect.hpp"

using namespace glm;
static int expected_dim = 512;

struct triangle
{
	vec3 p1, p2, p3;
	vec3 unitNormal()
	{
		return normalize(glm::cross(normalize(p2 - p1), normalize(p3 - p2)));
	}
};

float surfaceHeight(const char* data, vec2 start, vec2 end, int dim = expected_dim)
{
	if ((start.x > dim) | (start.y > dim) | (end.x > dim) | (end.y > dim))
	{
		std::cout << "points out of bounds" << std::endl;
		return 0.0f;
	}

	float dist = 0;
	vec2 cp1 = start; // current point 1
	vec2 cp2;
	triangle tri;

	while (abs(distance(end, cp1) > .001))
	{
		// get the current triangle;
		int x = (int)cp1.x;
		int y = (int)cp1.y;
		if (cp1.x - (float)x + cp1.y - (float)y < 2.0f)
		{
			tri.p1 = vec3(x, y, getHeight(data, x, y, dim));
			tri.p2 = vec3(x + 1, y, getHeight(data, x + 1, y, dim));
			tri.p3 = vec3(x, y + 1, getHeight(data, x, y + 1, dim));
		}
		else
		{
			tri.p1 = vec3(x + 1, y + 1, getHeight(data, x + 1, y + 1, dim));
			tri.p2 = vec3(x, y + 1, getHeight(data, x, y + 1, dim));
			tri.p3 = vec3(x + 1, y, getHeight(data, x + 1, y, dim));
		}

		float t1 = lineIntersection(cp1, end, (vec2)tri.p1, (vec2)tri.p2);
		float t2 = lineIntersection(cp1, end, (vec2)tri.p1, (vec2)tri.p3);
		float t3 = lineIntersection(cp1, end, (vec2)tri.p2, (vec2)tri.p3);
		float t = min(min(t1, t2), t3);
		if (t == INFINITY)
			cp2 = end;
		else
			cp2 = cp1 + t * (end - cp1);

		vec3 un = tri.unitNormal();
		vec3 mp1 = vec3(cp1, 0);
		mp1.z = distToPlane(un, tri.p1, cp1);
		vec3 mp2 = vec3(cp2, 0);
		mp2.z = distToPlane(un, tri.p1, cp2);
		dist += glm::distance(mp1, mp2);

		if (cp2 == (vec2)tri.p1)
		{
			tri.p2.x -= 2;
			tri.p3.y -= 2;
			// todo: reverse state?
		}
		else if (cp2 == (vec2)tri.p3)
		{
			tri.p2 += vec3(-2, 2, 0);
			tri.p1.y += 2;
		}
		else if (cp2 == (vec2)tri.p2)
		{
			tri.p3 += vec3(2, -2, 0);
			tri.p1.x += 2;
		}
		else if (cp2.x == tri.p1.x)
		{
			tri.p1.y += 1;
			tri.p3.y -= 1;
			tri.p2 += vec3(-2, 1, 0);
		}
		else if (cp2.y == tri.p1.y)
		{
			tri.p1.x += 1;
			tri.p2.x -= 1;
			tri.p3 += vec3(1, -2, 0);
		}
		
	return dist;
	}

	// this is as far as I got, but I know how to finish:
	// calculate the next point by using parametric line intersection
	// handle special cases (slope is 0 or inf)
	// handle cases where slope is 0 or inf and the start point is on a vertex in geometry
	// finish the loop by setting cp to the next critical point in the geometry, and checking 
	// whether we're close to p2

	// maybe there's an easier way to do this by doing one big plane intersection on the surface,
	// but I think that would require an actual mesh. This method makes the mesh as I go.

}

float getHeight(const char* data, int x, int y, int dim = expected_dim)
{
	return data[y * dim + x];
}

float distToPlane(vec3 norm, vec3 p1, vec2 p)
{
	// a (x - x0) + b (y - y0) c (z - z0) = 0
	// (a (x - x0) + b (y - y0)) / c - z0 = z
	return -(norm.x * (p.x - p1.x) + norm.y * (p.y - p1.y)) / norm.z + p1.z;
}

float lineIntersection(vec2 p1, vec2 d1, vec2 p2, vec2 d2)
{
	float ang = glm::dot(normalize(d1 - p1), normalize(d2 - p2));
	if (ang == 1)
		return INFINITY; // parralell case
	
	float num = (p1.x - p2.x) * (p2.y - d1.y) - (p1.y - p2.y) * (p2.x - d2.x);
	float den = (p1.x - d1.x) * (p2.y - d2.y) - (p1.y - d1.y) * (p2.x - d2.x);
	float t = num / den;
	if (t <= 0)
		return INFINITY; // on or in front of the line
	return t; // return the parameter for the first line
}