# church-cpp
a c++17 implementation of basic church numerals

## How to use:

a typical use case:
```cpp
#include "church_numerals.h"
#include <iostream>

inline int foo(int x){
    std::cout<<"hello"<<x<<std::endl; 
    return x+1;
}

int main(){
    auto One = Church::church_one;

    auto Two = Church::church_add(One,One);
    auto Four = Church::church_add(Two,Two);
    auto Eight = Church::church_mult(Two,Four);
    auto Seven = Church::church_pred(Eight);

    Seven(foo)(1);
}
```
