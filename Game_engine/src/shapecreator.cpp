#include "shapecreator.hpp"
#include "vertexarrayobject.hpp"
#include "indexbuffer.hpp"
#include "vertexbuffer.hpp"
#include "vertextypes.hpp"
namespace graf
{
    float pi = 3.14159265358979323846f;

    ShapeCreator* ShapeCreator::m_Ins = nullptr;
    
    void findNormals(Vertex *vertices,int vertexCount, unsigned int *indices,int indexCount){
        for (size_t i = 0; i < indexCount; i++)
        {
            auto v0 = vertices[indices[i]].position;
            auto v1 = vertices[indices[i+1]].position;
            auto v2 = vertices[indices[i+2]].position;

            auto a = v2-v0;
            auto b = v1-v0;
            auto normal = glm::cross(a,b);
            vertices[indices[i]].normal   = -normal;
            vertices[indices[i+1]].normal = -normal;
            vertices[indices[i+2]].normal = -normal;
        }
    }
    void averageNormals(Vertex* vertices, int count){

        for (size_t i = 0; i < count; i++)
        {
            int normalCount=0;
            glm::vec3 averageNormal =glm::vec3(0);
            for (size_t j = 0; j < count; j++)
            {
                if(vertices[i].position == vertices[j].position)
                {
                    averageNormal += vertices[j].normal;
                    normalCount++;
                }
            }

            averageNormal = averageNormal / (float)normalCount;
            for (size_t j = 0; j < count; j++)
            {
                if(vertices[i].position == vertices[j].position)
                {
                    vertices[i].normal = averageNormal;
                }
            }
        }
    }

