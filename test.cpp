#include "angleDetection.h"
#include <iostream>
#include <cmath>
using namespace std;

int main(){
    std::string path = "/Users/miladhaghighi/Downloads/Screenshot_2024-11-06_at_10.23.53_PM.png";
    FindAngle test(path);
    cout << test.getAngle() << endl;
    return 0;
}
