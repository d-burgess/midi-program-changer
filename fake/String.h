/*

String

*/

#ifndef __ARDUINO__STRING__
#define __ARDUINO__STRING__

#include <string>

class String : public std::string {
    private:
        std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
    public:
        String() {}
        String(const String& string) : String(string.c_str()) {}
        String(const std::string& string) : std::string(string) {}
        String(const char* string) : std::string(string) {}

        String substring(int start);
        String substring(int start, int end);
        String remove(int start);
        void replace(const String& from, const String& to);
        int indexOf(const char needle);
};

#endif