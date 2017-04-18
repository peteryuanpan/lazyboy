#ifndef TIME_H_
#define TIME_H_

// 设置时间限制
void SetTimeLimit ( const double limit );

// 初始化开始时间
void InitBeginTime ( void );

// 已超时
bool TimeOut ( void );

// 计算用时
double TimeCost ( void );

#endif /* TIME_H_ */
