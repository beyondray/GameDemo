/*------------------------------------------------------------------
【Cpp文件】：DirectInputClass.cpp    Create by Beyond Ray,2013年2月
（描述）：一个封装了键盘和鼠标接收和输入状态类的实现
--------------------------------------------------------------------*/

#include"D3DComMacro.h"
#include"DirectInputClass.h"

//----------------------------------【DirectInputClass()函数】------------------------------------------
// Desc:构造函数
//------------------------------------------------------------------------------------------------------
DirectInputClass::DirectInputClass()
{
	m_pDirectInput8 = NULL;
	m_pKeyboardDevice8 = NULL;
	m_pMouseDevice8 = NULL;
	::ZeroMemory(m_KeyboardState, sizeof(char) * 256);
	::ZeroMemory(&m_diMouseState, sizeof(m_diMouseState));
}

//----------------------------------【DirectInputInit()函数】-------------------------------------------
// Desc://DirectInput初始化，创建键盘设备，设置数据格式和协作级别，获取设备控制权
//------------------------------------------------------------------------------------------------------
HRESULT DirectInputClass::KeyBoardInit(HINSTANCE hInst, HWND hwnd, DWORD dwKeyboardFlags)
{
	//初始化DirectInput接口
	HR(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput8, NULL));

	//创建键盘设备
	HR(m_pDirectInput8->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice8, NULL));

	//设置键盘数据格式和协作级别，获取设备控制权
	HR(m_pKeyboardDevice8->SetDataFormat(&c_dfDIKeyboard));
	HR(m_pKeyboardDevice8->SetCooperativeLevel(hwnd, dwKeyboardFlags));
	HR(m_pKeyboardDevice8->Acquire());

	SAFE_RELEASE(m_pDirectInput8);
	return S_OK;
}

//----------------------------------【DirectInputInit()函数】-------------------------------------------
// Desc://DirectInput初始化，创建鼠标设备，设置数据格式和协作级别，获取设备控制权
//------------------------------------------------------------------------------------------------------
HRESULT DirectInputClass::MouseInit(HINSTANCE hInst, HWND hwnd, DWORD dwMouseFlags)
{
	//初始化DirectInput接口
	HR(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput8, NULL));

	//创建鼠标设备
	HR(m_pDirectInput8->CreateDevice(GUID_SysMouse, &m_pMouseDevice8, NULL));

	//设置鼠标数据格式和协作级别，获取设备控制权
	HR(m_pMouseDevice8->SetDataFormat(&c_dfDIMouse));
	HR(m_pMouseDevice8->SetCooperativeLevel(hwnd, dwMouseFlags));
	HR(m_pMouseDevice8->Acquire());

	SAFE_RELEASE(m_pDirectInput8);
	return S_OK;
}

//--------------------------------------【GetInput()函数】----------------------------------------------
// Desc://判断按键情况，取得按键的状态，简称取按键
//------------------------------------------------------------------------------------------------------
HRESULT DirectInputClass::GetKeyboardInput()
{
	HRESULT hr = E_FAIL;
	while (!(hr = m_pKeyboardDevice8->Poll()))
	{
		if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED)
			return E_FAIL;
		HR(m_pKeyboardDevice8->Acquire());
	}
	m_pKeyboardDevice8->Acquire();
	HR(m_pKeyboardDevice8->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)m_KeyboardState));
	return S_OK;
}

//--------------------------------------【GetInput()函数】----------------------------------------------
// Desc:判断按键情况，取得按键的状态，简称取按键
//------------------------------------------------------------------------------------------------------
HRESULT DirectInputClass::GetMouseInput()
{
	HRESULT hr = E_FAIL;
	while (!(hr = m_pMouseDevice8->Poll()))
	{
		if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED)
			return E_FAIL;
		HR(m_pMouseDevice8->Acquire());
	}
	m_pMouseDevice8->Acquire();
	HR(m_pMouseDevice8->GetDeviceState(sizeof(m_diMouseState), &m_diMouseState));
	return S_OK;
}

//------------------------------------【IsKeyboardDown()函数】------------------------------------------
// Desc:判断某键盘按键是否按下
//------------------------------------------------------------------------------------------------------
BOOL DirectInputClass::IsKeyboardDown(int DIK)
{
	if (m_KeyboardState[DIK] & 0X80)
		return TRUE;
	else
		return FALSE;
	
}

//------------------------------------【IsMouseButtonDown()函数】---------------------------------------
// Desc:判断某鼠标按键是否按下
//------------------------------------------------------------------------------------------------------
BOOL DirectInputClass::IsMouseButtonDown(int iKey)
{
	return ((m_diMouseState.rgbButtons[iKey] & 0X80) != 0);
}

//------------------------------------【GetMouselX()函数】----------------------------------------------
// Desc:记录了X轴相对移动量，由鼠标横向移动控制
//------------------------------------------------------------------------------------------------------
long DirectInputClass::GetMouselX()
{
	return m_diMouseState.lX;
}

//------------------------------------【GetMouselY()函数】----------------------------------------------
// Desc:记录了Y轴相对移动量，由鼠标纵向移动控制
//------------------------------------------------------------------------------------------------------
long DirectInputClass::GetMouselY()
{
	return m_diMouseState.lY;
}
//------------------------------------【GetMouselZ()函数】----------------------------------------------
// Desc:记录了Z轴相对移动量，由鼠标滚轮控制
//------------------------------------------------------------------------------------------------------
long DirectInputClass::GetMouselZ()
{
	return m_diMouseState.lZ;
}

//----------------------------------【~DirectInputClass()函数】-----------------------------------------
// Desc:析构函数
//------------------------------------------------------------------------------------------------------
DirectInputClass::~DirectInputClass()
{
	if (m_pKeyboardDevice8 != NULL)
		m_pKeyboardDevice8->Unacquire();
	if (m_pMouseDevice8 != NULL)
		m_pMouseDevice8->Unacquire();
	SAFE_RELEASE(m_pDirectInput8);			//释放DirectInput接口对象
	SAFE_RELEASE(m_pKeyboardDevice8);		//释放键盘设备对象
	SAFE_RELEASE(m_pMouseDevice8);			//释放鼠标设备对象
}