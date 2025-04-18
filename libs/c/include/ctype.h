#ifndef CTYPE_H
#define CTYPE_H

extern unsigned char __lower_map[];

inline int tolower(int c) {
    return (c == -1 ? -1 : (int)__lower_map[(unsigned char)c]);
}

#endif /* CTYPE_H */
