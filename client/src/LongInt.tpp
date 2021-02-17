#include "LongInt.h"

/************/
template <typename T>
my::LongInt<T>::LongInt() {

}

/************/
template <typename T>
my::LongInt<T>::LongInt(const std::string &str) {
    setVal(str);
}

/************/
template <typename T>
my::LongInt<T>::LongInt(const std::vector<T> v) {
    for (auto it : v) {
        m_data.push_back(it);
    }
}

/************/
template <typename T>
my::LongInt<T>::LongInt(const LongInt<T> &num) {
    m_data.clear();
    for (auto it = num.m_data.begin(); it != num.m_data.end(); it++) {
        m_data.push_back(*it);
    }
}

/************/
template <typename T>
my::LongInt<T>::~LongInt() {
    m_data.clear();
}

/************/
template <typename T>
void my::LongInt<T>::clear() {
    m_data.clear();
}

/************/
template <typename T>
void my::LongInt<T>::setVal(const std::string &str) {
    uint64_t base = std::pow<uint8_t, uint64_t>(2, 8*sizeof(T));
    std::string whole;
    uint64_t remainder;

    if (m_data.size() > 0) {
        m_data.clear();
    }

    whole = str;

    T dat;
    std::memset(&dat, 0, sizeof(T));
    do {
        remainder = mod(whole, base);
        whole = div(whole, base);
        std::memcpy(&dat, &remainder, sizeof(T));
        m_data.push_back(dat);
    } while (isMore(whole, std::to_string(base)));
    remainder = mod(whole, base);
    std::memcpy(&dat, &remainder, sizeof(T));
    m_data.push_back(dat);

    if (*(m_data.end() - 1) == 0)
        m_data.erase(m_data.end() - 1);

    // Revers data
    auto it1 = m_data.begin();
    auto it2 = m_data.end() - 1;
    while (it1 < it2) {
        std::iter_swap(it1, it2);
        it1++;
        it2--;
    }
}

/************/
template <typename T>
const std::vector<T>& my::LongInt<T>::getDataVec() const {
    return m_data;
}

/************/
template <typename T>
size_t my::LongInt<T>::getSize() const {
    return m_data.size();
}

/************/
template <typename T>
size_t my::LongInt<T>::getSizeInBytes() const {
    return m_data.size()*sizeof(T);
}

/************/
template <typename T>
void my::LongInt<T>::printSrc() const {
    uint64_t num {0};
    for (auto it : m_data) {
        std::memcpy(&num, &it, sizeof(T));
        std::cout << "0x" << std::hex << num << std::dec << " ";
    }
    std::cout.flush();
}

/************/
template <typename T>
my::LongInt<T>& my::LongInt<T>::operator = (const LongInt<T> &num) {
    m_data.clear();
    for (auto it = num.m_data.begin(); it != num.m_data.end(); it++) {
        m_data.push_back(*it);
    }

    return *this;
}

/************/
template <typename T>
my::LongInt<T>& my::LongInt<T>::operator ++ () {
    uint64_t base = std::pow<uint8_t, uint64_t>(2, 8*sizeof(T));
    uint64_t buf {0};

    if (!m_data.empty()) {
        for (int i = m_data.size() - 1; i >= 0; i--) {
            memcpy(&buf, &m_data[i], sizeof(T));
            if (buf != base - 1) {
                m_data[i]++;
                break;
            } else {
                m_data[i] = 0;
                if (i == 0) {
                    m_data.insert(m_data.begin(), 1);
                    break;
                }
            }
        }
    }

    return *this;
}

/************/
template <typename T>
my::LongInt<T>& my::LongInt<T>::operator ++ (int) {
    uint64_t base = std::pow<uint8_t, uint64_t>(2, 8*sizeof(T));
    uint64_t buf {0};

    if (!m_data.empty()) {
        for (int i = m_data.size() - 1; i >= 0; i--) {
            memcpy(&buf, &m_data[i], sizeof(T));
            if (buf != base - 1) {
                m_data[i]++;
                break;
            } else {
                m_data[i] = 0;
                if (i == 0) {
                    m_data.insert(m_data.begin(), 1);
                    break;
                }
            }
        }
    }

    return *this;
}

