/*--------------------------------------------------------------
��ͷ�ļ�����DirectInputClass.h    Create by Beyond Ray,2013��2��
����������һ����װ�˼��̺������պ�����״̬����
----------------------------------------------------------------*/

#pragma once

#define DIRECTINPUT_VERSION 0X0800
#include<dinput.h>

class DirectInputClass
{
private:
	IDirectInput8 * m_pDirectInput8;			//DirectInput�ӿڶ���COM�ӿڶ���	
	IDirectInputDevice8 * m_pKeyboardDevice8;	//�����豸�ӿڣ�COM�ӿڶ���
	IDirectInputDevice8 * m_pMouseDevice8;		//����豸�ӿڣ�COM�ӿڶ���
	char	m_KeyboardState[256];				//�洢����״̬������
	DIMOUSESTATE m_diMouseState;				//�洢���״̬�Ľṹ��

public:
	DirectInputClass();		//���캯��
	~DirectInputClass();	//��������

public:
	HRESULT KeyBoardInit(HINSTANCE hInst, HWND hwnd, DWORD dwKeyboardFlags);
	//DirectInput��ʼ�������������豸�����ʽ��Э�����𣬻�ȡ�豸����Ȩ
	HRESULT MouseInit(HINSTANCE hInst, HWND hwnd, DWORD dwMouseFlags);	
	//DirectInput��ʼ������������豸�����ʽ��Э�����𣬻�ȡ�豸����Ȩ
	HRESULT GetKeyboardInput();					//ȡ�ü��̰�����״̬�����ȡ����
	HRESULT GetMouseInput();					//ȡ����갴��״̬�����ȡ����		
	BOOL IsKeyboardDown(int DIK);				//�ж�ĳ���̰����Ƿ���			
	BOOL IsMouseButtonDown(int iKey);			//�ж������Ƿ���
	long GetMouselX();							//��¼��X������ƶ��������������ƶ�����
	long GetMouselY();							//��¼��Y������ƶ���������������ƶ�����
	long GetMouselZ();							//��¼��Z������ƶ������������ֿ���
};