#include "church_numerals.h"

#include <iostream>

inline int foo(int x) {
    std::cout<<"hello"<<x<<std::endl; 
    return x+1;
}

int main(){
    auto One = Church::church_one;

    auto Two = Church::church_add(One,One);
    auto Four = Church::church_add(Two,Two);
    auto Eight = Church::church_mult(Two,Four);
    auto Seven = Church::church_pred(Eight);
    auto Five = Church::church_minus(Seven,Two);

    Five(foo)(1);
}
