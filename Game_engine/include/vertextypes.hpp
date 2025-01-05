#ifndef vertextypes_hpp
#define vertextypes_hpp
#include<glm/glm.hpp>
#include<vector>
namespace graf
{
    class Vertex
    {
    public:
        glm::vec3 position;
        glm::vec2 texture;
        glm::vec3 normal;
    };

    using VertexList = std::vector<Vertex>;

    using IndexList = std::vector<unsigned int>;
}



#endif