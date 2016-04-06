/*--------头文件、宏定义、定量--------*/
#ifndef HEADFILE_H
#define HEADFILE_H
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cassert>
#include <map>
using namespace std;

typedef long long ll;
#define glen(s) (int)strlen(s)
#define slen(s) (int)s.length()
#define peter cout << "i am peter" << endl

const int N = 300010, M = 2 * N;
const int nrow = 10, ncol = 9;
const int JiangX[] = {1, -1, 0, 0};
const int JiangY[] = {0, 0, 1, -1};
const int MaX[] = {-2, -2, -1, 1, 2, 2, 1, -1};
const int MaY[] = {-1, 1, 2, 2, 1, -1, -2, -2};
const int XiangX[] = {2, 2, -2, -2};
const int XiangY[] = {2, -2, 2, -2};
const int ShiX[] = {1, 1, -1, -1};
const int ShiY[] = {1, -1, 1, -1};
const int BinX[] = {1, -1, 0, 0};
const int BinY[] = {0, 0, 1, -1};
#endif
