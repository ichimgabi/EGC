#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace obiect
{

    // Create rectangle with given bottom left corner, length and color
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color);

    // Create unfilled rectangle with given bottom left corner, length and color
    Mesh* CreateUnfilledRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color);

    // Create circle with given radius and color
    Mesh* CreateCircle(const std::string& name, float radius, glm::vec3 color);

    // Create triangle with given left corner, length and color
    Mesh* CreateTriangle(const std::string& name, glm::vec3 top, float length, glm::vec3 color);
}
