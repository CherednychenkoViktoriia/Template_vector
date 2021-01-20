#define _OPEN_SYS_ITOA_EXT
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

class String
{
public:
    String() {}

    String(const char* str) 
    {
        std::cout << "String(const char *const c_string)\n";
        m_size = strlen(str) + 1;
        m_string = new char[m_size];
        strcpy_s(m_string, m_size, str);
    }

    String(int value)
    {
        std::cout << "String(int)\n";
        int number = value;
        m_size = 10;
        m_string = new char[m_size];
        snprintf(m_string, m_size, "%ld", number);
    }

    String(long& value)
    {
        std::cout << "String(long)\n";
        long number = value;
        m_size = 10;
        m_string = new char[m_size];        
        _ltoa_s(number, m_string, m_size, 10);
    }

    String(float& value)
    {
        std::cout << "String(float)\n";
        float number = value;
        m_size = 50;
        m_string = new char[m_size];
        snprintf(m_string, m_size, "%f", number);
    }

    String(double& value)
    {
        std::cout << "String(double)\n";
        double number = value;
        m_size = 50;
        m_string = new char[m_size];
        snprintf(m_string, m_size, "%f", number);
    }

    String(const char* str, size_t trueSize)
    {
        std::cout << "String(const char *const c_string, size_t)\n";
        m_size = trueSize + 1;
        m_string = new char[m_size];
        strcpy_s(m_string, trueSize, str);
    }

    String(const String& other)
    {
        std::cout << "String(const String& other)\n";
        m_string = new char[other.m_size];
        strcpy_s(m_string, other.m_size, other.m_string);
        m_size = other.m_size;
    }

    String(String&& other) noexcept
    {
        std::cout << " String(String&& other)\n";
        m_string = other.m_string;
        m_size = other.m_size;
        other.m_string = nullptr;
        other.m_size = 0;
    }

    String& operator=(const String& other)
    {
        std::cout << " operator=(const String& other)\n";
        m_string = new char[other.m_size];
        strcpy_s(m_string, other.m_size, other.m_string);
        m_size = other.m_size;
        return *this;
    }
    String& operator=(String&& other) noexcept
    {
        std::cout << " operator=(String&& other)\n";
        m_string = other.m_string;
        m_size = other.m_size;
        other.m_string = nullptr;
        other.m_size = 0;
        return *this;
    }
    String& operator+(const String& other) {
        char* tmpStr = m_string;
        m_string = new char[m_size + other.m_size];
        strcpy_s(m_string, m_size, tmpStr);
        strcpy_s(m_string + m_size - 1, other.m_size, other.m_string);
        delete[] tmpStr;
        m_size += other.m_size;
        return *this;
    }
    size_t GetStringSize(String&& other) {
        size_t sizeOfString = sizeof(other)/sizeof(char);
        return sizeOfString;
    }
    ~String() noexcept
    {
        std::cout << " ~String()\n";
        if (m_string) {
            delete[] m_string;
            m_string = nullptr;
        }
    }

private:
    char* m_string = nullptr;    
    size_t m_size = 0;
};

template <class T>
class Vector {
public:
    Vector() :
    m_vectorSize(0),
    m_capacity(10),
    m_data(new T[m_capacity])
    {}    

    Vector(Vector&& vt) noexcept:
    m_vectorSize(vt.m_vectorSize),
    m_capacity(vt.m_capacity),
    m_data(new T[vt.m_capacity])
    {    
        for (size_t i = 0; i < m_vectorSize; ++i)
        {
            m_data[i] = std::move(vt.m_data[i]);
        }
        vt.m_data = nullptr;
        vt.m_vectorSize = 0;
        vt.m_capacity = 0;
    }

    Vector<T>& operator=(Vector&& vt) noexcept
    {
        std::cout << " Vector(Vector&& vt)\n";
        m_vectorSize = vt.m_vectorSize;
        m_capacity = vt.m_capacity;
        m_data = new T[vt.m_capacity];
        for (size_t i = 0; i < m_vectorSize; ++i)
        {
            m_data[i] = std::move(vt.m_data[i]);
        }
        vt.m_data = nullptr;
        vt.m_vectorSize = 0;
        vt.m_capacity = 0;
        return *this;
    }
        
    ~Vector() noexcept
    {
        std::cout << "  ~Vector\n";
        if (m_data) {
            delete[] m_data;
            m_data = nullptr;
        }
    }

    template<typename... U>    
    void EmplaceBack(U&&... value) {
        ++m_vectorSize;        
        if (m_vectorSize > m_capacity) {
            Realloc();
        }
        new (&m_data[m_vectorSize - 1]) T(std::forward<U>(value)...);
    }
       
private:
    void Realloc() {
        if (m_capacity < 5) {
            m_capacity = 10;
        }
        else {
            m_capacity = static_cast<size_t>(m_capacity * 1.5);
        }
        auto data = m_data;
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_vectorSize; ++i) {
            m_data[i] = std::move(data[i]);
        }
        delete[] data;        
    }
    
private:
    size_t m_vectorSize = 0;
    size_t m_capacity = 0;
    T* m_data = nullptr;
};

int main()
{        
    String str1("First");
    Vector<String> vt1;
    vt1.EmplaceBack(str1);
    vt1.EmplaceBack(String("Second"));
    vt1.EmplaceBack("Third");
    vt1.EmplaceBack("Hello vars", 12);
    vt1.EmplaceBack(111111);  
    long myLong = 33000;
    vt1.EmplaceBack(myLong);
    float myFloat = 8888888888.4444;
    vt1.EmplaceBack(myFloat);
    double myDouble = 123412341234.123456789;
    vt1.EmplaceBack(myDouble);
}
