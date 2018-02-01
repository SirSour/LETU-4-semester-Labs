#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include "shape.h"

using namespace std;

class myshape : public rectangle {
    line* l_eye;
    line* r_eye;
    line* mouth;
public:
    string name;
    myshape(point, point, string name);
    void draw();
    void move(int, int, point);
};

myshape::myshape(point a, point b, string new_name) : rectangle(a,b) {

    name = new_name;
    int ll = neast().x - swest().x + 1;
    int hh = neast().y - swest().y + 1;

    l_eye = new line(point(swest().x + 2, swest().y + hh*3/4), 2, " левый глаз.");
    r_eye = new line(point(swest().x + ll - 4, swest().y + hh*3/4), 2, " правый глаз.");
    mouth = new line(point(swest().x + 2, swest().y + hh/4), ll - 4, " рот.");
}

void myshape::draw() {

    rectangle::draw();
    int a = (swest().x + neast().x)/2;
    int b = (swest().y + neast().y)/2;
    put_point(point(a,b));
}

void myshape::move(int a, int b, point error = point (0,0)) {

    rectangle::move(a,b,error);
    l_eye->move(a,b,error);
    r_eye->move(a,b,error);
    mouth->move(a,b,error);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    system("title = Наследование и обработка исключительных ситуаций.");

    screen_init();
    shape *face = new myshape(point(20,12), point(40,23), "лицо.");
    shape *whiskers_l;
    try {
        whiskers_l = new paral( point(1,1), point(7,5), 2 ,"левая бакенбарда.");
    }
    catch(init_worse &s) {
        cout << s.what << "Теперь объект - ошибка." << endl;
        whiskers_l = new rubbish(s.worse, s.what);
    }
    shape_refresh();
    reflectable *whiskers_r;
    try {
        whiskers_r = new paral(point(7,1),point(13,5), 2 ,"правая бакенбарда.");
    }
    catch(init_worse &s) {
        cout << s.what << "Теперь объект - ошибка." << endl;
        whiskers_r = new rubbish(s.worse, s.what);
    }
    reflectable *horn_l;
    try {
        horn_l = new paral(point(19,1),point(24,10),3, "левый рог.");
    }
    catch(init_worse &s) {
        cout << s.what << "Теперь объект - ошибка." << endl;
        horn_l = new rubbish(s.worse, s.what);
    }
    shape *horn_r;
    try {
        horn_r = new paral(point(25,1),point(30,10), 3,"правый рог.");
    }
    catch(init_worse &s) {
        cout << s.what << "Теперь объект - ошибка." << endl;
        horn_r = new rubbish(s.worse, s.what);
    }
    shape_refresh();
    _getch();
    whiskers_r->flip_horisontally();
    horn_l->flip_horisontally();
    left_whis(whiskers_l, face);
    shape_refresh();
    _getch();
    right_whis(whiskers_r, face);
    shape_refresh();
    _getch();
    shape_refresh();
    _getch();
    left_horn(horn_l, face);
    shape_refresh();
    _getch();
    right_horn(horn_r, face);
//    cout<< <<endl;
    _getch();

    shape_refresh();
    cout << "На экране присутствует " << list_shape.size()+1 << " элементов.";
    _getch();
    system("pause");
    return 0;
}



