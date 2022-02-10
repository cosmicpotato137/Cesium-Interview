#include "read_data.h"
#include <string>
#include <iostream>
#include <iostream>
#include "SurfaceHeight.h"

using namespace glm;

int main()
{
	//char* data = readData(rootdir + "data/post.data");

	char data[4] = { 1, 1, 1, 1 };

	float f = lineIntersection(vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0));

	float d = surfaceHeight(data, vec2(0, 0), vec2(1, 1), 2);

	std::cout << d << std::endl;
}