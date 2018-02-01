#include "screen.h"
#include "error.h"
#include <string.h>
#include <list>
#include <algorithm>

#define ERR catch(...){cout << "Непредвиденная ошибка."; cin.get(); throw;}

char screen[xmax][ymax];
enum color {black = '*', white = ' ', mistake = '@'};
bool init_rubbish = 0;

void screen_init() {

    for(int y = 0; y < ymax; ++y){
        for (int x = 0; x < xmax; ++x){
            screen[x][y] = white;
            }
    }

}
int on_screen(int a, int b) {

    return 0 <= a && a < xmax && 0 <= b && b <= ymax;
}

void put_point(int a, int b) {

    if(init_rubbish)
        screen[abs(a)][abs(b)] = mistake;
    else
        if(on_screen(a, b))
            screen[a][b] = black;
        else
        {
            out_of_screen out;
            throw out; //в put_line
        }
}

void put_line(int x0, int y0, int x1, int y1) {

    try {
        int dx = 1;
        int a = x1-x0;
        if(a < 0)
            dx = -1, a = -a;
        int dy = 1;
        int b = y1-y0;
        if(b < 0)
            dy = -1, b = -b;
        int two_a = 2*a;
        int two_b = 2*b;
        int xcrit = -b+two_a;
        int esp = 0;

        for(; ;){
            put_point(x0, y0);
            if(x0 == x1 && y0 == y1) break;
            if(esp <= xcrit)
                x0 += dx, esp += two_b;
            if(esp >= a || a < b)
                y0 += dy, esp -= two_a;
        }
    }
    catch(out_of_screen &out)
    {
        throw out; //в draw
    }
    ERR;
}

void screen_clear() {

    screen_init();
}

void screen_refresh() {

    for(int y = ymax - 1; 0 <= y; --y){
        for(int x = 0; x < xmax; ++x)
            cout << screen[x][y];
        cout << endl;
    }
}

struct shape {

    string name;
    virtual point north() const = 0;
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual void draw() = 0;
    virtual void move (int, int, point) = 0;
    shape();
};
static list <shape*> list_shape;
shape::shape(){

    list_shape.push_back(this);
}

class rotatable: virtual public shape {

public:
    virtual void rotate_left() = 0;
    virtual void rotate_right() = 0;
};

class reflectable:virtual public shape {

public:
    virtual void flip_horisontally() = 0;
    virtual void flip_vertically() = 0;
};

class line:public shape {

    point w, e;
public:
    string name;
    point north() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point south() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point east() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point west() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point neast() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point seast() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point nwest() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    point swest() const{return point((w.x+e.x)/2, e.y < w.y ? w.y:e.y);}
    void move(int a, int b, point s = point(0,0)) {
        w.x += a; w.y += b; e.x += a; e.y += b;
        if(!on_screen(w.x, w.y) || !on_screen(e.x, e.y)) {
            out_of_screen out;
            out.what = name;
            out.scr = s;
            this->~line();
            throw out;
        }
    }
    void draw() {put_line(w, e);}
    line(point a, int L, string new_name = " линия." ) : w(point(a.x + L - 1, a.y)), e(a) {  };
    line(point a, point b, string new_name = " линия."):w(a),e(b) {
        if(!on_screen(w.x,w.y) || !on_screen(e.x, e.y)){
            init_worse bad;
            bad.what = name;
            bad.worse = point((west().x - east().x)/2, (north().y - south().y)/2);
            this->~line();
            throw bad;
        }
        else name = new_name;
    };
    ~line(){list_shape.remove(this); }
};

class rectangle: public rotatable {

    point sw,ne;
public:

    virtual point north() const{return point ((sw.x,ne.x)/2,ne.y);}
    virtual point south() const{return point ((sw.x,ne.x)/2,sw.y);}
    virtual point west() const{return point (sw.x, (sw.y,ne.y)/2);}
    virtual point east() const{return point (ne.x, (sw.y,ne.y)/2);}
    virtual point neast() const{return ne;}
    virtual point nwest() const{return point (sw.x,ne.y);}
    virtual point seast() const{return point (ne.x, sw.y);}
    virtual point swest() const{return sw;}
    string name;
    virtual void move(int a, int b, point scr = point(0,0)) {
        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
        if(!on_screen(sw.x,sw.y) || !on_screen(ne.x,ne.y)) {
            out_of_screen out;
            out.what = name;
            out.scr = scr;
            this->~rectangle();
            throw out;
        }
    }
    void rotate_right() // Поворот вправо относительно se
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        sw.x = ne.x - h*2; ne.y = sw.y + w/2;
    }

    void rotate_left() //Поворот влево относительно sw
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h*2; ne.y = sw.y + w/2;
    }

    void move(int a, int b){
        sw.x += a; sw.y += b;
        ne.x += a; ne.y += b;
    }
    virtual void draw();
    rectangle(point, point, string name = " прямоугольник.");
    ~rectangle() {list_shape.remove(this);}
};

