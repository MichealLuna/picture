#include "Picture.h"

const char* p[] = 
{
    "Pairs",
    "in the",
    "Spring"
};

Picture title(const char* str)
{
    const char* const strs [] = {str};
    return frame(Picture(strs,1));
}

int main()
{

    Picture a(p,sizeof(p)/sizeof(char*));
    std::cout<< title("a") << a;

    Picture b = frame(a);
    std::cout<< title("b = frame(a)") << b;

    Picture c = a | b;
    std::cout<< title("c = frame(a | b)") << frame(c);
    std::cout<< title("frame(a & c)") << frame(a & c);
    // std::cout<< frame((a|b)|frame(a)|frame(b));
}