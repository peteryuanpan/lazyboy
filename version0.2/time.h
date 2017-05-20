#ifndef TIME_H_
#define TIME_H_

const int SEARCH_TOTAL_TIME = 0;
const int THIS_SEARCH_TIME = 1;

// 设置时间限制
void SetTimeLimit ( const int x, const double limit );

// 初始化开始时间
void InitBeginTime ( const int x );

// 已超时
bool TimeOut ( const int x );

// 计算用时
double TimeCost ( const int x );

#endif /* TIME_H_ */
