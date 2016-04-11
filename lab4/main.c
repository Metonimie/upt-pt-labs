#include <stdio.h>
#include <stdlib.h>

enum { point, circle, triangle, rectangle }; // Holds type data. Because numbers are too mainstream.

typedef struct _point {
  unsigned int type: 2;
  int x;
  int y;
} Point;

typedef struct _circle {
  unsigned int type: 2;
  Point point;
  int radius;
} Circle;

typedef struct _triangle {
  unsigned int type: 2;
  Point points[3];
} Triangle;

typedef struct _rectangle {
  unsigned int type: 2; // 11
  Point points[4];
} Rectangle;

typedef union _geo {
  Point point;
  Triangle triangle;
  Circle circle;
  Rectangle rectangle;
} Shape;



// stolen from course website
typedef struct ilst *intlist_t;
intlist_t empty(void);
int isempty(intlist_t lst);
Shape head(intlist_t lst);
intlist_t tail(intlist_t lst);
intlist_t cons(Shape el, intlist_t tl);
intlist_t decons(intlist_t lst, Shape *elp);

struct ilst {
  intlist_t nxt;
  Shape el;
};

int isempty(intlist_t lst)
{
  return lst == NULL;
}

intlist_t empty(void)
{
  return NULL;
}

Shape head(intlist_t lst)
{
  return lst->el;
}

intlist_t tail(intlist_t lst)
{
  return lst->nxt;
}

intlist_t cons(Shape el, intlist_t tl)
{
  intlist_t p = malloc(sizeof(struct ilst));
  if (!p) return NULL;
  p->el = el;
  p->nxt = tl;
  return p;
}
// end

// returns tail, assignes *elp with head
intlist_t decons(intlist_t lst, Shape *elp)
{
  if (elp) *elp = lst->el;
  intlist_t tl = lst->nxt;
  free(lst);
  return tl;
}
// end stolen from course website.

void printlist(intlist_t lst)
{
  if (isempty(lst)) putchar('\n');
  else {
    if ( head(lst).point.type == 0 ) {
      printf("This is a point.\n");
      printf("x: %d y: %d\n", head(lst).point.x, head(lst).point.y );
    } else if ( head(lst).point.type == circle ) {
      printf("This is a Circle.\n");
      printf("x: %d y: %d\n", head(lst).circle.point.x, head(lst).circle.point.y );
      printf("radius: %d \n", head(lst).circle.radius );
    } else if ( head(lst).point.type == triangle ) {
      printf("This is a Triangle.\n");//written_by_denis
      printf("x: %d y: %d\n", head(lst).triangle.points[0].x, head(lst).triangle.points[0].y );
      printf("x: %d y: %d\n", head(lst).triangle.points[1].x, head(lst).triangle.points[1].y );
      printf("x: %d y: %d\n", head(lst).triangle.points[2].x, head(lst).triangle.points[2].y );
    } else if ( head(lst).point.type == rectangle ) {
      printf("This is a Rectangle.\n");
      printf("x: %d y: %d\n", head(lst).rectangle.points[0].x, head(lst).rectangle.points[0].y );
      printf("x: %d y: %d\n", head(lst).rectangle.points[1].x, head(lst).rectangle.points[1].y );
      printf("x: %d y: %d\n", head(lst).rectangle.points[2].x, head(lst).rectangle.points[2].y );
      printf("x: %d y: %d\n", head(lst).rectangle.points[3].x, head(lst).rectangle.points[3].y );
    }
    printlist(tail(lst));
  }
}

void print_array(Shape * shapes, size_t size) {
  for ( int i = 0; i < size; i++ ) {
    if ( shapes[i].point.type == point ) { // point
      printf("shapes[%d] is a point.\n", i);
      printf("x: %d y: %d\n", shapes[i].point.x, shapes[i].point.y );
    } else if ( shapes[i].point.type == circle ) {
      printf("shapes[%d] is a circle.\n", i);
      printf("x: %d y: %d\n", shapes[i].circle.point.x, shapes[i].circle.point.y );
      printf("radius: %d\n", shapes[i].circle.radius );
    } else if ( shapes[i].point.type == triangle ) {
      printf("shapes[%d] is a triangle.\n", i);
      printf("x: %d y: %d\n", shapes[i].triangle.points[0].x, shapes[i].triangle.points[0].y );
      printf("x: %d y: %d\n", shapes[i].triangle.points[1].x, shapes[i].triangle.points[1].y );
      printf("x: %d y: %d\n", shapes[i].triangle.points[2].x, shapes[i].triangle.points[2].y );
    } else if ( shapes[i].point.type == rectangle ) {
      printf("shapes[%d] is a triangle.\n", i);
      printf("x: %d y: %d\n", shapes[i].rectangle.points[0].x, shapes[i].rectangle.points[0].y );
      printf("x: %d y: %d\n", shapes[i].rectangle.points[1].x, shapes[i].rectangle.points[1].y );
      printf("x: %d y: %d\n", shapes[i].rectangle.points[2].x, shapes[i].rectangle.points[2].y );
      printf("x: %d y: %d\n", shapes[i].rectangle.points[3].x, shapes[i].rectangle.points[3].y );
    }
  }
}

