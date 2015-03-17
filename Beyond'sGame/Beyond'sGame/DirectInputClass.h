/*--------------------------------------------------------------
【头文件】：DirectInputClass.h    Create by Beyond Ray,2013年2月
（描述）：一个封装了键盘和鼠标接收和输入状态的类
----------------------------------------------------------------*/

#pragma once

#define DIRECTINPUT_VERSION 0X0800
#include<dinput.h>

class DirectInputClass
{
private:
	IDirectInput8 * m_pDirectInput8;			//DirectInput接口对象（COM接口对象）	
	IDirectInputDevice8 * m_pKeyboardDevice8;	//键盘设备接口（COM接口对象）
	IDirectInputDevice8 * m_pMouseDevice8;		//鼠标设备接口（COM接口对象）
	char	m_KeyboardState[256];				//存储键盘状态缓冲区
	DIMOUSESTATE m_diMouseState;				//存储鼠标状态的结构体

public:
	DirectInputClass();		//构造函数
	~DirectInputClass();	//析构函数

public:
	HRESULT KeyBoardInit(HINSTANCE hInst, HWND hwnd, DWORD dwKeyboardFlags);
	//DirectInput初始化，创键键盘设备，设格式和协作级别，获取设备控制权
	HRESULT MouseInit(HINSTANCE hInst, HWND hwnd, DWORD dwMouseFlags);	
	//DirectInput初始化，创键鼠标设备，设格式和协作级别，获取设备控制权
	HRESULT GetKeyboardInput();					//取得键盘按键的状态，简称取按键
	HRESULT GetMouseInput();					//取得鼠标按键状态，简称取按键		
	BOOL IsKeyboardDown(int DIK);				//判断某键盘按键是否按下			
	BOOL IsMouseButtonDown(int iKey);			//判断鼠标键是否按下
	long GetMouselX();							//记录了X轴相对移动量，由鼠标横向移动控制
	long GetMouselY();							//记录了Y轴相对移动量，由鼠标纵向移动控制
	long GetMouselZ();							//记录了Z轴相对移动量，由鼠标滚轮控制
};