#ifndef _C_STRING_H
#define _C_STRING_H

#include <stddef.h>

void memcpy(void* dest, void* src, size_t count);
void memmove(void* dest, void* src, size_t count);
void memset(void* dest, int ch, size_t count);
void* memchr(const void* ptr, int ch, size_t count);
void* memrchr(const void* ptr, int ch, size_t count);
int memcmp(const void* ptr1, const void* ptr2, size_t count);
void __memset_impl(void* dest, int ch, size_t count);

size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t num);
char* strcat(char* dest, const char* src);
int strcmp(char* str1, char* str2);
int strncmp(char* str1, char* str2, size_t num);
int strcasecmp(char* str1, char* str2);
int strncasecmp(char* str1, char* str2, size_t num);
const char* strchr(const char* str, char ch);
const char* strstr(const char* str1, const char* str2);

#endif
