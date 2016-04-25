#include <iostream>

using namespace std;
class Person {
  protected:
    string name;
    int age;

  public:
    virtual void getdata() = 0;
    virtual void putdata() = 0;
};

class Professor : public Person {
  static int professors;
  int publications;
  int id;
  public:
    Professor() {
      Professor::professors++;
    }
    ~Professor() {
      Professor::professors--;
    }

  virtual void getdata() {
    cin >> Professor::name;
    cin >> Professor::age;
    cin >> Professor::publications;
    Professor::id = professors;
  }
  virtual void putdata() {
    cout << Professor::name << " " << Professor::age << " "
         << Professor::publications << " " << Professor::id << endl;
  }
};

int Professor::professors = 0;

class Student : public Person {
  static int students;
  int marks[6];
  int id;
  public:
    Student() {
      Student::students++;
    }
    ~Student() {
      Student::students--;
    }

    virtual void getdata() {
      cin >> Student::name;
      cin >> Student::age;
      Student::id = Student::students;
      int mark;
      for (int i = 0; i < 6; i++) {
        cin >> mark;
        Student::marks[i] = mark;
      }
    }
    virtual void putdata() {
      cout << Student::name << " " << Student::age << " ";
      for (int i = 0; i < 6; i++) {
        cout << Student::marks[i] << " ";
      }
      cout << Student::id << endl;
    }
};

int Student::students = 0;

int main() {
  return 0;
}
