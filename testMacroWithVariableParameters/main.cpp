//
//  main.cpp
//  testMacroWithVariableParameters
//
//  Created by liyoudi on 15/1/26.
//  Copyright (c) 2015Äê liyoudi. All rights reserved.
//

#include <iostream>

#define INNER_MACRO_1(p1) std::cout << p1 << std::endl
#define INNER_MACRO_2(p1, p2) std::cout << p1 << p2 << std::endl
#define INNER_MACRO_3(p1, p2, p3) std::cout << p1 << p2 << p3 << std::endl

#define SOME_MACRO(n, ...) INNER_MACRO_##n(__VA_ARGS__)

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    SOME_MACRO(1, 10);
    SOME_MACRO(2, "second macro with ", 11211);
    return 0;
}
