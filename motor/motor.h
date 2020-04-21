#ifndef MOTOR_H
#define MOTOR_H

#include "pthread.h"
#include <wiringPi.h>

#ifdef __cplusplus
extern "C" {
#endif


#define Left_Motor_Pin_1	23
#define Left_Motor_Pin_2	24
#define Right_Motor_Pin_1	21
#define Right_Motor_Pin_2	22

#define Left_track_forward()  {digitalWrite (Left_Motor_Pin_1, 1);digitalWrite (Left_Motor_Pin_2, 0);}
#define Left_track_backoff()  {digitalWrite (Left_Motor_Pin_1, 0);digitalWrite (Left_Motor_Pin_2, 1);}
#define Left_track_stop()	  {digitalWrite (Left_Motor_Pin_1, 0);digitalWrite (Left_Motor_Pin_2, 0);}
#define Right_track_forward() {digitalWrite (Right_Motor_Pin_1, 1);digitalWrite (Right_Motor_Pin_2, 0);}
#define Right_track_backoff() {digitalWrite (Right_Motor_Pin_1, 0);digitalWrite (Right_Motor_Pin_2, 1);}
#define Right_track_stop()    {digitalWrite (Right_Motor_Pin_1, 0);digitalWrite (Right_Motor_Pin_2, 0);}

class motor
{
public:
	motor(){};
	~motor(){};
	int start(void);
	friend void speed_right(void);
	friend void speed_left(void);
	void turn_left(int);
	void trun_left_done(void);
	void turn_right(int);
	void trun_right_done(void);
	int speed_right_value = 0;
	int speed_left_value = 0;
	void direct_line(int value);
	void backoff();
	void turn_over();
private:
	pthread_t tid[2];
	void init(void);
};




#ifdef __cplusplus
}
#endif

#endif