#!/bin/bash

# Check for filename
if [ -z "$1" ]; then
    echo "Usage: canonize ClassName"
    exit 1
fi

CLASSNAME=$(basename "$1" .hpp)
HEADER_FILE="$CLASSNAME.hpp"
SOURCE_FILE="$CLASSNAME.cpp"
INCLUDE_GUARD=$(echo "${CLASSNAME}_HPP" | tr '[:lower:]' '[:upper:]')

# Create header file if it doesn't exist
if [ ! -f "$HEADER_FILE" ]; then
cat <<EOF > "$HEADER_FILE"
#ifndef $INCLUDE_GUARD
#define $INCLUDE_GUARD

class $CLASSNAME 
{
    private:
    
    public:
        $CLASSNAME();
        $CLASSNAME(const $CLASSNAME& other);
        $CLASSNAME& operator=(const $CLASSNAME& other);
        ~$CLASSNAME();
};

#endif
EOF
fi

# Create cpp file if it doesn't exist
if [ ! -f "$SOURCE_FILE" ]; then
cat <<EOF > "$SOURCE_FILE"
#include "$HEADER_FILE"

$CLASSNAME::$CLASSNAME() 
{
    std::cout << "$CLASSNAME default constructor called" << std::endl;
}

$CLASSNAME::$CLASSNAME(const $CLASSNAME& other)
{
    std::cout << "$CLASSNAME copy constructor called" << std::endl;
}

$CLASSNAME& $CLASSNAME::operator=(const $CLASSNAME& other) 
{
    std::cout << "$CLASSNAME assignment operator called" << std::endl;
    return (*this);
}

$CLASSNAME::~$CLASSNAME() 
{
    std::cout << "$CLASSNAME deconstructor called" << std::endl;
}
EOF
fi

echo "Generated $HEADER_FILE and $SOURCE_FILE"