rectangle::rectangle(point a, point b, string name) {

     if (a.x <= b.x) {
        if (a.y <= b.y) sw = a, ne = b;
        else sw = point(a.x, b.y), ne = point(b.x, a.y);
        }
    else {
        if (a.y <= b.y) sw = point(b.x, a.y), ne = point(a.x, b.y);
        else sw = b, ne = a;
      }
     // cout<<" "<<ne.x<<" "<<ne.y<<endl<<sw.x<<" "<<sw.y<<endl;
    if(!on_screen(sw.x,sw.y) || (!on_screen(ne.x,ne.y))) {
       init_worse bad;
       bad.what = name;
       bad.worse = point(west().y,north().x);
       this->~rectangle();
       throw bad;
    }
}

void rectangle::draw() {

    try {
        point nw(sw.x, ne.y);
        point se(ne.x, sw.y);
        put_line(nw, ne);
        put_line(ne, se);
        put_line(se, sw);
        put_line(sw, nw);
    }
    catch (out_of_screen &out) {
        out.what = name;
        out.scr = point((sw.x + ne.x)/2, (sw.y+ne.y)/2);
        this->~rectangle();
        throw out;
    }
    ERR;
}

class rubbish: public reflectable {

    point k;
public:
    point north() const {return point(k.x, k.y+1);}
    point south() const {return point(k.x, k.y-1);}
    point east() const {return point(k.x+1, k.y);}
    point west() const {return point(k.x-1, k.y);}
    point neast() const {return point(k.x+1, k.y+1);}
    point seast() const {return point(k.x+1, k.y-1);}
    point nwest() const {return point(k.x-1, k.y+1);}
    point swest() const {return point(k.x-1, k.y-1);}
    string name;
     void flip_horisontally()
    {
    };
    void flip_vertically()
    {
        flip_horisontally();
    };
    void move(int a, int b, point s = point(0,0)) {
        k.x += a; k.y += b;
    }
    void draw();
    rubbish(point a, string n = " лишнее."): k(a){name = n;};
    ~rubbish() {list_shape.remove(this);}
};

void rubbish::draw() {

    init_rubbish = 1;
    put_line(nwest(), neast());
    put_line(neast(), seast());
    put_line(seast(), swest());
    put_line(swest(), nwest());
    init_rubbish = 0;
}

void shape_refresh() {

    screen_clear();
    for(list <shape*> p = list_shape; !p.empty(); p.pop_front()) {
        try {
            p.front()->draw();

        }
        catch(out_of_screen &out) {
            cout << out.what << "Выход за пределы экрана, объект-ошибка cоздан." << endl;
            shape *s = new rubbish(out.scr);
            list_shape.remove(p.front());
        }
        ERR;
    }
    screen_refresh();
}

void up(shape *p, shape *q) {

    try {
        point n = q->north();
        point s = p->south();
        point error = point (n.x, n.y + 2);
        p->move(n.x - s.x, n.y - s.y + 1, error);
    }
    catch(out_of_screen &out) {
        cout << "Выход за пределы экрана: " << out.what << "  Oбъект-ошибка создан.";
        shape *s = new rubbish(out.scr);
        list_shape.remove(p);
    }
    ERR;
}

void left_whis(shape *p, shape *q) {

    try {
        point n = q->swest();
        point s = p->neast();
        point err = point(n.x - 2, n.y - 1);
        p->move(n.x - s.x - 1, n.y - s.y, err);
    }
    catch(out_of_screen &out) {
        cout << "Выход за пределы экрана: " << out.what << "  Oбъект-ошибка создан.";
        shape *s = new rubbish(out.scr);
        list_shape.remove(p);
    }
    ERR;
}

