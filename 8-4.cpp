#include <functional>
#include <vector>
#include <iostream>
#include <limits>
#include <string>

class String
{
public:
    struct pimp {
        char *ptr;
        int *ref;
        int len;
    } *buf = nullptr;

    String () {
        buf = new pimp;
        buf->ptr = new char('\0');
        buf->ref = new int(1);
        buf->len = 1;
    }

    String (const char* s) {
        buf = new pimp;
        auto it = s;
        for (; *it != '\0'; it++) {}
        buf->len = it - s + 1;
        buf->ptr = new char[buf->len];
        std::copy(s, s + buf->len, buf->ptr);
        buf->ref = new int(1);

    }

    String (const String& a) {
        buf = new pimp;
        buf->ptr = a.buf->ptr;
        (*a.buf->ref)++;
        buf->ref = a.buf->ref;
        buf->len = a.buf->len;

    }

    String& operator=(const String& a) {
        if (buf->ptr == a.buf->ptr) {
            return *this;
        }
        del (this);
        buf = nullptr;

        buf = new pimp;
        buf->ptr = a.buf->ptr;
        (*a.buf->ref)++;
        buf->ref = a.buf->ref;
        buf->len = a.buf->len;

        return *this;
    }

    String (String&& a) {
        del(this);
        buf = a.buf;
        a.buf = nullptr;
    }

    String& operator=(String&& a) {
        del(this);
        buf = a.buf;
        a.buf = nullptr;
        return *this;
    }

    void del(const String* a) {
        if (a->buf == nullptr) return;
        (*a->buf->ref)--;
        if (*a->buf->ref == 0) {
            delete a->buf->ref;
            delete [] a->buf->ptr;
        }
        delete a->buf;
    }

    String operator+=(const String& sr) {

        String t = std::move(*this);

        this->buf = new pimp;
        this->buf->len = t.buf->len + sr.buf->len - 1;
        this->buf->ptr = new char[this->buf->len];
        std::copy(t.buf->ptr, t.buf->ptr + t.buf->len - 1, this->buf->ptr);
        std::copy(sr.buf->ptr, sr.buf->ptr + sr.buf->len, this->buf->ptr + t.buf->len - 1);
        this->buf->ref = new int(1);

        return *this;
    }

    const char operator[](int ind) const {
        return *(this->buf->ptr + ind);
    }

    char& operator[](int ind) {

        String t = std::move(*this);

        this->buf = new pimp;
        this->buf->len = t.buf->len;
        this->buf->ptr = new char[this->buf->len];
        std::copy(t.buf->ptr, t.buf->ptr + t.buf->len, this->buf->ptr);
        this->buf->ref = new int(1);

        return *(this->buf->ptr + ind);
    }

    operator std::string() {
        return buf->ptr;
    }

    ~String() {
        del (this);
    }
};


int main(int argc, char const *argv[]) {
    String s, s2 = "123";
    s = "abc";
std::cout << "tf" << '\n';
    String s1 = s2;
    std::cout << std::string(s1) << std::endl;

    std::cout << std::string(s2) << std::endl;
    const String m = "abacaba";
    std::cout << m[1] << std::endl;
    String n = "mmmm";

    String nn = n;
    (void)nn;
    std::cout << std::string(n) << std::endl;


    return 0;
}