/************/
template <typename T>
my::LongInt<T>& my::LongInt<T>::operator += (uint64_t num) {
    std::vector<T> lNum;
    lNum.resize(8/sizeof(T));
    memcpy(lNum.data(), &num, sizeof(num));

    // Revers data
    auto lnit1 = lNum.begin();
    auto lnit2 = lNum.end() - 1;
    while (lnit1 < lnit2) {
        std::iter_swap(lnit1, lnit2);
        lnit1++;
        lnit2--;
    }

    m_data = add(m_data, lNum);

    return *this;
}

/************/
template <typename T>
my::LongInt<T>& my::LongInt<T>::operator += (LongInt<T> &num) {
    m_data = add(m_data, num.m_data);

    return *this;
}

/************/
template <typename T>
bool my::LongInt<T>::operator == (LongInt<T> &num) {
    if (m_data.size() == num.m_data.size()) {
        auto it1 = m_data.begin();
        auto it2 = num.m_data.begin();

        while (it1 != m_data.end()) {
            if (*it1 != *it2)
                return false;
            it1++;
            it2++;
        }

        return true;
    }

    return false;
}

/************/
template <typename T>
bool my::LongInt<T>::operator != (LongInt<T> &num) {
    if (m_data.size() == num.m_data.size()) {
        auto it1 = m_data.begin();
        auto it2 = num.m_data.begin();

        while (it1 != m_data.end()) {
            if (*it1 != *it2)
                return true;
            it1++;
            it2++;
        }

        return false;
    }

    return true;
}

/************/
template <typename T>
bool my::LongInt<T>::operator > (LongInt &num) {
    if (m_data.size() > num.m_data.size())
        return true;

    if (m_data.size() < num.m_data.size())
        return false;

    auto it1 = m_data.begin();
    auto it2 = num.m_data.begin();
    while (it1 != m_data.end()) {
        if (*it1 > *it2) return true;
        if (*it1 < *it2) return false;
    }

    return false;
}

/************/
template <typename T>
bool my::LongInt<T>::operator < (LongInt &num) {
    if (m_data.size() > num.m_data.size())
        return false;

    if (m_data.size() < num.m_data.size())
        return true;

    auto it1 = m_data.begin();
    auto it2 = num.m_data.begin();
    while (it1 != m_data.end()) {
        if (*it1 > *it2) return false;
        if (*it1 < *it2) return true;
    }

    return false;
}

/************/
template <typename T>
bool my::LongInt<T>::isMore(const std::string &str1, const std::string &str2) {
    auto it1 = str1.begin();
    auto it2 = str2.begin();

    if (str1.size() == str2.size()) {
        if ((*it1) > (*it2)) {
            return true;
        } else if ((*it1) < (*it2)) {
            return false;
        }

        it1++;
        it2++;
    }

    if (str1.size() > str2.size())
        return true;

    return false;
}

/************/
template <typename T>
bool my::LongInt<T>::isLess(const std::string &str1, const std::string &str2) {
    auto it1 = str1.begin();
    auto it2 = str2.begin();

    if (str1.size() == str2.size()) {
        if ((*it1) > (*it2)) {
            return false;
        } else if ((*it1) < (*it2)) {
            return true;
        }

        it1++;
        it2++;
    }

    if (str1.size() > str2.size())
        return false;

    return false;
}

/************/
template <typename T>
bool my::LongInt<T>::isEqu(const std::string &str1, const std::string &str2) {
    auto it1 = str1.begin();
    auto it2 = str2.begin();

    if (str1.size() == str2.size()) {
        if ((*it1) != (*it2)) {
            return false;
        }

        it1++;
        it2++;
    }

    return true;
}

