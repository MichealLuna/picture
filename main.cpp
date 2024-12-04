#include "Picture.h"

const char* p[] = 
{
    "Pairs",
    "in the",
    "Spring"
};

int main()
{
    Picture a(p,sizeof(p)/sizeof(char*));
    std::cout << a;
    Picture b = frame(a);
    std::cout << b;
    std::cout<< frame(a & (a | b));
    std::cout<< frame((a|b)|frame(a)|frame(b));
}