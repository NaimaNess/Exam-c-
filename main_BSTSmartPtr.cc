#include<BSTSmartPtr.h>


int main()
{
    bst< int , double, std::less<int>> t{};
    auto x1 = std::pair(55, 2.3);
    auto x2 = std::pair<int,double>{142,0.123};
    t.insert(std::pair<int,double>{4,0.3});
    t.insert(x1);
    t.insert(x2);
    std::cout << "The tree with smart ponters is " << t << std::endl;
    t.clear();
    return 0;
 }