void process_input() {
  int figures = -1;
  intlist_t list = NULL;
  printf("How many figures?\n");
  scanf("%d", &figures);
  // printf("figs %d\n", figures);
  printf("Adding %d figures.\n", figures);
  for (int i = 0; i < figures; i++) {
    int type = -1;
    printf("What's the type of the figure? (1 - CIRCLE, 2 - TRIANGLE, 3 - RECTANGLE)\n");
    scanf("%d", &type);
    Shape * shape;
    switch (type) {
      case circle:
        shape = malloc(sizeof(Shape));
        shape->circle.type = circle;
        printf("What's the X and Y of the circle point?\n");
        scanf("%d %d", &shape->circle.point.x, &shape->circle.point.y);
        printf("What's the radius of the circle?\n");
        scanf("%d", &shape->circle.radius);
        printf("Circle added, moving on!\n");
        list = cons(*shape, list);
        free(shape);
        break;
      case triangle:
        shape = malloc(sizeof(Shape));
        shape->triangle.type = triangle;
        printf("What's the X and Y of the first triangle point?\n");
        scanf("%d %d", &shape->triangle.points[0].x, &shape->triangle.points[0].y);

        printf("What's the X and Y of the seccond triangle point?\n");
        scanf("%d %d", &shape->triangle.points[1].x, &shape->triangle.points[1].y);

        printf("What's the X and Y of the third triangle point?\n");
        scanf("%d %d", &shape->triangle.points[2].x, &shape->triangle.points[2].y);

        printf("Triangle added, moving on!\n");
        list = cons(*shape, list);
        free(shape);
        break;
      case rectangle:
        shape = malloc(sizeof(Shape));
        shape->rectangle.type = rectangle;
        printf("What's the X and Y of the first rectangle point?\n");
        scanf("%d %d", &shape->rectangle.points[0].x, &shape->rectangle.points[0].y);

        printf("What's the X and Y of the seccond rectangle point?\n");
        scanf("%d %d", &shape->rectangle.points[1].x, &shape->rectangle.points[1].y);

        printf("What's the X and Y of the third rectangle point?\n");
        scanf("%d %d", &shape->rectangle.points[2].x, &shape->rectangle.points[2].y);

        printf("What's the X and Y of the fourth rectangle point?\n");
        scanf("%d %d", &shape->rectangle.points[3].x, &shape->rectangle.points[3].y);

        printf("Rectangle added, moving on!\n");
        list = cons(*shape, list);
        free(shape);
        break;
      default:
        fprintf(stderr, "Error, invalid type!\n");
        return;
    }
  }

  printf("\nPrinting...\n\n");
  printlist(list);
}

int main() {
  //METHOD !
  Shape shapes[10]; // (Cast to type) (Struct initializer)
  shapes[0].point = (Point) { point, 4, 6 };
  shapes[1].point = (Point) { point, 1, 2};
  shapes[2].point = (Point) { point, 1, 2};
  shapes[3].circle = (Circle) { circle, {point, 1, 1}, 10 };
  shapes[4].circle = (Circle) { circle, {point, 5, 5}, 16 };
  shapes[5].triangle = (Triangle) { triangle, { {point, 1, 1}, {point, 2, 2}, {point, 3, 3} } };
  shapes[6].triangle = (Triangle) { triangle, { {point, 1, 1}, {point, 2, 2}, {point, 3, 9} } };
  shapes[7].circle = (Circle) { circle, {point, 52, 52}, 20 };
  shapes[8].circle = (Circle) { circle, {point, 12, 64}, 36 };
  shapes[9].triangle = (Triangle) { triangle, { {point, 10, 5}, {point, 15, 3}, {point, 21, 7} } };

  //LIST
  // intlist_t l1 = cons(shapes[0], cons(shapes[3], cons(shapes[5], cons(shapes[1], cons(shapes[2],
  // cons(shapes[4], cons(shapes[6], cons(shapes[7], cons(shapes[8], cons(shapes[9], empty()))))))))));
  // printlist(l1);

  //ARRAY
  // print_array(shapes, 10);

  process_input();
  return 0;
}
