#include <iostream>

using std::cout;
using std::cin;

enum reg{
    Zero,
    sp,
    gp,
    tp,
    s0
};

int main(){
    reg r = reg::Zero;
    cout << r << '\n';
}