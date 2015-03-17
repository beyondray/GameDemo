/*--------------------------------------------------------------
��Cpp�ļ�����CameraClass.cpp    Create by Beyond Ray,2013��2��
����������һ����װ�������Ӧ�õ����ʵ��(ȡ���任��ͶӰ�任��
----------------------------------------------------------------*/

#include"CameraClass.h"

//-------------------------------------------��CameraClass()������-----------------------------------------------
// Desc:���캯��
//---------------------------------------------------------------------------------------------------------------
CameraClass::CameraClass(LPDIRECT3DDEVICE9 pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vCameraPos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	m_vLookatPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-------------------------------------------��CalculateMarixView()������----------------------------------------
// Desc:����������۲�������λ������������������ȡ���任����
//---------------------------------------------------------------------------------------------------------------
VOID CameraClass::CalculateMatrixView(D3DXMATRIX * pmatView)
{
	//�����������������
	D3DXVec3Normalize(&m_vLook, &m_vLook);			
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);

	D3DXVec3Normalize(&m_vRight, &m_vRight);

	//����ȡ���任����
	pmatView->_11 = m_vRight.x;	
	pmatView->_12 = m_vUp.x;
	pmatView->_13 = m_vLook.x;
	pmatView->_14 = 0.0f;

	pmatView->_21 = m_vRight.y;
	pmatView->_22 = m_vUp.y;
	pmatView->_23 = m_vLook.y;
	pmatView->_24 = 0.0f;

	pmatView->_31 = m_vRight.z;
	pmatView->_32 = m_vUp.z;
	pmatView->_33 = m_vLook.z;
	pmatView->_34 = 0.0f;

	pmatView->_41 = -D3DXVec3Dot(&m_vCameraPos, &m_vRight);
	pmatView->_42 = -D3DXVec3Dot(&m_vCameraPos, &m_vUp);
	pmatView->_43 = -D3DXVec3Dot(&m_vCameraPos, &m_vLook);
	pmatView->_44 = 1.0f;
}

//--------------------------------------------��SetCameraPos()������-------------------------------------------
// Desc:���������������λ��
//-------------------------------------------------------------------------------------------------------------
VOID CameraClass::SetCameraPos(D3DXVECTOR3 * pvCameraPos /* = NULL */)
{
	if (pvCameraPos)
		m_vCameraPos = *pvCameraPos;
	else
		m_vCameraPos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
}

//--------------------------------------------��SetLookatPos()������-------------------------------------------
// Desc:���ù۲��λ��
//-------------------------------------------------------------------------------------------------------------
VOID CameraClass::SetLookatPos(D3DXVECTOR3 * pvLookatPos /* = NULL */)
{
	if (pvLookatPos)
		m_vLookatPos = *pvLookatPos;
	else
		m_vLookatPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���¼���۲�����
	m_vLook = m_vLookatPos - m_vCameraPos;
	m_iDistance = D3DXVec3Length(&m_vLook);

	//�����������������
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);

	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);

	D3DXVec3Normalize(&m_vRight, &m_vRight);
}

//--------------------------------------------��SetMatrixView()������-----------------------------------------
// Desc:����ȡ���任����
//------------------------------------------------------------------------------------------------------------
VOID CameraClass::SetMatrixView(D3DXMATRIX * pmatView /* = NULL */)
{
	if (pmatView)
		m_matView = *pmatView;
	else
		CalculateMatrixView(&m_matView);
	m_pd3dDevice9->SetTransform(D3DTS_VIEW, &m_matView);

	m_vRight = D3DXVECTOR3(m_matView._11, m_matView._21, m_matView._31);
	m_vUp = D3DXVECTOR3(m_matView._12, m_matView._22, m_matView._32);
	m_vLook = D3DXVECTOR3(m_matView._13, m_matView._23, m_matView._33);
}

