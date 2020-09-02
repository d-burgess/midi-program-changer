#include "String.h"

std::string String::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

String String::substring(int start) {
    return substr(start);
}

String String::substring(int start, int end) {
    return substr(start, end-start);
}

String String::remove(int start) {
    return erase(start);
}

void String::replace(const String& from, const String& to) {
    ReplaceAll(*this, from, to);
}

int String::indexOf(const char needle) {
    return find(needle);
}
