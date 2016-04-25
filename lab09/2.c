#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIN 0.8660254

typedef struct{
    double x, y;
} coordinate;

// Draws the computation
void draw_koch_snowflake(coordinate a, coordinate b, int t, FILE *f){
    if(!t){
        fprintf(f, "M%f %f L%f %F ", a.x, a.y, b.x, b.y);
        return;
    }
    coordinate tmp = { (b.x - a.x)/3, (b.y - a.y)/3};
    coordinate c = {a.x + tmp.x, a.y + tmp.y};
    coordinate d = {a.x + (1.5 * tmp.x - SIN * tmp.y), a.y + (1.5 * tmp.y + SIN * tmp.x)};
    coordinate e = {a.x + 2 * tmp.x, a.y + 2 * tmp.y};
    draw_koch_snowflake(a, c, t-1, f);
    draw_koch_snowflake(c, d, t-1, f);
    draw_koch_snowflake(d, e, t-1, f);
    draw_koch_snowflake(e, b, t-1, f);
}

// Provides a frame for the path
void framework(coordinate a, coordinate b, int t, FILE *f){
    fprintf(f, "<path d=\" ");
    draw_koch_snowflake(a, b, t, f);
    fprintf(f, " \" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />");
}

int main(int argc, char *argv[]){
    int w, h, r;
    w = h = 512;
    r = w / 3;
    int n = atoi(argv[1]);
    FILE *f;
    if( !(f=fopen("koch.svg","w")) ) return -1;
    fprintf(f,"<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
        "width=\"%d\" height=\"%d\">", w, h);
    coordinate a = {w / 2, h / 2 + r};
    coordinate b = {w / 2 - r * SIN, h / 2 - r * 0.5};
    coordinate c = {w / 2 + r * SIN, h / 2 - r * 0.5};
    framework(b, a, n, f);
    framework(c, b, n, f);
    framework(a, c, n, f);
    fprintf(f, "</svg>");
    fclose(f);
    return 0;
}
