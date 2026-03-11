#include "BigInt.h"

#include <iostream>

#include <algorithm>

using namespace std;

BigInt::BigInt () : digits(1,0) , negative(false) {};

BigInt::BigInt(int64_t value){ // 64 bit because long long possible
    negative = (value<0);
    this->digits.clear();
    uint64_t temp;

    if(negative)
    {
      temp= static_cast<uint64_t>(-(value+1))+1; //handles Lint_min overflow
    }
    else
    temp = static_cast<uint64_t>(value);

    digits.push_back(static_cast<uint32_t>(temp)); // lower 32 bits
    uint32_t high=static_cast<uint32_t>(temp >> 32); //higher 32 bits
    //static_cast fixes it at compile time
    if(high)
    {
        digits.push_back(high);
    }
    
};

void BigInt::trim()
{
   while(digits.size()>1 &&  digits.back()==0)
   digits.pop_back(); //trims leading zeroes

   if(digits.size() == 1 && digits[0]==0)
   negative = false; //handles -0
}

int BigInt::compareAbs(const BigInt& a , const BigInt& b)
{
   if(a.digits.size() > b.digits.size()) return 1;
   if(a.digits.size() < b.digits.size()) return -1;

   for(int i=a.digits.size()-1 ; i>=0 ; i--) 
   {
    if(a.digits[i] > b.digits[i]) return 1;
     if(a.digits[i] < b.digits[i]) return -1;
   }
return 0;
}

BigInt BigInt::addAbs(const BigInt& a, const BigInt& b) 
{
    BigInt result;
    result.digits.clear();
    int maxsize= (a.digits.size()> b.digits.size()) ? a.digits.size() : b.digits.size();
    
    uint64_t carry =0;

    for(size_t i = 0 ; i<maxsize ; i++)
    {
        uint64_t ai= (i < a.digits.size()) ? a.digits[i] : 0;
        uint64_t bi= (i < b.digits.size()) ? b.digits[i] : 0;

        uint64_t sum= ai +bi+ carry;

        result.digits.push_back(static_cast<uint32_t>(sum));
        carry=sum >> 32; // the higher 32 bits
    }
    
    if(carry)
    result.digits.push_back(static_cast<uint32_t>(carry)); // the lower 32 bits

    return result;
}

BigInt BigInt::subAbs(const BigInt& a,const BigInt& b)
{
   BigInt result;
   result.digits.clear();

   int64_t borrow = 0;
   for(size_t i=0 ; i<a.digits.size() ; i++)
 {  // |a| >= |b| so final borrrow is never zero
    int64_t ai = a.digits[i];
    int64_t bi = (i < b.digits.size()) ? b.digits[i] : 0;
    int64_t diff = ai - bi - borrow;

    if(diff<0)
    {diff+= BASE;
    borrow=1;}
    else borrow=0;

    result.digits.push_back(static_cast<uint32_t>(diff));
   }
    
   result.trim();
   return result;
   
}

BigInt BigInt::operator+(const BigInt& other) const//operator overloading and compiler sees this as (BigInt* this , BigInt& other)
{
  if(this->negative == other.negative)
  {
    BigInt result = addAbs(*this,other);
    result.negative = this->negative;
    return result;
  }
  int cmp = compareAbs(*this,other);

  if(cmp>=0)
  {
    BigInt result = subAbs(*this,other);
    result.negative = this->negative;
    return result;
  }
  else{
   BigInt result = subAbs(other,*this);
   result.negative = other.negative;
   return result;
  }
}

BigInt BigInt::operator-(const BigInt& other) const
{
    BigInt temp = other;
    temp.negative = !other.negative;
    return *this + temp ; //compiler sees this as this->operator+(other) a-b=a+(-b)
}

BigInt BigInt::operator*(const BigInt& other) const
{
  BigInt result;
  result.digits.assign(this->digits.size()+other.digits.size(),0);

  for(size_t i=0 ; i<this->digits.size() ; i++)
  {
    uint64_t carry =0;
    for(size_t j=0 ; j<other.digits.size() ; j++)
    {
      uint64_t current = (uint64_t)this->digits[i] * other.digits[j] + result.digits[i+j] + carry; // previosuly stored value + current + carry

      result.digits[i+j]=static_cast<uint32_t>(current);
      carry = current >> 32;
    }

    int pos = i + other.digits.size(); // the next block for carry after last j
    while(carry) // while because this carry + the value in that block can itself cause a new carry
    {
      uint64_t temp = result.digits[pos] + carry;
      result.digits[pos] = static_cast<uint32_t>(temp);
      carry = temp >> 32;
      pos++;
    }   
  }
      result.negative = (this->negative != other.negative);
      result.trim();
      return result;
}

BigInt::BigInt(const string& str) : digits(1,0), negative(false)
{
    size_t start = 0;
    bool isNegative = false;

    if (str[0] == '-')
    {
        isNegative = true;
        start = 1;
    }

    for (size_t i = start; i < str.size(); i++)
    {
        *this = (*this * BigInt(10)) + BigInt(str[i] - '0');
    }

    negative = isNegative;  // restore sign after magnitude built
    trim();
}


string BigInt::tostring() const{
  if(this->digits.size() == 1 && this->digits[0]==0)
  return "0";
 
  string result;
  BigInt temp = *this;
  temp.negative = false;

  while(!(temp.digits.size() == 1 && temp.digits[0] == 0))
  { //base 100 example logic exteded to base 2^32 
    // long division because 1024 bits ant be done in one cpu operation
      uint64_t remainder = 0;

      for(int i = temp.digits.size() - 1; i>=0 ; i--)
      {
             uint64_t current = (remainder << 32) + temp.digits[i];
             temp.digits[i] = current/10;
             remainder = current%10;
      }
       //for loop gave only one digit so need to iterate the whole bigint unutll it becomes 0 for all digits
      result.push_back('0'+remainder);
      temp.trim();
    }
    if(this->negative)
     result.push_back('-');

     reverse(result.begin(),result.end());

return  result;
}