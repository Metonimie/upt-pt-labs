#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef int (*comp)(const void *, const void *);

typedef struct Student_Tag {
  int grade;
  char name[41];
} Student;

int n = 0;
Student * students;

int sort_grade(Student * a, Student * b) {
  return b->grade - a->grade;
}
int sort_alpha(Student * a, Student * b) {
  return strcmp(a->name, b->name);
}

void initialize_students(unsigned n) {
  students = malloc(sizeof(Student) * n);
}

void fenter_student() {
  if (n == 0) {
    printf("\tN must be bigger than 0, did you gave an N?\n");
    return;
  } else {
    int index = 0;
    for (int i = 0; i < n; i++) {
      printf("Enter student: FIRST NAME_LAST NAME, GRADE. MAX 40 CHARACTERS!\n");
      scanf("%40s %d", (students + index)->name, &(students + index)->grade);
      printf("Student was added successfully.\n");
      printf("Name: %s\n", (students + index)->name);
      printf("Grade: %d\n", (students + index)->grade);
      index += sizeof(Student);
    }
  }
}

void fgive_n() {
  printf("Type your N\n");
  scanf("%d", &n);
}

void display(Student * arr, int n) {
  for (int i = 0; i < n * sizeof(Student); i += sizeof(Student)) {
    printf("Student %s Grade %d\n", arr[i].name, arr[i].grade);
  }
}

int choice() {
  enum { exit_program, give_n, enter_student, display_class_alpha,
    display_class_grade, display_first_three };
  int ch = 0;
  scanf("%d", &ch);
  switch (ch) {
    case exit_program:
      printf("Exiting..\n");
      return 1;
      break;
    case give_n:
      fgive_n();
      initialize_students(n);
      break;
    case enter_student:
      fenter_student();
      break;
    case display_class_alpha:
      assert(n > 0);
      printf("Display according to name\n");
      qsort(students, n, sizeof(students[0]), (comp)sort_alpha);
      display(students, n);
      break;
    case display_class_grade:
      assert(n > 0);
      // printf("%lu %lu \n", sizeof(students), sizeof(students[0]) );
      printf("Display according to grades\n");
      qsort(students, n, sizeof(students[0]), (comp)sort_grade);
      display(students, n);
      break;
    case display_first_three:
      assert(n > 0);
      printf("Display first three according to grades\n");
      qsort(students, n, sizeof(students[0]), (comp)sort_grade);
      display(students, 3);
      break;
    default:
      printf("\tInvalid choice\n");
  }
  return 0;
}

void display_menu() {
  printf("\n");
  printf("0. Exit program\n");
  printf("1. Give N, number of students\n");
  printf("2. Enter the student\n");
  printf("3. Display class in alphabetic order\n");
  printf("4. Display class in creating a top based on grades\n");
  printf("5. Display the first three students according to their grades\n");
  printf("\n");
}

int main() {
  int sgexit = 0;
  while (!sgexit) {
    display_menu();
    sgexit = choice();
  }
  return 0;
}