    ShapeCreator* ShapeCreator::getInstance()
    {
        if(m_Ins==nullptr)
        {
            m_Ins = new ShapeCreator();
        }

        return m_Ins;
    }
    VertexArrayObject* ShapeCreator::create(ShapeTypes shape)
    {
        switch (shape)
        {
        case ShapeTypes::Square:           return getInstance()->createSquare();
            
        case ShapeTypes::Circle:           return getInstance()->createCircle(10);
            
        case ShapeTypes::Cube:             return getInstance()->createCube();

        case ShapeTypes::Pyramid:           return getInstance()->createPyramid();
            
        case ShapeTypes::Frustum:           return getInstance()->createFrustum();

        case ShapeTypes::Cylinder:          return getInstance()->createCylinder();

        default:
            return nullptr;
            break;
        }
    }
    ShapeCreator::ShapeCreator(){
        m_sizeOfShapeTypes=5;
    }
    unsigned int ShapeCreator::getSizeOfShapeTypes(){
        return getInstance()->m_sizeOfShapeTypes; 
    }
    VertexArrayObject* ShapeCreator::createCircle(int anglesInDegrees)
    {

        if(m_vaoMap.count(ShapeTypes::Circle)>0)
            return m_vaoMap[ShapeTypes::Circle];

        graf::VertexArrayObject *   va = new VertexArrayObject();
        graf::VertexBuffer*         vb = new VertexBuffer();
        graf::IndexBuffer*          ib = new IndexBuffer();
       
        int vertexCount = 360/anglesInDegrees;
        int faceCount = vertexCount-2;

        VertexList vertices;
        IndexList indices;       
        
        for(int i=0;i<vertexCount;i++)
        {
            Vertex vertex;
            
            double currentAngle =anglesInDegrees*i;
            vertex.position.x= glm::cos(glm::radians(currentAngle));
            vertex.position.y = glm::sin(glm::radians(currentAngle));
            vertex.position.z = 0.0f;
            vertex.texture.s = 0.5+0.5*glm::cos(glm::radians(currentAngle));
            vertex.texture.t = 0.5+0.5*glm::sin(glm::radians(currentAngle));
            vertices.emplace_back(vertex);
           
        }
        for(int i=0;i<faceCount;i++)
        {
            indices.emplace_back(0);
            indices.emplace_back(i+2);
            indices.emplace_back(i+1);
        }
        va->create();
        va->bind();
        // vb ve ib'nin create işlemlerinin içerisine bind etme işemi yapmışız bu da otamatik oalrak onların aktif va'ya bağlanmasını sağlar
        vb->create(&vertices[0],sizeof(Vertex)*vertices.size());      
        ib->create(&indices[0],sizeof(unsigned int)*indices.size());
        va->setVertexBuffer(vb);
        va->setIndexBuffer(ib);
        va->addVertexAttribute(graf::VertexAttributeType::Position);
        va->addVertexAttribute(graf::VertexAttributeType::Texture);
        va->addVertexAttribute(VertexAttributeType::Normal);
        va->activateAttributes();

        m_vaoMap[ShapeTypes::Circle]= va;
        va->unbind();
        return va;
    }
    VertexArrayObject* ShapeCreator::createSquare( )
    {
        if(m_vaoMap.count(ShapeTypes::Square)>0)
            return m_vaoMap[ShapeTypes::Square];

        float vertices[] = {  
        /*   x     y     z     s    t */
        -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
         0.5f, 0.5f, 0.0f, 1.0f,1.0f,
         0.5f,-0.5f, 0.0f, 1.0f,0.0f,

         0.5f,-0.5f, 0.0f, 1.0f,0.0f,   
        -0.5f,-0.5f, 0.0f, 0.0f,0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f,1.0f       
        }; 

        unsigned int indices[]={
            0,1,2,3,4,5
        };
        VertexArrayObject *va = new VertexArrayObject;
        VertexBuffer *vb= new VertexBuffer;
        IndexBuffer *ib = new IndexBuffer;

        va->create();
        va->bind();
        // vb ve ib'nin create işlemlerinin içerisine bind etme işemi yapmışız bu da otamatik oalrak onların aktif va'ya bağlanmasını sağlar
        vb->create(vertices,sizeof(vertices));
        ib->create(indices,sizeof(indices));
        va->setVertexBuffer(vb);
        va->setIndexBuffer(ib);

        va->addVertexAttribute(VertexAttributeType::Position);
        va->addVertexAttribute(VertexAttributeType::Texture);
        va->activateAttributes();
        m_vaoMap[ShapeTypes::Square] =va;
        va->unbind();
        return va;
    }
    /*
        taban kenarları 1 uzunlugunda, kesik oldugu yerde ise 0.5 uzunlugunda olan bir frustum yapamya çalıştım.
        yukseklik kok3 bolu4 oldu. Biraz kucuk bir cisim oldu ama esit ve duzgun bir cisim icin böyle olması gerekiyordu.
    */
    VertexArrayObject* ShapeCreator::createFrustum()
    {
        if(m_vaoMap.count(ShapeTypes::Frustum)>0)
            return m_vaoMap[ShapeTypes::Frustum];

        float kok3_bolu8 = sqrt(3)/8;
        glm::vec3 positions[] = {  
            // ON 
            {-0.25f,  kok3_bolu8, 0.25f}, // sol ust  0
            { 0.25f,  kok3_bolu8, 0.25f}, // sag ust    1 
            {-0.5f,  -kok3_bolu8, 0.5f},  //  sol alt     2
            {0.5f,   -kok3_bolu8,  0.5f},  //  sag alt      3

                // ARKA
            {-0.25f,  kok3_bolu8, -0.25f}, //  sol ust  4 
            { 0.25f,  kok3_bolu8, -0.25f}, //  sag ust  5 
            {-0.5f, -kok3_bolu8, -0.5f},  //   sol alt   6 
            { 0.5f, -kok3_bolu8, -0.5f},  //   sag alt  7 
        }; 

        //tepe noktasına sadece 1 tane texture vermek istedim
        // diğer noktalara ise dilediğimiz şekilde texture verleim istedim
        // Eğerki tepe noktasına da her bir yüzey için texture verdirseydim 16 nokta kullanmam gerekirdi. Ben bu yolu seçtim.
        int vertexCount = 24;
        Vertex* vertices= new Vertex[vertexCount];
        std::vector<unsigned int> indices;

        //Ön Yüz
        vertices[0].position = positions[0];
        vertices[1].position = positions[2];
        vertices[2].position = positions[3];
        vertices[3].position = positions[1];

        vertices[0].texture = glm::vec2(0.25f,1.0f);
        vertices[1].texture = glm::vec2(0.0f,0.0f);
        vertices[2].texture = glm::vec2(1.0f,0.0f);
        vertices[3].texture = glm::vec2(0.75f,1.0f);

        //Sol Yüz
        vertices[4].position = positions[4];
        vertices[5].position = positions[6];
        vertices[6].position = positions[2];
        vertices[7].position = positions[0];
        
        vertices[4].texture = glm::vec2(0.25f,1.0f);
        vertices[5].texture = glm::vec2(0.0f,0.0f);
        vertices[6].texture = glm::vec2(1.0f,0.0f);
        vertices[7].texture = glm::vec2(0.75f,1.0f);


        //Arka Yüz
        vertices[8 ].position = positions[5];
        vertices[9 ].position = positions[7];
        vertices[10].position = positions[6];
        vertices[11].position = positions[4];
        
        vertices[8 ].texture = glm::vec2(0.25f,1.0f);
        vertices[9 ].texture = glm::vec2(0.0f,0.0f);
        vertices[10].texture = glm::vec2(1.0f,0.0f);
        vertices[11].texture = glm::vec2(0.75f,1.0f);


        //Sag Yüz
        vertices[12].position = positions[1];
        vertices[13].position = positions[3];
        vertices[14].position = positions[7];
        vertices[15].position = positions[5];

        vertices[12].texture = glm::vec2(0.25f,1.0f);
        vertices[13].texture = glm::vec2(0.0f,0.0f);
        vertices[14].texture = glm::vec2(1.0f,0.0f);
        vertices[15].texture = glm::vec2(0.75f,1.0f);


        //Tavan
        vertices[16].position = positions[4];
        vertices[17].position = positions[0];
        vertices[18].position = positions[1];
        vertices[19].position = positions[5];
        
        vertices[16].texture = glm::vec2(0.0f,1.0f);
        vertices[17].texture = glm::vec2(0.0f,0.0f);
        vertices[18].texture = glm::vec2(1.0f,0.0f);
        vertices[19].texture = glm::vec2(1.0f,1.0f);

        //Taban
        vertices[20].position = positions[2];
        vertices[21].position = positions[6];
        vertices[22].position = positions[7];
        vertices[23].position = positions[3];
        
        vertices[20].texture = glm::vec2(0.0f,1.0f);
        vertices[21].texture = glm::vec2(0.0f,0.0f);
        vertices[22].texture = glm::vec2(1.0f,0.0f);
        vertices[23].texture = glm::vec2(1.0f,1.0f);


        /*
        0 1 2
        0 2 3
        4 5 6 
        4 6 7
        */
        for(int i=0;i<6;i++)
        {
            indices.push_back(i*4);
            indices.push_back(i*4+1);
            indices.push_back(i*4+2);

            indices.push_back(i*4);
            indices.push_back(i*4+2);
            indices.push_back(i*4+3);            
        }


        graf::VertexArrayObject *   va = new VertexArrayObject();
        graf::VertexBuffer*         vb = new VertexBuffer();
        graf::IndexBuffer*          ib = new IndexBuffer();

        va->create();
        va->bind();
        // vb ve ib'nin create işlemlerinin içerisine bind etme işemi yapmışız bu da otamatik oalrak onların aktif va'ya bağlanmasını sağlar
        vb->create(vertices,sizeof(Vertex)*vertexCount);

        ib->create(indices.data(),indices.size()*sizeof(unsigned int));
        va->setVertexBuffer(vb);
        va->setIndexBuffer(ib);

        va->addVertexAttribute(VertexAttributeType::Position);
        va->addVertexAttribute(VertexAttributeType::Texture);
        va->addVertexAttribute(VertexAttributeType::Normal);
        va->activateAttributes();
        m_vaoMap[ShapeTypes::Frustum] =va;
        va->unbind();
        return va;
    }

