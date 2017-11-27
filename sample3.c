#include<stdio.h>
#include<stdlib.h>
#include<string.h>

double vald;
float  valf;
char   valc;
struct D myD;

struct mystruct {
   float f;
   double d;
   long l;
   short s;
   int i;
   char c;
};


	/* function prototype */
float total (float,float) ;
float product(float , float  ) ;

	/* class with overloaded methods */


	// simple function outside of a class
float product(float a, float b) {
   return( a*b );
}


	/* class with no methods */


	/* class with no variables */









float total(float a, float b){
   return(a +b );
}

struct  A {
int a, aa;
int aaa;
	/* add parameters */
   void (*Aaddii)(struct A*, int, int);

	// add class variables
   void (*Aadd)(struct A*);

};

void Aaddii(struct A * theStruct, int a, int aa) {
int aaa;
aaa = theStruct->aaa;
      printf("%d\n", a+ aa);
   
theStruct->aaa = aaa;
}

void Aadd(struct A * theStruct) {
int a;
int aa;
int aaa;
a = theStruct->a;
aa = theStruct->aa;
aaa = theStruct->aaa;
      printf("%d\n", a + aa);
   
theStruct->a = a;
theStruct->aa = aa;
theStruct->aaa = aaa;
}


void constructorA(struct A * sPointer) {
 sPointer->Aaddii = &Aaddii;
 sPointer->Aadd = &Aadd;
}

struct  B {
long b;
short c;
};


struct  C{
   void (*Caddii)(struct C*, int, int);

	// create a class inside the method
   void (*CmakeObject)(struct C*);
};

void Caddii(struct C * theStruct, int a, int b) {
      printf("%d\n", a+b);
   
}

void CmakeObject(struct C * theStruct) {
      int a, b;
      struct B myB1;
         a = 7;
         myB1.b = 3;
         myB1.c = 10;
         printf("output should be 20: %d", a + (int)myB1.b +myB1.c);

		/* call the method inside the class */

         printf("output should be 3: ");
         a = 1; b = 2;
         Caddii(theStruct, a, b);
}


void constructorC(struct C * sPointer) {
 sPointer->Caddii = &Caddii;
 sPointer->CmakeObject = &CmakeObject;
}

struct  D
{
int a,b, c;
   void (*Dsetai)(struct D*, int);

   void (*Dsetbi)(struct D*, int);

   void (*Dsetci)(struct D*, int);

   int (*Dgeta)(struct D*);

   int (*Dgetb)(struct D*);

   int (*Dgetc)(struct D*);

   void (*Dmath)(struct D*);

};

void Dsetai(struct D * theStruct, int val ) {
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
      a = val;
   
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
}

void Dsetbi(struct D * theStruct, int val ){
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
      b = val;
   
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
}

void Dsetci (struct D * theStruct, int val) {
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
      c = val;
   
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
}

int Dgeta(struct D * theStruct) {
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
      
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
return(a);
   
}

int Dgetb (struct D * theStruct){
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
      
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
return( b);
   
}

int Dgetc (struct D * theStruct) {
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
      
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
return( c );
   
}

void Dmath(struct D * theStruct) {
int a;
int b;
int c;
a = theStruct->a;
b = theStruct->b;
c = theStruct->c;
   int result;
      result = a + b* c /a;
   
theStruct->a = a;
theStruct->b = b;
theStruct->c = c;
}


void constructorD(struct D * sPointer) {
 sPointer->Dsetai = &Dsetai;
 sPointer->Dsetbi = &Dsetbi;
 sPointer->Dsetci = &Dsetci;
 sPointer->Dgeta = &Dgeta;
 sPointer->Dgetb = &Dgetb;
 sPointer->Dgetc = &Dgetc;
 sPointer->Dmath = &Dmath;
}

struct 
E {
   float
      abc , p , aber;
      float sabre, dogs ;

   struct mystruct *(*Emethod)(struct E*);
;
   void (*Estring1c)(struct E*, char[]);

   void (*Estring2c)(struct E*, char*);

};

struct mystruct *  Emethod(struct E * theStruct)
   {
float abc;
float p;
float aber;
float sabre;
float dogs;
abc = theStruct->abc;
p = theStruct->p;
aber = theStruct->aber;
sabre = theStruct->sabre;
dogs = theStruct->dogs;
   struct mystruct *ptr;
      int z = 3;
      z++;
      printf("output should be 4 1.200000 : %d %f\n", z, abc);

      ptr = malloc(sizeof(struct mystruct));
      
theStruct->abc = abc;
theStruct->p = p;
theStruct->aber = aber;
theStruct->sabre = sabre;
theStruct->dogs = dogs;
return(ptr);
   
}

void Estring1c(struct E * theStruct, char str[]) {
float abc;
float p;
float aber;
float sabre;
float dogs;
abc = theStruct->abc;
p = theStruct->p;
aber = theStruct->aber;
sabre = theStruct->sabre;
dogs = theStruct->dogs;
      printf("output should be abcd: %s\n", str);
   
theStruct->abc = abc;
theStruct->p = p;
theStruct->aber = aber;
theStruct->sabre = sabre;
theStruct->dogs = dogs;
}

void Estring2c(struct E * theStruct, char *str) {
float abc;
float p;
float aber;
float sabre;
float dogs;
abc = theStruct->abc;
p = theStruct->p;
aber = theStruct->aber;
sabre = theStruct->sabre;
dogs = theStruct->dogs;
      printf("output should be efgh: %s\n", str);
   
theStruct->abc = abc;
theStruct->p = p;
theStruct->aber = aber;
theStruct->sabre = sabre;
theStruct->dogs = dogs;
}


void constructorE(struct E * sPointer) {
 sPointer->Emethod = &Emethod;
 sPointer->Estring1c = &Estring1c;
 sPointer->Estring2c = &Estring2c;

 sPointer->abc = 1.2;
 sPointer->p = 2.3;
 sPointer->dogs = 1.7;
}

int main(int argc, char *argv[]) {
constructorD(&myD);
struct A myA;
constructorA(&myA);
struct B myB1,myB2, myB3;
struct C myC, anotherC, againwithC;
constructorC(&myC);
constructorC(&anotherC);
constructorC(&againwithC);
int x, y;
char str[50];
struct mystruct *sptr;
char *name;
struct E myE;
constructorE(&myE);
int america;

  america = 2/2;
  america = 6 / 3;
  america = 12/ 6;

   printf("Hello \" there\n");
   strcpy(str, "A string containing the word class may be tricky");

   sptr = malloc(sizeof(struct mystruct));

   name = malloc(sizeof(char)*100);

   myA.a = 5;
   myA.aa = 10;
   printf("output should be 15: ");
   myA.Aadd(&myA);

   x = 4;
   y = 5;
   printf("output should be 9: ");
   myA.Aaddii(&myA, x, y);

   myB3.b = 10;
   myB3.c = 11;
   printf("output should be 10, 11: %ld %d", myB3.b, myB3.c);
   myC.CmakeObject(&myC) ;

	// comment with a keyword in it class A nota {

   x = 11;
   myD.Dsetai(&myD, x);
   printf("output should be 11: %d\n", myD.Dgeta(&myD));

   free(sptr);
   sptr = myE.Emethod(&myE);
   strcpy(str, "abcd");
   myE.Estring1c(&myE, str);
   strcpy(str, "efgh");
   myE.Estring2c(&myE, str);
}


