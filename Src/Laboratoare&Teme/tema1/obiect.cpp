#include "obiect.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "obiect.h"


Mesh* obiect::CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    indices.push_back(0);
    indices.push_back(2);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* obiect::CreateUnfilledRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    square->SetDrawMode(GL_LINE_LOOP);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* obiect::CreateCircle(const std::string& name, float radius, glm::vec3 color)
{
    double i;
    int triangleAmount = 20;

    glm::vec3 center = glm::vec3(0, 0, 0);

    std::vector<VertexFormat> vertices;

    for (i = 0; i <= triangleAmount; i++)
        vertices.push_back(VertexFormat(center + glm::vec3((radius * cos(i * 2.0 * 3.141 / triangleAmount)), (radius * sin(i * 2.0 * 3.141 / triangleAmount)), 0), color));

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for (i = 0; i < triangleAmount; i++)
        indices.push_back(i);

    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;

}

Mesh* obiect::CreateTriangle(
    const std::string& name,
    glm::vec3 top,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = top;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(-length / 2, 1.5 * length, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, 1.5 * length, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    triangle ->SetDrawMode(GL_TRIANGLES);
    triangle->InitFromData(vertices, indices);
    return triangle;
}
