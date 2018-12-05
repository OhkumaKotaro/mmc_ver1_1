#ifndef __MOTION_H
#define __MOTION_H
#ifdef __cplusplus
 extern "C" {
#endif



void Normal_Turn_Half(void);
void Turn_Half(void);
void Turn_Quarter_Left(void);
void Turn_Quarter_Right(void);
void Normal_Ennkai(void);
void Ennkai(void);
void Normal_Straight(void);
void Straight(void);
void Straight_HalF(void);
void Straight_Start(void);
void Straight_Check(void);
void Back(void);
void Motion_Start(void);
void Motion_Straight(void);
void Motion_Straight_Check(void);
void Motion_Back(void);
void Motion_Left(void);
void Motion_Right(void);
void Motion_Uturn(void);
void Motion_Kabeate(void);
void Motion_Goal(void);
void Motion_nStraight(unsigned char num);
void Show_log(void);
void Sensor_Mode(void);
void Test_Create_Map(void);
void Batt_Check(void);
void PT_Switch(void);
void Kabeate_dec(unsigned char flag);


#ifdef __cplusplus
}
#endif
#endif