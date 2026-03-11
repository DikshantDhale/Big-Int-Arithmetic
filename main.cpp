#include "BigInt.h"
#include <iostream>
#include <cmath>
using namespace std;

#include <string>
int main()
{
   string s1;
   string s2;


   cout<<"Enter first large integer\n";
   cin>> s1;

   cout<<"Enter second large integer\n";
   cin >> s2;

   BigInt a(s1);

   BigInt b(s2);

   cout<<"addition : "<< (a+b).tostring() << endl; // temporary oject created which calls tostring function
   cout<<"subtraction : "<<(a-b).tostring()<<endl;
   cout<<"Multiplication : "<< (a*b).tostring()<<endl;
// cout << "A: " << a.tostring() << endl;
//     cout << "B: " << b.tostring() << endl;
   return 0;
}