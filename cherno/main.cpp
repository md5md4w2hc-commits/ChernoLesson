#include <iostream>
#include "glfw3.h"
#include "glfw3native.h"

int main()
{
    int a = glfwInit();
    std::cout << a << std::endl;
    
    std::cin.get();
}
