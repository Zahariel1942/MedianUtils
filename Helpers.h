#pragma once

#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <ctime>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>

void Print(const char* szFormat, ...);
bool SplitLines(const std::string& str, size_t maxlen, const char delim, std::list<std::string>& lst);
void Log(char* szFormat, ...);
void LogNoFormat(char* szString);
wchar_t* AnsiToUnicode(const char* str);
char* UnicodeToAnsi(const wchar_t* str);
bool StringToBool(const char* str);
bool InitHooks(void);
#endif
