#pragma once
#include "glm/glm.hpp"

float surfaceHeight(const char* data, glm::vec2 p1, glm::vec2 p2, int dim);

float distToPlane(glm::vec3 norm, glm::vec3 p1, glm::vec2 p);

float getHeight(const char* data, int x, int y, int dim);

float lineIntersection(glm::vec2 p1, glm::vec2 d1, glm::vec2 p2, glm::vec2 d2);