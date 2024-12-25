/*
    Euler açıları 0'Dan başlayarak oluşturur ve verdiğimiz X Y Z sırasının öenmi vardır
    X Y Z şeklinde verirsek ilk verdiğimiz X olacağından X aynı zamanda global X gibi davranarak açı yapmış oalcka.

    Cameranın çalışma mantığında projeksiyon matrisindenn ötürü gelen sebeplerle merkeze çekilme durumu vardır.
    Camera'da projeksiyon matrisini kameranın konumuna göre ayarlamak her bir seferinde bir matris hesabı 
    gerektirdiğinden ve bu matrisin hesaplanamsı çok zor olduğundan bizler farklı bir yol izliyoruz
    Camera ve diğer cisimlerin position'unu merkeze çekip oryantasyonunu ise +z eksenine bakacak şekilde ayarlıyoruz.
    Yani kameraya uygulanmış dünya matrisinin (transform) bir nevi tersini alıyoruz. Elbette burada bütün 
    matrislerin tersini alamayız çünkü translate matrisi toplanıyor diğerleri çarpılıyor. Bu yüzden translate matrisinin
    tersini position değerlerinin -'lisini vererek elde edeceğiz. Rotation matrisini ise matrisin tersinin alarak elde edeceğiz. 

    Ne olursa olsun translate*rotation*scale işlemi sırasında yaparsan döndürme işlemi 
    Orijin etrafında döndürme yapar. Yani local veya global döndürme işlemi farketmeksizin döndürme işlemleri
    orijin etrafında gerçekleşir.
*/



#include "transform.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <cmath> // fmod fonksiyonu için
#include <fstream>

