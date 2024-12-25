#include "shapecreator.hpp"
#include "vertexarrayobject.hpp"
#include "indexbuffer.hpp"
#include "vertexbuffer.hpp"
#include "vertextypes.hpp"
namespace graf
{
    ShapeCreator* ShapeCreator::m_Ins = nullptr;
    

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
            {-0.5f,  0.5f, 0.5f},
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

        vertices[0].texture = glm::vec2(0.0f,1.0f);
        vertices[1].texture = glm::vec2(1.0f,1.0f);
        vertices[2].texture = glm::vec2(1.0f,0.0f);
        vertices[3].texture = glm::vec2(0.0f,0.0f);


        //Sağ Yüz
        vertices[4].position = positions[1];
        vertices[5].position = positions[5];
        vertices[6].position = positions[6];
        vertices[7].position = positions[2];

        vertices[4].texture = glm::vec2(0.0f,1.0f);
        vertices[5].texture = glm::vec2(1.0f,1.0f);
        vertices[6].texture = glm::vec2(1.0f,0.0f);
        vertices[7].texture = glm::vec2(0.0f,0.0f);



        //Üst Yüz
        vertices[8].position = positions[4];
        vertices[9].position = positions[5];
        vertices[10].position = positions[1];
        vertices[11].position = positions[0];

        vertices[8].texture = glm::vec2(0.0f,1.0f);
        vertices[9].texture = glm::vec2(1.0f,1.0f);
        vertices[10].texture = glm::vec2(1.0f,0.0f);
        vertices[11].texture = glm::vec2(0.0f,0.0f);

        //Sol Yüz
        vertices[12].position = positions[4];
        vertices[13].position = positions[0];
        vertices[14].position = positions[3];
        vertices[15].position = positions[7];

        vertices[12].texture = glm::vec2(0.0f,1.0f);
        vertices[13].texture = glm::vec2(1.0f,1.0f);
        vertices[14].texture = glm::vec2(1.0f,0.0f);
        vertices[15].texture = glm::vec2(0.0f,0.0f);

        //Arka Yüz
        vertices[16].position = positions[5];
        vertices[17].position = positions[4];
        vertices[18].position = positions[7];
        vertices[19].position = positions[6];

        vertices[16].texture = glm::vec2(0.0f,1.0f);
        vertices[17].texture = glm::vec2(1.0f,1.0f);
        vertices[18].texture = glm::vec2(1.0f,0.0f);
        vertices[19].texture = glm::vec2(0.0f,0.0f);

        //Alt Yüz
        vertices[20].position = positions[3];
        vertices[21].position = positions[2];
        vertices[22].position = positions[6];
        vertices[23].position = positions[7];

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
        va->activateAttributes();
        m_vaoMap[ShapeTypes::Cube] =va;
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