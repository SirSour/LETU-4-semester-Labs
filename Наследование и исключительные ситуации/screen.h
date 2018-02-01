const int xmax = 80;
const int ymax = 40;

class point {
public:
    int x, y;
    point(){};
    point(int a, int b):x(a),y(b){}
};

void put_point(int a, int b);
void put_point(point p) {put_point(p.x, p.y);}
void put_line(int, int, int, int);
void put_line(point a, point b){put_line(a.x, a.y, b.x, b.y);}

extern void screen_init();
extern void screen_destroy();
extern void screen_refresh();
extern void screen_clear();
