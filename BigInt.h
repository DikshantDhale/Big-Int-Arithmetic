#ifndef BIGINT_H
#define BIGINT_H

#include <vector>
#include <string>
#include <iostream>
#include <cstdint> // so that size remains same throghout different OS and machines

using namespace std;

class BigInt{

    private:
    static const uint64_t BASE = (1ULL << 32); // 1ULL so that 1 is 64bit
    vector<uint32_t> digits;  //each element in a vector is 32bit unsigned integer
    bool negative;

    void trim();
    static int compareAbs(const BigInt& a , const BigInt& b);
    static BigInt addAbs(const BigInt& a , const BigInt& b);
    static BigInt subAbs(const BigInt& a , const BigInt& b);

    public:
    BigInt(); // handles default
    BigInt(int64_t value); // handles upto long long 
    BigInt(const string& decimal);// handels beyond long long

    BigInt operator+(const BigInt& other) const; //signed addition
    BigInt operator-(const BigInt& other) const; //subtraction
    BigInt operator*(const BigInt& other) const; //multiplication

    string tostring() const;
};
#endif