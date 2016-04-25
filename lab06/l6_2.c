#include <stdio.h>
#include <math.h>
#define N 5

struct gshape {
  int type ; /* 0/1/2 */
  int ID;
  union
  {
    struct
    {
      float radius;
      float x, y;
    } circle;

    struct
    {
      float x1,y1, x2,y2, x3,y3;
    } triangle;

    struct {
      float x1,y1;
      float x4,y4;
    } rectangle ;

  } gshape ;

  /* upper left */
  /* right bottom */
  /* function pointers in struct provide contained intelligence */
  void (*read_gshape)(struct gshape *gs);
  /* like an object having both data and code */
  void (*write_gshape)(struct gshape *gs);
  void (*compute_area)(struct gshape *gs);
};

void read_circle ( struct gshape *gs) {
  printf("radius? ");
  scanf("%f", &(gs->gshape.circle.radius));
  printf("X center? ");
  scanf("%f" , &(gs->gshape.circle.x));
  printf("Y center? ");
  scanf("%f" , &(gs->gshape.circle.x));
}

void write_circle ( struct gshape *gs) {
  printf("Circle Radius: %f X: %f Y: %f\n", gs->gshape.circle.radius,
                                            gs->gshape.circle.x,
                                            gs->gshape.circle.y);
}

void read_triangle ( struct gshape *gs) {
  printf("What is the triangle coordonates? (x1,y1) (x2,y2), (x3,y3)\n");
  scanf("%f %f %f %f %f %f", &gs->gshape.rectangle.x1,
                       &gs->gshape.triangle.y1,
                       &gs->gshape.triangle.x2,
                       &gs->gshape.triangle.y2,
                       &gs->gshape.triangle.x3,
                       &gs->gshape.triangle.y3);
}


void write_triangle ( struct gshape *gs) {
  printf("Triangle (%f,%f) (%f,%f), (%f,%f)\n", gs->gshape.triangle.x1,
                                                gs->gshape.triangle.y1,
                                                gs->gshape.triangle.x2,
                                                gs->gshape.triangle.y2,
                                                gs->gshape.triangle.x3,
                                                gs->gshape.triangle.y3);
}

void read_rectangle(struct gshape *gs) {
  printf("What's the A B C D of the Rectangle?\n" );
  scanf("%f %f %f %f", &gs->gshape.rectangle.x1,
                       &gs->gshape.rectangle.y1,
                       &gs->gshape.rectangle.x4,
                       &gs->gshape.rectangle.y4);
}

void write_rectangle( struct gshape *gs) {
  printf("Heres's the A B C D of the Rectangle\n" );
  printf("%f %f %f %f\n", gs->gshape.rectangle.x1,
                       gs->gshape.rectangle.y1,
                       gs->gshape.rectangle.x4,
                       gs->gshape.rectangle.y4);
}

void area_rectangle(struct gshape *gs) {
  double length = gs->gshape.rectangle.x4 - gs->gshape.rectangle.x1;
  double width = gs->gshape.rectangle.y4 - gs->gshape.rectangle.x4;
  printf("AREA OF RECTANGLE #%d IS: %g\n", gs->ID, fabs(length*width) );
}

int main() {
  int i, type;
  struct gshape gs[N];
  for (i = 0; i < N; i++) {
    do {
      printf("ShapeID %d: ", i);
      printf("What type of shape is it? (0=circle , 1=triangle , 2=rectangle)\n");
      scanf("%d", &type);
      gs[i].type = type;
      gs[i].ID = i;
      switch (gs[i].type) {
        case 0:
          gs[i].read_gshape = &read_circle;
          gs[i].write_gshape = &write_circle;
          gs[i].read_gshape(&gs[i]);
          gs[i].write_gshape(&gs[i]);
          break;
        case 1:
          gs[i].read_gshape = &read_triangle;
          gs[i].write_gshape = &write_triangle;
          gs[i].read_gshape(&gs[i]);
          gs[i].write_gshape(&gs[i]);
          break;
        case 2:
          gs[i].read_gshape = &read_rectangle;
          gs[i].write_gshape = &write_rectangle;
          gs[i].compute_area = &area_rectangle;

          // calls
          gs[i].read_gshape(&gs[i]);
          gs[i].write_gshape(&gs[i]);
          break;
        default:
          fprintf(stderr, "Err!\n");
      }
    } while ((gs[i].type != 0) && (gs[i].type != 1) && (gs[i].type != 2));
  }

  // COMPUTE AREA
  printf("\n\n");
  for (int i = 0; i < N; i++) {
    if (gs[i].type == 2) {
      gs[i].compute_area(&gs[i]);
    }
  }
}
