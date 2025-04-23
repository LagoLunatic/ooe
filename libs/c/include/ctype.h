#ifndef CTYPE_H
#define CTYPE_H

extern unsigned char __lower_map[];
extern unsigned char __upper_map[];

inline int tolower(int c) {
    return (c < 0 || c >= 128 ? c : (int)__lower_map[c]);
}

inline int toupper(int c) {
    return (c < 0 || c >= 128 ? c : (int)__upper_map[c]);
}

#endif /* CTYPE_H */
