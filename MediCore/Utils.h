#ifndef UTILS_H
#define UTILS_H

inline int strLen(const char* s) {
    int i = 0; while (s[i]) i++; return i;
}

inline void strCopy(char* dst, const char* src, int max = 256) {
    int i = 0;
    while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

inline int strCmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return a[i] - b[i]; i++; }
    return a[i] - b[i];
}

inline char toLowerChar(char c) {
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

inline int strCmpInsensitive(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i]) {
        char ca = toLowerChar(a[i]), cb = toLowerChar(b[i]);
        if (ca != cb) return ca - cb; i++;
    }
    return toLowerChar(a[i]) - toLowerChar(b[i]);
}

inline void strConcat(char* dst, const char* src, int max = 256) {
    int d = strLen(dst), s = 0;
    while (src[s] && d < max - 1) dst[d++] = src[s++];
    dst[d] = '\0';
}

inline void intToStr(int n, char* buf) {
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    char tmp[20]; int i = 0; bool neg = n < 0;
    if (neg) n = -n;
    while (n > 0) { tmp[i++] = '0' + n % 10; n /= 10; }
    if (neg) tmp[i++] = '-';
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

inline int strToInt(const char* s) {
    int r = 0, i = 0; bool neg = s[0] == '-';
    if (neg) i = 1;
    while (s[i] >= '0' && s[i] <= '9') r = r * 10 + (s[i++] - '0');
    return neg ? -r : r;
}

inline void floatToStr(float f, char* buf) {
    if (f < 0) { *buf++ = '-'; f = -f; }
    int whole = (int)f, frac = (int)((f - whole) * 100 + 0.5f);
    char tmp[20]; intToStr(whole, tmp); strCopy(buf, tmp);
    int p = strLen(buf); buf[p++] = '.';
    if (frac < 10) buf[p++] = '0';
    intToStr(frac, tmp);
    for (int i = 0; tmp[i]; i++) buf[p++] = tmp[i];
    buf[p] = '\0';
}

inline float strToFloat(const char* s) {
    float r = 0; int i = 0; bool neg = s[0] == '-';
    if (neg) i = 1;
    while (s[i] >= '0' && s[i] <= '9') r = r * 10 + (s[i++] - '0');
    if (s[i] == '.') { i++; float f = 0.1f; while (s[i] >= '0' && s[i] <= '9') { r += (s[i++] - '0') * f; f *= 0.1f; } }
    return neg ? -r : r;
}

inline bool isAllDigits(const char* s) {
    if (!s[0]) return false;
    for (int i = 0; s[i]; i++) if (s[i] < '0' || s[i]>'9') return false;
    return true;
}

inline int parseCSV(const char* line, char tokens[][200], int maxT) {
    int col = 0, pos = 0, count = 0;
    while (line[col] && line[col] != '\n' && line[col] != '\r') {
        if (line[col] == ',' && count < maxT - 1) { tokens[count][pos] = '\0'; count++; pos = 0; }
        else if (pos < 199) tokens[count][pos++] = line[col];
        col++;
    }
    tokens[count][pos] = '\0'; if (pos > 0 || count > 0) count++;
    return count;
}

inline void trimNewline(char* s) {
    int len = strLen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) s[--len] = '\0';
}

#endif