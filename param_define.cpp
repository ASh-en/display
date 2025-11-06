#pragma once
#include "type_define.h"


const int WAVE_POINT_NUM = 1500;
struct PARAM_STRUCT_S
{
	UINT16 index; //参数号
	INT16 value;  //参数值
};

//是否使用曲线平滑 1使用曲线平滑、0不使用
typedef enum
{
	NONE_SMOOTH = 0,
	USING_CURVE_SMOOTH = 1
}IS_CURVE_SMOOTH;
/*回波类型*/
typedef enum
{
	ENVELOPE_WAVE = 0, //包络波
	ORIGIN_WAVE = 1    //原始波
}WAVE_TYPE_E;
/*频率选择*/
typedef enum
{
	LOW_FREQUENCY = 0,    //低频
	HIGH_FREQUENCY = 1    //高频
}FREQUENCY_SELECT_E;
/*测量模式*/
typedef enum
{
	MEASURE_AUTO = 0,        //自动
	MEASURE_HALF_AUTO,
	MEASURE_MANUAL,      //手动
	
	MEASURE_THIN_THIKNESS = 4,   //测薄板模式
}MEASURE_MODE_E;
/*测量模式控制*/
typedef enum
{
	PP_CONTROL_MODE = 0,
	AUTO_CONTROL_MODE =1,
}MEASURE_MODE_CONTROL_E;
/*手动测量模式控制*/
typedef enum
{
	MANUAL_PP_CONTROL_MODE = 0,
	MANUAL_ZP_CONTROL_MODE = 1,
}MANUAL_MEASURE_MODE;

typedef enum
{
	DISPLAY_THICKNESS = 0,
	DISPLAY_WAVE_TYPE = 1,
}LCD_DISPLAY_SWITCH;
typedef enum
{
	SELF_MULTIPLYING = 0,
	ORIGINAL_SIGNAL = 1,
}SELF_MULTIPLY_ALGORITHM;

typedef enum
{
	ALUMINIUM_ALLOY = 0,
	TITANIUM_ALLOY = 1,
}MESURE_MATERIAL_E;

class DEVICE_PARAM_S
{
public:
	PARAM_STRUCT_S gate2End;          //0.闸门2结束
	PARAM_STRUCT_S zeroPointEnvelope; //1.探头零点（包络波）
	PARAM_STRUCT_S ultraSpeed;        //2.超声声速
	PARAM_STRUCT_S zeroPointOrigin;	  //3.探头零点（原始波）
	PARAM_STRUCT_S gate2Start;        //4.闸门2起始
	PARAM_STRUCT_S gate1Start;        //5.闸门1开始
	PARAM_STRUCT_S gate1End;          //6.闸门1结束
	PARAM_STRUCT_S calibrationThick;  //7.标块厚度
	PARAM_STRUCT_S temperature;       //8.环境温度
	PARAM_STRUCT_S curveSmooth;       //9.是否使用曲线平滑 IS_CURVE_SMOOTH
	PARAM_STRUCT_S smoothTime;        //10.平滑次数
	PARAM_STRUCT_S sensorGain;        //11.探头增益
	PARAM_STRUCT_S ferqSelect;        //12.频率选择 FREQUENCY_SELECT_E
	PARAM_STRUCT_S waveType;          //13.回波类型  WAVE_TYPE_E
	PARAM_STRUCT_S measureMode;       //14.测量模式
	PARAM_STRUCT_S SNR;               //15.信噪比
	PARAM_STRUCT_S dampingRadio;      //16.信号衰减比值
	PARAM_STRUCT_S envelopeWidth;     //17.波宽
	PARAM_STRUCT_S widthHeightRadio;  //18.包络波宽高比
	PARAM_STRUCT_S thresholdValue;    //19.阈值
	PARAM_STRUCT_S emitInterval;      //20.发射间隔
	PARAM_STRUCT_S pulseNumber;       //21.脉冲数量
	PARAM_STRUCT_S excitationFrequency;  //22.激励频率
	PARAM_STRUCT_S startDisplay;         //23.显示开始
	PARAM_STRUCT_S stopDisplay;          //24.显示结束
	PARAM_STRUCT_S communicateMode;      //25. 通讯模式
	PARAM_STRUCT_S measureControlMode;    //26.测量控制模式
	PARAM_STRUCT_S manualMeasureMode;     //27.手动测量控制模式
	PARAM_STRUCT_S lcdDisplayTxt;          //28.LCD屏显示内容切换
	PARAM_STRUCT_S selfMultiplying;        //29.自乘算法
	PARAM_STRUCT_S  thicknessSmooth;        //30.厚度平滑
	PARAM_STRUCT_S  measureMaterial;        //31.测厚材料 0.铝 1.钛合金
};

const int PARAM_SZIE = sizeof(DEVICE_PARAM_S) / sizeof(PARAM_STRUCT_S);
//设备仪表参数
union DEVICE_ULTRA_PARAM_U
{
	DEVICE_PARAM_S stParam;
	PARAM_STRUCT_S arrParam[sizeof(DEVICE_PARAM_S) / sizeof(PARAM_STRUCT_S)];
};
void init_device_param(DEVICE_ULTRA_PARAM_U& deviceParam);

