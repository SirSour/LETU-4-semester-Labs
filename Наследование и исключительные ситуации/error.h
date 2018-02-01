#include <string>
#include <iostream>
using namespace std;
class init_worse {

public:
    string what;
    init_worse(){/*cout << "Некорректные параметры при формировании фигуры: "*/;}
    point worse;
};

class out_of_screen {

public:
    string what;
    out_of_screen() {/*cout << "Выход за пределы экрана. "*/;}
    point scr;
};