    /*
        Kenarı bir birim aldım. Yukseklik kok3 bolu2 cıktı
        tepe noktasına 1 texture verilebilecek şekilde ayarladım farklı daolabilirdi elbette.
    */
    VertexArrayObject* ShapeCreator::createPyramid()
    {
        if(m_vaoMap.count(ShapeTypes::Pyramid)>0)
            return m_vaoMap[ShapeTypes::Pyramid];

        graf::VertexArrayObject *   va = new VertexArrayObject();
        graf::VertexBuffer*         vb = new VertexBuffer();
        graf::IndexBuffer*          ib = new IndexBuffer();


        float kok3_bolu2= sqrt(3)/2;
        glm::vec3 positions[] = {  
            {-0.0f,  kok3_bolu2*2/3, 0.0f}, // tepe 0
            { -0.5f,  -kok3_bolu2/3, 0.5f}, // sol yakın     1 
            { 0.5f, -kok3_bolu2/3, 0.5f},  // sag yakın      2
            {-0.5f, -kok3_bolu2/3, -0.5f},  // sol uzak      3
            { 0.5f, -kok3_bolu2/3, -0.5f},  // sag uzak      4
        }; 

        //tepe noktasına sadece 1 tane texture vermek istedim
        // diğer noktalara ise dilediğimiz şekilde texture verleim istedim
        // Eğerki tepe noktasına da her bir yüzey için texture verdirseydim 16 nokta kullanmam gerekirdi. Ben bu yolu seçtim.
        int vertexCount = 13;
        Vertex* vertices= new Vertex[vertexCount];
        std::vector<unsigned int> indices;

        //Ön Yüz
        vertices[0].position = positions[0];
        vertices[1].position = positions[1];
        vertices[2].position = positions[2];

        vertices[0].texture = glm::vec2(0.5f,1.0f);
        vertices[1].texture = glm::vec2(0.0f,0.0f);
        vertices[2].texture = glm::vec2(1.0f,0.0f);


        //Sol Yüz
        vertices[3].position = positions[3];
        vertices[4].position = positions[1];

        vertices[3].texture = glm::vec2(0.0f,0.0f);
        vertices[4].texture = glm::vec2(1.0f,0.0f);
        
        
        //Arka Yüz
        vertices[5].position = positions[4];
        vertices[6].position = positions[3];

        vertices[5].texture = glm::vec2(0.0f,0.0f);
        vertices[6].texture = glm::vec2(1.0f,0.0f);
        
        //Sag Yüz
        vertices[7].position = positions[2];
        vertices[8].position = positions[4];

        vertices[7].texture = glm::vec2(0.0f,0.0f);
        vertices[8].texture = glm::vec2(1.0f,0.0f);
        
        //Taban
        vertices[9].position = positions[1];
        vertices[10].position = positions[3];
        vertices[11].position = positions[4];
        vertices[12].position = positions[2];

        vertices[9].texture = glm::vec2(0.0f,1.0f);
        vertices[10].texture = glm::vec2(0.0f,0.0f);
        vertices[11].texture = glm::vec2(1.0f,0.0f);
        vertices[12].texture = glm::vec2(1.0f,1.0f);
        
        
        for (size_t i = 0; i < 4; i++)
        {
            indices.push_back(0);
            indices.push_back(i*2+1);
            indices.push_back(i*2+2);
        }
        
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(11);

        indices.push_back(9);
        indices.push_back(11);
        indices.push_back(12);
        
        

        va->create();
        va->bind();
        // vb ve ib'nin create işlemlerinin içerisine bind etme işemi yapmışız bu da otamatik oalrak onların aktif va'ya bağlanmasını sağlar
        vb->create(vertices,sizeof(Vertex)*vertexCount);

        ib->create(indices.data(),indices.size()*sizeof(unsigned int));
        va->setVertexBuffer(vb);
        va->setIndexBuffer(ib);

        va->addVertexAttribute(VertexAttributeType::Position);
        va->addVertexAttribute(VertexAttributeType::Texture);
        va->addVertexAttribute(VertexAttributeType::Normal);
        va->activateAttributes();
        m_vaoMap[ShapeTypes::Pyramid] =va;
        va->unbind();
        return va;

    }
    VertexArrayObject* ShapeCreator::createCube()
    {
        if(m_vaoMap.count(ShapeTypes::Cube)>0)
            return m_vaoMap[ShapeTypes::Cube];

        glm::vec3 positions[] = {  
            {-0.5f,  0.5f, 0.5f},   ///0
            { 0.5f,  0.5f, 0.5f},
            { 0.5f, -0.5f, 0.5f},
            {-0.5f, -0.5f, 0.5f},

            {-0.5f, 0.5f, -0.5f},
            {0.5f,  0.5f,  -0.5f},
            {0.5f, -0.5f, -0.5f},
            {-0.5f,-0.5f,-0.5f}        
        }; 


        int vertexCount = 24;
        Vertex* vertices= new Vertex[vertexCount];
        std::vector<unsigned int> indices;


        //Ön Yüz
        vertices[0].position = positions[0];
        vertices[1].position = positions[1];
        vertices[2].position = positions[2];
        vertices[3].position = positions[3];

        vertices[0].normal = glm::vec3(0.0,0.0,1.0f);
        vertices[1].normal = glm::vec3(0.0,0.0,1.0f);
        vertices[2].normal = glm::vec3(0.0,0.0,1.0f);
        vertices[3].normal = glm::vec3(0.0,0.0,1.0f);

        vertices[0].texture = glm::vec2(0.0f,1.0f);
        vertices[1].texture = glm::vec2(1.0f,1.0f);
        vertices[2].texture = glm::vec2(1.0f,0.0f);
        vertices[3].texture = glm::vec2(0.0f,0.0f);


        //Sağ Yüz
        vertices[4].position = positions[1];
        vertices[5].position = positions[5];
        vertices[6].position = positions[6];
        vertices[7].position = positions[2];

        vertices[4].normal = glm::vec3(1.0f,0.0f,0.0f);
        vertices[5].normal = glm::vec3(1.0f,0.0f,0.0f);
        vertices[6].normal = glm::vec3(1.0f,0.0f,0.0f);
        vertices[7].normal = glm::vec3(1.0f,0.0f,0.0f);

        
        vertices[4].texture = glm::vec2(0.0f,1.0f);
        vertices[5].texture = glm::vec2(1.0f,1.0f);
        vertices[6].texture = glm::vec2(1.0f,0.0f);
        vertices[7].texture = glm::vec2(0.0f,0.0f);



        //Üst Yüz
        vertices[8].position = positions[4];
        vertices[9].position = positions[5];
        vertices[10].position = positions[1];
        vertices[11].position = positions[0];


        vertices[8].normal = glm::vec3(0.0f,1.0f,0.0f);
        vertices[9].normal = glm::vec3(0.0f,1.0f,0.0f);
        vertices[10].normal = glm::vec3(0.0f,1.0f,0.0f);
        vertices[11].normal = glm::vec3(0.0f,1.0f,0.0f);


        vertices[8].texture = glm::vec2(0.0f,1.0f);
        vertices[9].texture = glm::vec2(1.0f,1.0f);
        vertices[10].texture = glm::vec2(1.0f,0.0f);
        vertices[11].texture = glm::vec2(0.0f,0.0f);

        //Sol Yüz
        vertices[12].position = positions[4];
        vertices[13].position = positions[0];
        vertices[14].position = positions[3];
        vertices[15].position = positions[7];

        vertices[12].normal =  glm::vec3(-1.0f,0.0f,0.0f);
        vertices[13].normal =  glm::vec3(-1.0f,0.0f,0.0f);
        vertices[14].normal =  glm::vec3(-1.0f,0.0f,0.0f);
        vertices[15].normal =  glm::vec3(-1.0f,0.0f,0.0f);

        vertices[12].texture = glm::vec2(0.0f,1.0f);
        vertices[13].texture = glm::vec2(1.0f,1.0f);
        vertices[14].texture = glm::vec2(1.0f,0.0f);
        vertices[15].texture = glm::vec2(0.0f,0.0f);

        //Arka Yüz
        vertices[16].position = positions[5];
        vertices[17].position = positions[4];
        vertices[18].position = positions[7];
        vertices[19].position = positions[6];

        vertices[16].normal = glm::vec3(0.0f,0.0f,-1.0f);
        vertices[17].normal = glm::vec3(0.0f,0.0f,-1.0f);
        vertices[18].normal = glm::vec3(0.0f,0.0f,-1.0f);
        vertices[19].normal = glm::vec3(0.0f,0.0f,-1.0f);

        vertices[16].texture = glm::vec2(0.0f,1.0f);
        vertices[17].texture = glm::vec2(1.0f,1.0f);
        vertices[18].texture = glm::vec2(1.0f,0.0f);
        vertices[19].texture = glm::vec2(0.0f,0.0f);

        //Alt Yüz
        vertices[20].position = positions[3];
        vertices[21].position = positions[2];
        vertices[22].position = positions[6];
        vertices[23].position = positions[7];

        vertices[20].normal = glm::vec3(0.0f,-1.0f,0.0f);
        vertices[21].normal = glm::vec3(0.0f,-1.0f,0.0f);
        vertices[22].normal = glm::vec3(0.0f,-1.0f,0.0f);
        vertices[23].normal = glm::vec3(0.0f,-1.0f,0.0f);

        vertices[20].texture = glm::vec2(0.0f,1.0f);
        vertices[21].texture = glm::vec2(1.0f,1.0f);
        vertices[22].texture = glm::vec2(1.0f,0.0f);
        vertices[23].texture = glm::vec2(0.0f,0.0f);

        
        for(int i=0;i<6;i++)
        {
            indices.push_back(i*4);
            indices.push_back(i*4+2);
            indices.push_back(i*4+1);

            indices.push_back(i*4);
            indices.push_back(i*4+3);
            indices.push_back(i*4+2);            
        }


        // findNormals(vertices,vertexCount,indices.data(),indices.size());
        // averageNormals(vertices,vertexCount);
        VertexArrayObject *va = new VertexArrayObject;
        VertexBuffer *vb= new VertexBuffer;
        IndexBuffer *ib = new IndexBuffer;
        va->create();
        va->bind();

        // vb ve ib'nin create işlemlerinin içerisine bind etme işemi yapmışız bu da otamatik oalrak onların aktif va'ya bağlanmasını sağlar
        vb->create(vertices,sizeof(Vertex)*vertexCount);
        ib->create(indices.data(),indices.size()*sizeof(unsigned int));
        va->setVertexBuffer(vb);
        va->setIndexBuffer(ib);

        va->addVertexAttribute(VertexAttributeType::Position);
        va->addVertexAttribute(VertexAttributeType::Texture);
        va->addVertexAttribute(VertexAttributeType::Normal);
        va->activateAttributes();
        m_vaoMap[ShapeTypes::Cube] =va;
        va->unbind();
        return va;          
    }
 VertexArrayObject* ShapeCreator::createCylinder()
{
    // Daha önce oluşturulmuşsa tekrar oluşturmayalım
    if (m_vaoMap.count(ShapeTypes::Cylinder) > 0)
        return m_vaoMap[ShapeTypes::Cylinder];

    // Silindirin temel parametreleri
    const int   segmentCount = 36;  
    const float radius       = 0.5f;
    const float height       = 1.0f;

    std::vector<Vertex>        vertices;
    std::vector<unsigned int>  indices;

    // -----------------------
    // 1) ALT DISK
    // -----------------------
    Vertex bottomCenter;
    bottomCenter.position = glm::vec3(0.0f, -height * 0.5f, 0.0f);
    bottomCenter.texture  = glm::vec2(0.5f, 0.5f);  // Daire merkezini doku koordinatında (0.5, 0.5) alalım
    unsigned int bottomCenterIndex = static_cast<unsigned int>(vertices.size());
    vertices.push_back(bottomCenter);

    // Alt disk çevresi
    for (int i = 0; i < segmentCount; ++i)
    {
        float angle = 2.0f * pi * i / segmentCount;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);

        Vertex v;
        v.position = glm::vec3(x, -height * 0.5f, z);

        // Doku için dairesel koordinat
        float u = 0.5f + 0.5f * std::cos(angle);
        float w = 0.5f + 0.5f * std::sin(angle);
        v.texture = glm::vec2(u, w);

        vertices.push_back(v);
    }

