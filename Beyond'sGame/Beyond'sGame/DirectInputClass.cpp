/*------------------------------------------------------------------
��Cpp�ļ�����DirectInputClass.cpp    Create by Beyond Ray,2013��2��
����������һ����װ�˼��̺������պ�����״̬���ʵ��
--------------------------------------------------------------------*/

#include"D3DComMacro.h"
#include"DirectInputClass.h"

//----------------------------------��DirectInputClass()������------------------------------------------
// Desc:���캯��
//------------------------------------------------------------------------------------------------------
DirectInputClass::DirectInputClass()
{
	m_pDirectInput8 = NULL;
	m_pKeyboardDevice8 = NULL;
	m_pMouseDevice8 = NULL;
	::ZeroMemory(m_KeyboardState, sizeof(char) * 256);
	::ZeroMemory(&m_diMouseState, sizeof(m_diMouseState));
}

//----------------------------------��DirectInputInit()������-------------------------------------------
// Desc://DirectInput��ʼ�������������豸���������ݸ�ʽ��Э�����𣬻�ȡ�豸����Ȩ
//------------------------------------------------------------------------------------------------------
HRESULT DirectInputClass::KeyBoardInit(HINSTANCE hInst, HWND hwnd, DWORD dwKeyboardFlags)
{
	//��ʼ��DirectInput�ӿ�
	HR(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput8, NULL));

	//���������豸
	HR(m_pDirectInput8->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice8, NULL));

	//���ü������ݸ�ʽ��Э�����𣬻�ȡ�豸����Ȩ
	HR(m_pKeyboardDevice8->SetDataFormat(&c_dfDIKeyboard));
	HR(m_pKeyboardDevice8->SetCooperativeLevel(hwnd, dwKeyboardFlags));
	HR(m_pKeyboardDevice8->Acquire());

	SAFE_RELEASE(m_pDirectInput8);
	return S_OK;
}

//----------------------------------��DirectInputInit()������-------------------------------------------
// Desc://DirectInput��ʼ������������豸���������ݸ�ʽ��Э�����𣬻�ȡ�豸����Ȩ
//------------------------------------------------------------------------------------------------------
HRESULT DirectInputClass::MouseInit(HINSTANCE hInst, HWND hwnd, DWORD dwMouseFlags)
{
	//��ʼ��DirectInput�ӿ�
	HR(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput8, NULL));

	//��������豸
	HR(m_pDirectInput8->CreateDevice(GUID_SysMouse, &m_pMouseDevice8, NULL));

	//����������ݸ�ʽ��Э�����𣬻�ȡ�豸����Ȩ
	HR(m_pMouseDevice8->SetDataFormat(&c_dfDIMouse));
	HR(m_pMouseDevice8->SetCooperativeLevel(hwnd, dwMouseFlags));
	HR(m_pMouseDevice8->Acquire());

	SAFE_RELEASE(m_pDirectInput8);
	return S_OK;
}

//--------------------------------------��GetInput()������----------------------------------------------
// Desc://�жϰ��������ȡ�ð�����״̬�����ȡ����
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

//--------------------------------------��GetInput()������----------------------------------------------
// Desc:�жϰ��������ȡ�ð�����״̬�����ȡ����
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

//------------------------------------��IsKeyboardDown()������------------------------------------------
// Desc:�ж�ĳ���̰����Ƿ���
//------------------------------------------------------------------------------------------------------
BOOL DirectInputClass::IsKeyboardDown(int DIK)
{
	if (m_KeyboardState[DIK] & 0X80)
		return TRUE;
	else
		return FALSE;
	
}

//------------------------------------��IsMouseButtonDown()������---------------------------------------
// Desc:�ж�ĳ��갴���Ƿ���
//------------------------------------------------------------------------------------------------------
BOOL DirectInputClass::IsMouseButtonDown(int iKey)
{
	return ((m_diMouseState.rgbButtons[iKey] & 0X80) != 0);
}

//------------------------------------��GetMouselX()������----------------------------------------------
// Desc:��¼��X������ƶ��������������ƶ�����
//------------------------------------------------------------------------------------------------------
long DirectInputClass::GetMouselX()
{
	return m_diMouseState.lX;
}

//------------------------------------��GetMouselY()������----------------------------------------------
// Desc:��¼��Y������ƶ���������������ƶ�����
//------------------------------------------------------------------------------------------------------
long DirectInputClass::GetMouselY()
{
	return m_diMouseState.lY;
}
//------------------------------------��GetMouselZ()������----------------------------------------------
// Desc:��¼��Z������ƶ������������ֿ���
//------------------------------------------------------------------------------------------------------
long DirectInputClass::GetMouselZ()
{
	return m_diMouseState.lZ;
}

//----------------------------------��~DirectInputClass()������-----------------------------------------
// Desc:��������
//------------------------------------------------------------------------------------------------------
DirectInputClass::~DirectInputClass()
{
	if (m_pKeyboardDevice8 != NULL)
		m_pKeyboardDevice8->Unacquire();
	if (m_pMouseDevice8 != NULL)
		m_pMouseDevice8->Unacquire();
	SAFE_RELEASE(m_pDirectInput8);			//�ͷ�DirectInput�ӿڶ���
	SAFE_RELEASE(m_pKeyboardDevice8);		//�ͷż����豸����
	SAFE_RELEASE(m_pMouseDevice8);			//�ͷ�����豸����
}