void right_whis(shape *p, shape *q) {

    try {
        point n = q->seast();
        point s = p->nwest();
        point err = point(n.x + 2, n.y);
        p->move(n.x - s.x + 1, n.y - s.y, err);
    }
    catch(out_of_screen &out) {
        cout << "Выход за пределы экрана: " << out.what << "  Oбъект-ошибка создан.";
        shape *s = new rubbish(out.scr);
        list_shape.remove(p);
    }
    ERR;
}

void left_horn(shape *p, shape *q) {

    try {
        point n = q->nwest();
        point s = p->seast();
        point err = point(n.x - 2, n.y);
        p->move(n.x - s.x - 1, n.y - s.y, err);
    }
    catch(out_of_screen &out) {
        cout << "Выход за пределы экрана: " << out.what << "   Oбъект-ошибка создан.";
        shape *s = new rubbish(out.scr);
        list_shape.remove(p);
    }
    ERR;
}

void right_horn(shape *p, shape *q) {

    try {
        point n = q->neast();
        point s = p->swest();
        point err = point(n.x + 2, n.y);
        p->move(n.x - s.x + 1, n.y - s.y, err);
    }
    catch(out_of_screen &out) {
        cout << "Выход за пределы экрана: " << out.what << "  Oбъект-ошибка создан.";
        shape *s = new rubbish(out.scr);
        list_shape.remove(p);
    }
    ERR;
}

class paral: public rectangle, public reflectable {
point sw,ne,;
int offset;
public:

    point north() const{return point ((sw.x+ne.x)/2,ne.y-offset/2);}
    point south() const{return point ((sw.x+ne.x)/2,sw.y+offset/2);}
    point east() const{return point (ne.x, (sw.y+ne.y+offset)/2);}
    point west() const{return point (sw.x, (sw.y+ne.y-offset)/2);}
    point neast() const{return ne;}
    point seast() const{return point (ne.x,sw.y+offset);}
    point nwest() const{return point (sw.x, ne.y-offset);}
    point swest() const{return sw;}
    string name;
    paral(point a, point b, int off, string name);
    ~paral() {list_shape.remove(this);}

    void flip_horisontally()
    {
        sw.y=sw.y+offset;
        ne.y=ne.y-offset;
        offset=-offset;
    };
    void flip_vertically()
    {
        flip_horisontally();
    };
    void draw();
    void move(int a, int b, point scr = point(0,0)) {
        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
        if(!on_screen(sw.x,sw.y) || !on_screen(ne.x,ne.y)) {
            out_of_screen out;
            out.what = name;
            out.scr = scr;
            this->~paral();
            throw out;
        }
    }
};
paral::paral(point a, point b, int off, string name): rectangle(a, b, name) {

    offset=off;
    if (a.x <= b.x) {
        if (a.y <= b.y) sw = a, ne = b;
        else sw = point(a.x, b.y), ne = point(b.x, a.y);
        }
    else {
        if (a.y <= b.y) sw = point(b.x, a.y), ne = point(a.x, b.y);
        else sw = b, ne = a;
      }
     //cout<<offset<<" "<<ne.x<<" "<<ne.y<<endl<<sw.x<<" "<<sw.y<<endl;
    if(!on_screen(sw.x,sw.y) || (!on_screen(ne.x,ne.y))) {
       init_worse bad;
       bad.what = name;
       bad.worse = point(west().y,north().x);
       this->~rectangle();
       throw bad;
    }


    //cout<<offset<<" "<<ne.x<<" "<<ne.y<<endl<<sw.x<<" "<<sw.y<<endl;
    if(!on_screen(sw.x,sw.y) || (!on_screen(ne.x,ne.y))) {
       init_worse bad;
       bad.what = name;
       bad.worse = point(west().y,north().x);
       this->~paral();
       throw bad;
    }
}

void paral::draw() {

    try {
        point nw(sw.x, ne.y-offset);
        point se(ne.x, sw.y+offset);
        put_line(ne, se);
        put_line(nw, ne);
        put_line(se, sw);
        put_line(sw, nw);
    }
    catch (out_of_screen &out) {
        out.what = name;
        out.scr = point((sw.x + ne.x)/2, (sw.y+ne.y)/2);
        this->~paral();
        throw out;
    }
    ERR;
}