    // Alt disk indeksleri (fan)
    for (int i = 0; i < segmentCount; ++i)
    {
        unsigned int start = bottomCenterIndex;
        unsigned int p1 = bottomCenterIndex + 1 + i;
        unsigned int p2 = bottomCenterIndex + 1 + ((i + 1) % segmentCount);

        indices.push_back(start);
        indices.push_back(p1);
        indices.push_back(p2);
    }

    // -----------------------
    // 2) ÜST DISK
    // -----------------------
    Vertex topCenter;
    topCenter.position = glm::vec3(0.0f, height * 0.5f, 0.0f);
    topCenter.texture  = glm::vec2(0.5f, 0.5f);
    unsigned int topCenterIndex = static_cast<unsigned int>(vertices.size());
    vertices.push_back(topCenter);

    // Üst disk çevresi
    for (int i = 0; i < segmentCount; ++i)
    {
        float angle = 2.0f * pi * i / segmentCount;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);

        Vertex v;
        v.position = glm::vec3(x, height * 0.5f, z);

        float u = 0.5f + 0.5f * std::cos(angle);
        float w = 0.5f + 0.5f * std::sin(angle);
        v.texture = glm::vec2(u, w);

        vertices.push_back(v);
    }

    // Üst disk indeksleri (fan)
    for (int i = 0; i < segmentCount; ++i)
    {
        unsigned int start = topCenterIndex;
        unsigned int p1 = topCenterIndex + 1 + i;
        unsigned int p2 = topCenterIndex + 1 + ((i + 1) % segmentCount);

        indices.push_back(start);
        indices.push_back(p1);
        indices.push_back(p2);
    }

    // -----------------------
    // 3) SİLİNDİR YAN YÜZEYİ
    // -----------------------
    // Yan yüzeyde: segmentCount+1 kadar noktayı (alt+üst) kullanarak “wrap-around” sağlıyoruz
    unsigned int sideStartIndex = static_cast<unsigned int>(vertices.size());

    for (int i = 0; i <= segmentCount; ++i)
    {
        float angle = 2.0f * pi * i / segmentCount;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);

        // Alt kenar
        Vertex vBottom;
        vBottom.position = glm::vec3(x, -height * 0.5f, z);
        // Doku koordinatı (u: 0..1), alt => 0
        vBottom.texture  = glm::vec2(float(i) / float(segmentCount), 0.0f);
        vertices.push_back(vBottom);

        // Üst kenar
        Vertex vTop;
        vTop.position    = glm::vec3(x, height * 0.5f, z);
        // Doku koordinatı (u: 0..1), üst => 1
        vTop.texture     = glm::vec2(float(i) / float(segmentCount), 1.0f);
        vertices.push_back(vTop);
    }

    // Yan yüzey indeksleri (iki üçgen = bir dikdörtgen şerit)
    for (int i = 0; i < segmentCount; ++i)
    {
        unsigned int bottom1 = sideStartIndex + 2 * i;
        unsigned int top1    = bottom1 + 1;
        unsigned int bottom2 = bottom1 + 2;
        unsigned int top2    = bottom2 + 1;

        // İlk üçgen
        indices.push_back(bottom1);
        indices.push_back(top1);
        indices.push_back(bottom2);

        // İkinci üçgen
        indices.push_back(bottom2);
        indices.push_back(top1);
        indices.push_back(top2);
    }

    // -----------------------
    // 4) OPENGL BUFFER OLUŞTURMA
    // -----------------------
    VertexArrayObject* va = new VertexArrayObject;
    VertexBuffer*      vb = new VertexBuffer;
    IndexBuffer*       ib = new IndexBuffer;

    va->create();
    va->bind();

    // Verteks verisini GPU’ya yolla
    vb->create(
        &vertices[0],
        sizeof(Vertex) * vertices.size()
    );

    // Indeks verisini GPU’ya yolla
    ib->create(
        &indices[0],
        indices.size() * sizeof(unsigned int)
    );

    va->setVertexBuffer(vb);
    va->setIndexBuffer(ib);

    // Sadece Pozisyon ve Doku koordinatı
    va->addVertexAttribute(VertexAttributeType::Position);
    va->addVertexAttribute(VertexAttributeType::Texture);
    va->addVertexAttribute(VertexAttributeType::Normal);

    va->activateAttributes();
    m_vaoMap[ShapeTypes::Cylinder] = va;
    va->unbind();

    return va;
}
    void ShapeCreator::deleteInstance(){
        if(getInstance() != nullptr)
            delete getInstance();
    }
    ShapeCreator::~ShapeCreator(){
        for (auto &&it : m_vaoMap)
        {
            delete it.second;
        }
    }
}