namespace graf
{
    Transform::Transform(glm::vec3 position,
                         glm::vec3 euler,
                         glm::mat4 worldMatrix,
                         glm::mat4 rotationMatrix,
                         glm::mat4 translateMatrix
                         )
    {
        m_position = position;
        m_euler = euler;
        m_worldMatrix = worldMatrix;
        m_rotationMatrix = rotationMatrix;
        m_translateMatrix = translateMatrix;
    }
    Transform::~Transform(){
        
    }
    // Cismin rotasyon olarak pozitif X eksenini yani sağ eksenini veriyor.
    // Bu illaki dünya ile aynı olmak zorunda değil. Orneğin cismin X vektörü 
    // x-> 30derece  y-> 23derece  z-> 12derece   olabilir yani dünyanin x vektörü x->1 y->0 z->0 iken 
    // cisminki böyle olmak zorunda değil.
    glm::vec3 Transform::getRight(){
        return glm::vec3(m_rotationMatrix[0][0],m_rotationMatrix[0][1],m_rotationMatrix[0][2]);
    }
    // Rotasyon olarak pozitif Y ekseni yani yukarı ekseni veriyor.
    glm::vec3 Transform::getUp(){
        return glm::vec3(m_rotationMatrix[1][0],m_rotationMatrix[1][1],m_rotationMatrix[1][2]);
    }
    // Rotasyon olarak pozitif Z ekseni yani bilgisayardan içeri olan  ekseni veriyor.
    glm::vec3 Transform::getLook(){
        return glm::vec3(m_rotationMatrix[2][0],m_rotationMatrix[2][1],m_rotationMatrix[2][2]);
    }
    void Transform::update(){
        m_translateMatrix = glm::translate(glm::mat4(1.0f),m_position);
        glm::extractEulerAngleXYZ(m_rotationMatrix,m_euler.x, m_euler.y, m_euler.z);
        m_worldMatrix = m_translateMatrix*m_rotationMatrix;
    }
    void Transform::rotateGobalX(float angle){
        float radX = glm::radians(angle);
        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f),radX,glm::vec3(1.0f,0.0f,0.0f));
        m_rotationMatrix=rotX*m_rotationMatrix;
        update();
    }
    void Transform::rotateGobalY(float angle){
        float radY = glm::radians(angle);
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f),radY,glm::vec3(0.0f,1.0f,0.0f));
        m_rotationMatrix=rotY*m_rotationMatrix;
        update();
    }
    void Transform::rotateGobalZ(float angle){
        float radZ = glm::radians(angle);
        glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f),radZ,glm::vec3(0.0f,0.0f,1.0f));
        m_rotationMatrix=rotZ*m_rotationMatrix;
        update();
    }

    
    // Normalde rotation matrix X Y ve Z kordinatları ile çarpıalrak dünyadaki oryantasyonu yani açısı belirleniyordu.
    // Ancak burda bizim yaptığımız Matris çapımının bir özelliğinden yaralnarak cismin rotation matrix'inin elde edip
    // yani cismin bakış açısını elde edip onu yeni değiştirme açısıyla çarptık yanii
    // cisim 30 derece X ile açı yaptıysa biz oan bu fonksiyon ile +23 derece daha açı yap demiş olduk.
    // yani buraya verdiğimiz değer eklendi.30+23 = 53 oldu cisim X ile 53 derecelik açı yapar oldu.
    // ancak bunu direkt X Y Z noktaları ile çarpmaktansa rotation matrixi ile çarptık 
    // Eğerki biz rotX matrisini direkt olarak nokta ile çarpsaydık. Cismin noktaları IDEAL cisim noktaları oldugundan yani
    // bizim ekranı full kaplayarak orijinde oldugunu varsaydığımız nokta oldugundan, asagidaki rotX ile çarpmak
    // bize istediğimiz şeyi vermeyecekti. Belirli bir X Ekseni etrafında +23 derece döndürecekti. 
    // O yüzden eklenebilmeyi yapabilmek için rotation matrix'ini güncelledik.


    // DENE!!!!!!
    // rotX'i rotation matrixi ile direkt çarp ve güncellenmiş olması gerekiyor.
    void Transform::rotateLocalX(float angle){
        float radX = glm::radians(angle);
        glm::vec3 normX = glm::normalize(this->getRight());
        // normalde normX Xerine X'si 1 olan vec3 veriXorduk.
        // burada normX verdik cunku normX aslında local Y oluyor. tam olarak glboal Y eksenine göre değil de
        // local Y eksenien göre dönmesini sağlayan şey o.
        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f),radX,normX);


        // Burada vectorel olarak dondurme yapıyoruz.
        // normalde noktanın dönemsini hesaplardan sondakine 1 verip çarıyorduk
        // ancak vectorel oldugunda 0 vermeliyiz.
        // Ayrıca burada y eksenine göre bir döndürme yaptığımızdan Y ekseni sabit kalıyor
        // diğer 2 yön döndürülüyor. Bu yüzden look ve right'ı gncelliyoruz.
        m_rotationMatrix= rotX* m_rotationMatrix;

        update();


    }
    // normalize ediyoruz cunku surekli dondurme yaparken kayna noktalı sayılarda hesaplamalarda 
    // virgulden sonra belirli bir basamak hassasiyeti oldugundan problemler oluşuyor. 
    void Transform::rotateLocalY(float angle){
        float radY = glm::radians(angle);
        // burada
        glm::vec3 normY = glm::normalize(this->getUp());
        // normalde normY yerine Y'si 1 olan vec3 veriyorduk.
        // burada normY verdik cunku normY aslında local Y oluyor. tam olarak glboal Y eksenine göre değil de
        // local Y eksenien göre dönmesini sağlayan şey o.
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f),radY,normY);



        // Burada vectorel olarak dondurme yapıyoruz.
        // normalde noktanın dönemsini hesaplardan sondakine 1 verip çarıyorduk
        // ancak vectorel oldugunda 0 vermeliyiz.
        // Ayrıca burada y eksenine göre bir döndürme yaptığımızdan Y ekseni sabit kalıyor
        // diğer 2 yön döndürülüyor. Bu yüzden look ve right'ı gncelliyoruz.

        m_rotationMatrix= rotY* m_rotationMatrix;

        update();
    }
    void Transform::rotateLocalZ(float angle){
        float radZ = glm::radians(angle);
        glm::vec3 normZ = glm::normalize(this->getLook());
        // normalde normX yerine X'si 1 olan vec3 veriXorduk.
        // burada normX verdik cunku normX aslında local Y oluyor. tam olarak glboal Y eksenine göre değil de
        // local Y eksenien göre dönmesini sağlayan şey o.
        glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f),radZ,normZ);

        glm::vec3 right = glm::normalize(this->getRight());
        glm::vec3 up = glm::normalize(this->getUp());

        // Burada vectorel olarak dondurme yapıyoruz.
        // normalde noktanın dönemsini hesaplardan sondakine 1 verip çarıyorduk
        // ancak vectorel oldugunda 0 vermeliyiz.
        // Ayrıca burada y eksenine göre bir döndürme yaptığımızdan Y ekseni sabit kalıyor
        // diğer 2 yön döndürülüyor. Bu yüzden look ve right'ı gncelliyoruz.
        m_rotationMatrix= rotZ* m_rotationMatrix;
        update();
    }
    glm::vec3 Transform::getPosition(){
        return m_position;
    }
    // Transform MAtrix
    glm::mat4 Transform::getWorldMatrix(){
        return m_worldMatrix;
    }
    glm::mat4 Transform::getRotationMatrix(){
        return m_rotationMatrix;
    }

    glm::vec3 Transform::getEuler(){
        return m_euler;
    }
    glm::vec3 Transform::setEuler(glm::vec3 euler){
        m_rotationMatrix = glm::eulerAngleXYZ(euler.x,euler.y,euler.z);
        update();
    }
    glm::vec3 Transform::setEuler(float x, float y, float z){
        x=x>=2.0f ||x<=-2.0f?std::fmod(x,2.0f):x;
        y=y>=2.0f ||y<=-2.0f?std::fmod(y,2.0f):y;
        z=z>=2.0f ||z<=-2.0f?std::fmod(z,2.0f):z;
        m_rotationMatrix = glm::eulerAngleXYZ(x,y,z);
        update();
    }
    
    void Transform::moveForward(){
        m_position += glm::normalize(getLook());
        update();
    }
    void Transform::moveBackward(){
        m_position -= glm::normalize(getLook());
        update();
    }
    void Transform::moveRight(){
        m_position += glm::normalize(this->getRight());
        update();
    }
    void Transform::moveLeft(){
        m_position -= glm::normalize(this->getRight());
        update();
    }
    void Transform::moveUp(){
        m_position += glm::normalize(this->getUp());
        update();
    }
    void Transform::moveDown(){
        m_position -= glm::normalize(this->getUp());
        update();
    }
    void Transform::setPosition(glm::vec3 position){
        m_position = position;
        update();
    }



    void Transform::setPosition(float x, float y, float z){
        glm::vec3 position(x,y,z);
        m_position = position;
        update();
    }
} // namespace graf
