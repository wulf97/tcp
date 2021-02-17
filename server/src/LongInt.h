#ifndef LONGINT_H
#define LONGINT_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cinttypes>
#include <cmath>
#include <algorithm>

namespace my
{
template <typename T>
class LongInt {
public:
    LongInt();
    LongInt(const std::string &str);
    LongInt(const std::vector<T> v);
    LongInt(const LongInt &num);
    virtual ~LongInt();

    void clear();
    void setVal(const std::string &str);
    const std::vector<T>& getDataVec() const;
    size_t getSize() const;
    size_t getSizeInBytes() const;
    void printSrc() const;

    LongInt& operator = (const LongInt &num);
    LongInt& operator ++ ();
    LongInt& operator ++ (int);
    LongInt& operator += (uint64_t num);
    LongInt& operator += (LongInt &num);
    bool operator == (LongInt &num);
    bool operator != (LongInt &num);
    bool operator > (LongInt &num);
    bool operator < (LongInt &num);

private:
    static bool isMore(const std::string &str1, const std::string &str2);
    static bool isLess(const std::string &str1, const std::string &str2);
    static bool isEqu(const std::string &str1, const std::string &str2);
    static std::vector<T> add(const std::vector<T> &num1, const std::vector<T> &num2);
    static std::vector<T> mul(const std::vector<T> &num1, const std::vector<T> &num2);
    static std::vector<T> pow(const std::vector<T> &num, const unsigned int p);
    static std::string div(const std::string &decStringNum, uint64_t base);
    static uint64_t mod(const std::string &decStringNum, uint64_t base);

private:
    std::vector<T> m_data;
};
}

#include "LongInt.tpp"

#endif // LONGINT_H
