#Simplified C++ to C Converter

This is a program which converts a simplified version of C++ into a C program which can then be compiled. It was created for a school assignment.

##Program Limitations
- Must be in Simplified C++ Form
- 50 variables in a class
- 75 functions in a class
- class name of 75 characters
- function within a class name of 50 characters

##Transformation Specifics

**Class to a Struct**
class A {
   ...
}

*would become:*

struct A {
   ...
}


```c++
class A {
   int a;

   int add(int b, int c) {
      return (b+c);
   }
}
```
 
*would be converted to:*
 
```c
struct A {
   int a;
   int (*Aaddii)();
}

   int Aaddii(int b, int c) {
      return (b+c);
   }
```


*If a class is defined as:*

```c++
class A myAClass;
```

*then the program will replace it with:*

```c
struct A myAClass;
constructorA(...);
```

**Renaming Methods in the Class**

```c++
class A {
   ...
   add(int x) {
      ...};
}

class B {
   ...
   add(int y) {
      ...};
}
```

*would become*

```c
struct A {
   ...
}

   Aadd(int x) {
      ...};

struct B {
   ...
}

   Badd(int y) {
      ...};
```

**Class Scope**
*printf statements are to show what variables can be accessed*

```c++
int a;

class B {
   int b;

   int add(int c) {
      int d;
      printf("%d\n", a+b+c+d);
   }
}

int main() {
class B myB;

   myB.add(3);
}
```

When this is converted to C the program would look like this:

```c
int a;

struct B {
   int b;
   int (*Baddi)();
}

   int Baddi(..., int c) {
      int d;
      printf("%d\n", a+b+c+d);
   }

int main() {
struct B myB;
constructorB(...);

   myB.Baddi(3);
}
```

**Method Overloading**

```c++
int main() {
class A myClass;
   ...
   myClass.add(int x, int x);
   ...
}
```

*would change to*

```c
int main() {
struct A myClass;
constructorA();
   ...
   myClass.Aaddii(int x, int x);
   ...
}
```