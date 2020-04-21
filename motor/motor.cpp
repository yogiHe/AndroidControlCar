#include <unistd.h>
#include "motor.h"
#include<string> 

static void* speed_right(void *arg)
{
	int i;
	motor *pdata = (motor *)arg;
	for (;;) {
		if (pdata->speed_right_value > 0) {
			for (i = 0; i < 100 - pdata->speed_right_value; i++)	usleep(10);
			Right_track_forward();
			for (i = 0; i < pdata->speed_right_value; i++)	usleep(10);
			Right_track_stop();
		}
		else if (pdata->speed_right_value < 0) {
			for (i = 0; i < pdata->speed_right_value+100; i++)	usleep(10);
			Right_track_backoff();
			for (i = 0; i > pdata->speed_right_value; i--)	usleep(10);
			Right_track_stop();
		}
		else {
			Right_track_stop();
			usleep(5000);
			//printf("stop mode \n");
		}
	}
	pthread_exit(NULL);
}

static void* speed_left(void *arg)
{
	int i;
	motor *pdata = (motor *)arg;
	for (;;) {
		if (pdata->speed_left_value > 0) {
			for (i = 0; i < 100 - pdata->speed_left_value; i++)	usleep(10);
			Left_track_forward();
			for (i = 0; i < pdata->speed_left_value; i++)	usleep(10);
			Left_track_stop();
		}
		else if (pdata->speed_left_value < 0) {
			for (i = 0; i < pdata->speed_left_value + 100; i++)	usleep(10);
			Left_track_backoff();
			for (i = 0; i > pdata->speed_left_value; i--)	usleep(10);
			Left_track_stop();
		}
		else {
			Left_track_stop();
			usleep(5000);
			//printf("stop mode \n");
		}
	}
	pthread_exit(NULL);
}

void motor::turn_left(int value)
{
	//speed_left_value = speed_left_value / 90 * (90 - value);
	speed_left_value = -speed_left_value;
}

void motor::trun_left_done(void)
{
	speed_left_value = -speed_left_value;
}

void motor::turn_right(int value)
{
	//speed_right_value = speed_right_value / 90 * value;
	speed_right_value = -speed_right_value;
}

void motor::trun_right_done(void)
{
	//speed_right_value = speed_right_value / 90 * value;
	speed_right_value = -speed_right_value;
}

void motor::direct_line(int value)
{/*
	if (speed_right_value > speed_left_value)
		speed_left_value = speed_right_value;
	else
		speed_right_value = speed_left_value;
*/
	speed_right_value = value;
	speed_left_value = value;
}

void motor::turn_over()
{
	if(speed_right_value>0 || speed_left_value>0){
		if (speed_right_value > speed_left_value)
			speed_left_value = speed_right_value;
		else
			speed_right_value = speed_left_value;
	}
	else{
		if (speed_right_value > speed_left_value)
			speed_right_value = speed_left_value;
		else
			speed_left_value = speed_right_value;
	}
	

}

void motor::backoff()
{
	speed_right_value = -speed_right_value;
	speed_left_value = -speed_left_value;
}

void motor::init(void)
{
	wiringPiSetup ();
	pinMode(Left_Motor_Pin_1, OUTPUT);
	pinMode(Left_Motor_Pin_2, OUTPUT);
	pinMode(Right_Motor_Pin_1, OUTPUT);
	pinMode(Right_Motor_Pin_2, OUTPUT);

}

int motor::start(void)
{
	int ret = 0;
	init();
	if ((ret = pthread_create(&tid[0], NULL, speed_right, this)) != 0) {
		printf("motor pthread_cread 1 error: error code =%d", ret) ;
		ret = -4;
		goto end;
	}
	if ((ret = pthread_create(&tid[1], NULL, speed_left, this)) != 0) {
		printf("motor pthread_cread 2 error: error code =%d", ret);
		ret = -4;
		goto end;
	}
end:
	return ret;

}




