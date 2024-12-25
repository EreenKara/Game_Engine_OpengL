#ifndef functiontypes_hpp
#define functiontypes_hpp
#include<functional>
#include "shaderprogram.hpp"
namespace graf
{
    using namespace std;

    using ImGuiRenderFunction = function<void()>;
    using RenderFunction = function<void()>;
    using KeyboardFunction = function<void(int,int,int)>;
    using MouseFunction = function<void(double,double)>;
}

#endif