/*--------------------------------------------------------------
��ͷ�ļ�����CameraClass.h    Create by Beyond Ray,2013��2��
����������һ����װ�������Ӧ�õ���(ȡ���任��ͶӰ�任��
----------------------------------------------------------------*/

#pragma once

#ifndef WINDOW_WIDTH 
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 775
#endif

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

class CameraClass
{
private:
	LPDIRECT3DDEVICE9	m_pd3dDevice9;					//Direct3D�豸�ӿڶ���(COM�ӿڶ���
	D3DXVECTOR3			m_vUp;							//������Ϸ�������
	D3DXVECTOR3			m_vRight;						//������ҷ�������	
	D3DXVECTOR3			m_vLook;						//������۲췽������
	D3DXVECTOR3			m_vCameraPos;					//�����������λ��
	D3DXVECTOR3			m_vLookatPos;					//�۲�������λ��
	D3DXMATRIX			m_matView;						//ȡ���任����
	D3DXMATRIX			m_matProj;						//ͶӰ�任����
	int					m_iDistance;					//�����λ�õ��۲��λ�õľ���				

public:
	CameraClass(LPDIRECT3DDEVICE9 pd3dDevice9);			//���캯��
	~CameraClass();										//��������

public:
	VOID CalculateMatrixView(D3DXMATRIX * matView);			//����ȡ���任����

	VOID SetCameraPos(D3DXVECTOR3 * pvCameraPos = NULL);		//���������λ��
	VOID SetLookatPos(D3DXVECTOR3 * pvLookatPos = NULL);		//���ù۲��λ��
	VOID SetMatrixView(D3DXMATRIX * pmatView = NULL);		//���õ�ǰȡ���任����
	VOID SetMatrixProj(D3DXMATRIX * pmatProj = NULL);		//���õ�ǰͶӰ�任����

	VOID GetMarixView(D3DXMATRIX * pmatView);					//�õ�ȡ���任����
	VOID GetMatrixProj(D3DXMATRIX * pmatProj);					//�õ�ͶӰ�任����
	D3DXVECTOR3* GetCameraPos(D3DXVECTOR3 * pvCameraPos = NULL);	//���ص�ǰ���������λ��
	D3DXVECTOR3* GetLookatPos(D3DXVECTOR3 * pvLookatPos = NULL);	//���ص�ǰ�۲������λ��

	//����������ľֲ�X,Y,Z����ƽ��
	VOID TranslateAlongX(float fUnit);					//��������ƽ��fUnit����λ
	VOID TranslateAlongY(float fUnit);					//��������ƽ��fUnit����λ
	VOID TranslateAlongZ(float fUnit);					//�ع۲�����ƽ��fUnit����λ

	//����������ľֲ�X,Y,Z����ת����Ϊ˳ʱ�룩
	D3DXMATRIX CameraRotationX(float fAngle);					//���������������˳ʱ����תfangle.
	D3DXMATRIX CameraRotationY(float fAngle);					//���������������˳ʱ����תfangle.
	D3DXMATRIX CameraRotationZ(float fAngle);					//����������۲�����˳ʱ����תfangle.

};