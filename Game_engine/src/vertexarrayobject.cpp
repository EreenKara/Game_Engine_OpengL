#include<glad/glad.h>
#include "vertexarrayobject.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
namespace graf
{
   void VertexArrayObject::create()
   {
         glGenVertexArrays(1, &m_id); 
         m_stride = 0;
   }
   void VertexArrayObject::bind()
   {
        glBindVertexArray(m_id);
   }
   void VertexArrayObject::setVertexBuffer(VertexBuffer* vb)
   {
        m_vb =vb;
        // shaderprogramına bağlı olan vertex array'e bağlanıyor vertex buffer. Yani biz va.bind() yapmazsak
        // o han hali hazırda hangi va varsa ona bağlanacak vertexbuffer. Ancak bind yaptığımızan bu nesnenin va'sına bağlanacak. Emin olduk.
        bind();
        m_vb->bind();
   }
   void VertexArrayObject::draw()
   {
     // glDrawArrays() varken neden elemnts kullandık
     // Cunku drawarray çizim yaparken VBO'yu kullanırken drawelemetns IBO'yu kullanıyor. 
     glDrawElements(GL_TRIANGLES,m_ib->indexCount(),GL_UNSIGNED_INT,0);
   }
   void VertexArrayObject::setIndexBuffer(IndexBuffer* ib)
   {
          m_ib =ib;
          // shaderprogramına bağlı olan vertex array'e bağlanıyor index buffer. Yani biz va.bind() yapmazsak
          // o han hali hazırda hangi va varsa ona bağlanacak inedex buffer. Ancak bind yaptığımızan bu nesnenin va'sına bağlanacak. Emin olduk.
          bind();
          m_ib->bind();           
   }
   void VertexArrayObject::addVertexAttribute(VertexAttributeType type)
   {
        m_attributes.push_back(type);
        m_stride+= getTypeSize(type);  
   }
   void VertexArrayObject::activateAttributes()
   {

        int location=0;
        for(int i=0;i<m_attributes.size();i++)
        {
            int attributeSize = getTypeSize(m_attributes[i]);
            int elementCount =attributeSize/sizeof(float);
        
            glVertexAttribPointer(i, 
                                   elementCount, 
                                    GL_FLOAT, 
                                    GL_FALSE, 
                                    m_stride, 
                                    (void*)location);
            location+=attributeSize;
            glEnableVertexAttribArray(i);
        }
   }

   int VertexArrayObject::getTypeSize(VertexAttributeType type)
   {
        int size=0;

        switch (type)
        {
        case VertexAttributeType::Position:
        case VertexAttributeType::Normal:
            size=sizeof(float)*3;
            break;
        case VertexAttributeType::Color:
            size=sizeof(float)*4;
        break;
        case VertexAttributeType::Texture:
            size=sizeof(float)*2;
            break;
        default:
            break;
        }
        return size;
   }
   void VertexArrayObject::release()
   {
        glDeleteVertexArrays(1,&m_id);
        m_vb->release();
        delete m_vb;
        m_ib->release();
        delete m_ib;
   }

   void VertexArrayObject::unbind()
   {
        glBindVertexArray(0);
   }
}