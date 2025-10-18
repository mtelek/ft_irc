#include "helper.hpp"

helper::helper() 
{
    std::cout << "helper default constructor called" << std::endl;
}

helper::helper(const helper& other)
{
    std::cout << "helper copy constructor called" << std::endl;
}

helper& helper::operator=(const helper& other) 
{
    std::cout << "helper assignment operator called" << std::endl;
    return (*this);
}

helper::~helper() 
{
    std::cout << "helper deconstructor called" << std::endl;
}
