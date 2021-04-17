#include <functional>
#include <vector>
#include <iostream>
#include <limits>
#include <string>

struct str {
    int len;
    char *ptr;

    str () {
        len = 1;
        ptr = new char[1];
        *ptr = '\0';
    }

    str (const char* s) {
        auto it = s;
        for (; *it != '\0'; it++) {}
        len = it - s + 1;
        ptr = new char[len];
        std::copy(s, s + len, ptr);
        //std::cout << ptr << '\n';
    }

    operator std::string() {
        return ptr;
    }

    void del() {
        delete [] ptr;
    }

    str operator+=(const str& sr) {

        char* t = new char[sr.len + len - 1];
        std::copy(ptr, ptr + len - 1, t);
        std::copy(sr.ptr, sr.ptr + sr.len, t + len - 1);
        delete [] ptr;
        ptr = t;
        len = len + sr.len - 1;

        return *this;
    }

    friend str operator+(str lhs, const str& rhs) {
        lhs += rhs;
        return lhs;
    }

    const char operator[](int ind) const {
        return *(ptr + ind);
    }

    char& operator[](int ind) {
       return *(ptr + ind);
   }

    ~str () {
        delete [] ptr;
    }


};

class String
{
public:
    struct sh_ptr {
        int ref;
        str* s;
    } *ptr;

    String () {
        ptr = new sh_ptr;
        ptr->ref = 1;
        ptr->s = new str("\0");
    }

    String (const char* a) {
        ptr = new sh_ptr;
        ptr->ref = 1;
        std::cout << std::string(str(a)) << '\n';
        ptr->s = new str(a);
        std::cout << ptr->s->ptr << '\n';
    }

    String (const String& a) {
        ptr = a.ptr;
        ptr->ref++;
    }

    String& operator=(const String& a) {
        if (ptr == a.ptr) {
            return *this;
        }

        del (this);
        ptr = nullptr;

        ptr = a.ptr;
        ptr->ref++;

        return *this;
    }

    String (String&& a) {
        del(this);
        ptr = a.ptr;
        a.ptr = nullptr;
    }

    String& operator=(String&& a) {
        del(this);
        ptr = a.ptr;
        a.ptr = nullptr;
        return *this;
    }

    void del(const String* a) {
        if (a->ptr == nullptr) return;
        a->ptr->ref--;
        if (a->ptr->ref == 0) {
            (*a->ptr->s).del();
            delete a->ptr->s;
            delete a->ptr;
        }
    }

    String operator+=(const String& sr) {

        str sn = *ptr->s + *sr.ptr->s;

        del(this);
        ptr = new sh_ptr;
        ptr->ref = 1;
        ptr->s = &sn;

        return *this;
    }

    const char operator[](int ind) const {
        return (*ptr->s)[ind];
    }

    char& operator[](int ind) {

        str sn = *ptr->s;

        del(this);
        ptr = new sh_ptr;
        ptr->ref = 1;
        ptr->s = &sn;

        return (*ptr->s)[ind];
    }

    operator std::string() {
        std::cout << ptr->s->ptr << '\n';
        return std::string(*ptr->s);
    }

    ~String() {
        del (this);
    }
};

int main(int argc, char const *argv[]) {
    String c, a("abc");
String b = a; // в памяти хранится "abc" в одном экземпляре
c = a;
b += "aaa"; // для b создался новый "массив" символов.
std::cout << "zh" << '\n';
//c[1] = 'f'; // a не изменилось
//std::cout << std::string(a);
    return 0;
}