/************/
template <typename T>
std::vector<T> my::LongInt<T>::add(const std::vector<T> &num1, const std::vector<T> &num2) {
    uint64_t base = std::pow<uint8_t, uint64_t>(2, 8*sizeof(T));
    uint8_t care {0};
    std::vector<T> res;

    if (num2.size() > num1.size()) {
        res.resize(num2.size());
    } else {
        res.resize(num1.size());
    }

    auto it = res.end() - 1;
    auto it1 = num1.end() - 1;
    auto it2 = num2.end() - 1;

    while (it != res.begin() - 1) {
        uint64_t it1_val {0};
        uint64_t it2_val {0};

        it1_val = *it1;
        it2_val = *it2;
        // Left to fill;
        uint64_t leftToFill = base - it1_val;

        if (leftToFill <= it2_val + care) {
            *it = it2_val - leftToFill + care;
            care = 1;
        } else {
            *it = it1_val + it2_val + care;
            care = 0;
        }

        it--;
        it1--;
        it2--;

        if (it == res.begin() - 1 && care) {
            res.insert(res.begin(), 1);
            break;
        }
    }

    // Erase zeros
    int zerCount = 0;
    for (auto rit : res) {
        if (rit != 0)
            break;
        zerCount++;
    }

    if (res.size() > zerCount) {
        res.erase(res.begin(), res.begin() + zerCount);
    } else {
        res.clear();
        res.push_back(0);
    }

    return res;
}

/************/
template <typename T>
std::vector<T> my::LongInt<T>::mul(const std::vector<T> &num1, const std::vector<T> &num2) {
    uint64_t base = std::pow<uint8_t, uint64_t>(2, 8*sizeof(T));
    uint64_t care {0};
    std::vector<T> res;
    res.resize(num1.size() + num2.size());

    int i = res.size() - 1;
    int i2 = num2.size() - 1;
    while (i2 >= 0) {
        care = 0;
        i = res.size() - (num2.size() - i2 - 1) - 1;
        int i1 = num1.size() - 1;
        while (i1 >= 0) {
            res[i] += care;
            care = (num2[i2]*num1[i1] + res[i])/base;
            res[i] = (num2[i2]*num1[i1] + res[i])%base;
            i--;
            i1--;
            if (i1 < 0)
                res[i] += care;
        }
        i2--;
    }

    // Erase zeros
    int zerCount = 0;
    for (auto rit : res) {
        if (rit != 0)
            break;
        zerCount++;
    }

    if (res.size() > zerCount) {
        res.erase(res.begin(), res.begin() + zerCount);
    } else {
        res.clear();
        res.push_back(0);
    }

    return res;
}

/************/
template <typename T>
std::vector<T> my::LongInt<T>::pow(const std::vector<T> &num, const unsigned int p) {
    std::vector<T> res;

    if (p > 1) {
        res = num;
        for (int i = 1; i < p; i++) {
            res = mul(res, num);
        }
    } else if (p == 1){
        res = num;
    } else {
        res.push_back(1);
    }

    return res;
}

/************/
template <typename T>
std::string my::LongInt<T>::div(const std::string &decStringNum, uint64_t base) {
    std::string out;
    if (!decStringNum.empty()) {
        auto it = decStringNum.begin();
        uint64_t num;
        uint64_t whole;
        uint64_t remainder;

        num = (*it) - '0';
        bool isMeanNum = false;
        while (it != decStringNum.end()) {
            if (++it == decStringNum.end()) {
                if (out.size() == 0)
                    out.push_back(num/base + '0');
                return out;
            }

            num *= 10;
            num += (*it) - '0';

            whole = num/base;
            if (!isMeanNum) {
                if (whole != 0) {
                    out.push_back(whole + '0');
                    isMeanNum = true;
                }
            } else {
                out.push_back(whole + '0');
            }
            remainder = num%base;
            num = remainder;
        }
    }

    return out;
}

/************/
template <typename T>
uint64_t my::LongInt<T>::mod(const std::string &decStringNum, uint64_t base) {
    uint64_t remainder {0};
    if (!decStringNum.empty()) {
        auto it = decStringNum.begin();
        uint64_t num;
        uint64_t whole;

        num = (*it) - '0';
        while (it != decStringNum.end()) {

            if (++it == decStringNum.end()) {
                remainder = num%base;
                return remainder;
            }
            num *= 10;
            num += (*it) - '0';

            whole = num/base;
            remainder = num%base;
            num = remainder;
        }
    }

    return remainder;
}
