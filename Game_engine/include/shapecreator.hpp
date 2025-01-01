#ifndef shapecreator_hpp
#define shapecreator_hpp
#include<unordered_map>
#include<glm/glm.hpp>
namespace graf
{
    class VertexArrayObject;

    enum class ShapeTypes
    {
        Circle,
        Square,
        Cube,
        Frustum,
        Pyramid,
        Cylinder
    };

    class ShapeCreator
    {
    public:
        static VertexArrayObject* create(ShapeTypes shape);
        static unsigned int getSizeOfShapeTypes();
        static void deleteInstance();
    private:
        ShapeCreator();
        static ShapeCreator* m_Ins ;
        static ShapeCreator* getInstance();
        ~ShapeCreator();


        VertexArrayObject* createSquare();
        VertexArrayObject* createCircle(int angleInDegrees);
        VertexArrayObject* createCube();  
        VertexArrayObject* createFrustum();  
        VertexArrayObject* createPyramid();  
        VertexArrayObject* createCylinder();  


        unsigned int m_sizeOfShapeTypes;
        std::unordered_map<ShapeTypes,VertexArrayObject*> m_vaoMap;
    };

}


#endif