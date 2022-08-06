#include <algorithm>
#include <iostream>
#include <list>

int main(){
    std::list<int> l = {1};
    std::cout << sizeof(l) << std::endl;
    std::cout << l.size() << std::endl;
    l.pop_front();
    std::cout << sizeof(l) << std::endl;
    std::cout << l.size() << std::endl;
}