//-------------------------------------------��SetMatrixProj()������-----------------------------------------
// Desc:����ͶӰ�任����
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::SetMatrixProj(D3DXMATRIX * pmatProj /* = NULL */)
{
	if (pmatProj)
		m_matProj = *pmatProj;
	else
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)((double)WINDOW_WIDTH / WINDOW_HEIGHT), 1.0f, 300000.0f);

	m_pd3dDevice9->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

//-------------------------------------------��GetMatrixView()������-----------------------------------------
// Desc:����ȡ���任����
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::GetMarixView(D3DXMATRIX * pmatView)
{
	pmatView = &m_matView;
}

//-------------------------------------------��GetMatrixProj()������-----------------------------------------
// Desc:����ͶӰ�任����
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::GetMatrixProj(D3DXMATRIX * pmatProj)
{
	pmatProj = &m_matProj;
}

//-------------------------------------------��GetCameraPos()������------------------------------------------
// Desc:�������������λ��
//-----------------------------------------------------------------------------------------------------------
D3DXVECTOR3* CameraClass::GetCameraPos(D3DXVECTOR3 * pvCameraPos)
{
	pvCameraPos = &m_vCameraPos;
	return &m_vCameraPos;
}

//-------------------------------------------��GetLookatPos()������------------------------------------------
// Desc:���ع۲������λ��
//-----------------------------------------------------------------------------------------------------------
D3DXVECTOR3* CameraClass::GetLookatPos(D3DXVECTOR3 * pvLookatPos)
{
	pvLookatPos = &m_vLookatPos;
	return &m_vLookatPos;
}

//-------------------------------------------��TranslateAlongX()������---------------------------------------
// Desc:��������ƽ��fUnit����λ
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::TranslateAlongX(float fUnit)
{
	m_vCameraPos += m_vRight * fUnit;
	m_vLookatPos += m_vRight * fUnit;
}

//-------------------------------------------��TranslateAlongY()������---------------------------------------
// Desc:��������ƽ��fUnit����λ
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::TranslateAlongY(float fUnit)
{
	m_vCameraPos += m_vUp * fUnit;
	m_vLookatPos += m_vUp * fUnit;
}

//-------------------------------------------��TranslateAlongY()������---------------------------------------
// Desc:�ع۲�����ƽ��fUnit����λ
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::TranslateAlongZ(float fUnit)
{
	m_vCameraPos += m_vLook * fUnit;
	m_vLookatPos += m_vLook * fUnit;
}

//-------------------------------------------��CameraRotationX()������---------------------------------------
// Desc:����������ҷ�������˳ʱ����תfangle.
//-----------------------------------------------------------------------------------------------------------
D3DXMATRIX CameraClass::CameraRotationX(float fAngle)
{
	static D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vRight, fAngle);		//�����������ת�ľ���
	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &R);			//���Ϸ���������Ըþ���任
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &R);		//�ڹ۲����������Ըþ���任
	m_vLookatPos = m_vCameraPos + m_vLook * m_iDistance;

	return R;
}

//-------------------------------------------��CameraRotationY()������---------------------------------------
// Desc:����������Ϸ�������˳ʱ����תfangle.
//-----------------------------------------------------------------------------------------------------------
D3DXMATRIX CameraClass::CameraRotationY(float fAngle)
{
	static D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vUp, fAngle);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &R);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &R);
	m_vLookatPos = m_vCameraPos + m_vLook * m_iDistance;

	return R;
}

//-------------------------------------------��CameraRotationZ()������---------------------------------------
// Desc:����������۲췽������˳ʱ����תfangle.
//-----------------------------------------------------------------------------------------------------------
D3DXMATRIX CameraClass::CameraRotationZ(float fAngle)
{
	static D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vLook, fAngle);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &R);
	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &R);
	m_vLookatPos = m_vCameraPos + m_vLook * m_iDistance;

	return R;
}

//-------------------------------------------��~CameraClass()������------------------------------------------
// Desc:��������
//-----------------------------------------------------------------------------------------------------------
CameraClass::~CameraClass()
{

}