/*--------------------------------------------------------------
【Cpp文件】：CameraClass.cpp    Create by Beyond Ray,2013年2月
（描述）：一个封装了摄像机应用的类的实现(取景变换，投影变换）
----------------------------------------------------------------*/

#include"CameraClass.h"

//-------------------------------------------【CameraClass()函数】-----------------------------------------------
// Desc:构造函数
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

//-------------------------------------------【CalculateMarixView()函数】----------------------------------------
// Desc:根据摄像机观察向量单位正交化分量，并计算取景变换矩阵
//---------------------------------------------------------------------------------------------------------------
VOID CameraClass::CalculateMatrixView(D3DXMATRIX * pmatView)
{
	//摄像机三分量正交化
	D3DXVec3Normalize(&m_vLook, &m_vLook);			
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);

	D3DXVec3Normalize(&m_vRight, &m_vRight);

	//计算取景变换矩阵
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

//--------------------------------------------【SetCameraPos()函数】-------------------------------------------
// Desc:设置摄像机的向量位置
//-------------------------------------------------------------------------------------------------------------
VOID CameraClass::SetCameraPos(D3DXVECTOR3 * pvCameraPos /* = NULL */)
{
	if (pvCameraPos)
		m_vCameraPos = *pvCameraPos;
	else
		m_vCameraPos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
}

//--------------------------------------------【SetLookatPos()函数】-------------------------------------------
// Desc:设置观察点位置
//-------------------------------------------------------------------------------------------------------------
VOID CameraClass::SetLookatPos(D3DXVECTOR3 * pvLookatPos /* = NULL */)
{
	if (pvLookatPos)
		m_vLookatPos = *pvLookatPos;
	else
		m_vLookatPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//重新计算观察向量
	m_vLook = m_vLookatPos - m_vCameraPos;
	m_iDistance = D3DXVec3Length(&m_vLook);

	//摄像机三分量正交化
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);

	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);

	D3DXVec3Normalize(&m_vRight, &m_vRight);
}

//--------------------------------------------【SetMatrixView()函数】-----------------------------------------
// Desc:设置取景变换矩阵
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

//-------------------------------------------【SetMatrixProj()函数】-----------------------------------------
// Desc:设置投影变换矩阵
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::SetMatrixProj(D3DXMATRIX * pmatProj /* = NULL */)
{
	if (pmatProj)
		m_matProj = *pmatProj;
	else
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)((double)WINDOW_WIDTH / WINDOW_HEIGHT), 1.0f, 300000.0f);

	m_pd3dDevice9->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

//-------------------------------------------【GetMatrixView()函数】-----------------------------------------
// Desc:返回取景变换矩阵
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::GetMarixView(D3DXMATRIX * pmatView)
{
	pmatView = &m_matView;
}

//-------------------------------------------【GetMatrixProj()函数】-----------------------------------------
// Desc:返回投影变换矩阵
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::GetMatrixProj(D3DXMATRIX * pmatProj)
{
	pmatProj = &m_matProj;
}

//-------------------------------------------【GetCameraPos()函数】------------------------------------------
// Desc:返回摄像机向量位置
//-----------------------------------------------------------------------------------------------------------
D3DXVECTOR3* CameraClass::GetCameraPos(D3DXVECTOR3 * pvCameraPos)
{
	pvCameraPos = &m_vCameraPos;
	return &m_vCameraPos;
}

//-------------------------------------------【GetLookatPos()函数】------------------------------------------
// Desc:返回观察点向量位置
//-----------------------------------------------------------------------------------------------------------
D3DXVECTOR3* CameraClass::GetLookatPos(D3DXVECTOR3 * pvLookatPos)
{
	pvLookatPos = &m_vLookatPos;
	return &m_vLookatPos;
}

//-------------------------------------------【TranslateAlongX()函数】---------------------------------------
// Desc:沿右向量平移fUnit个单位
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::TranslateAlongX(float fUnit)
{
	m_vCameraPos += m_vRight * fUnit;
	m_vLookatPos += m_vRight * fUnit;
}

//-------------------------------------------【TranslateAlongY()函数】---------------------------------------
// Desc:沿上向量平移fUnit个单位
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::TranslateAlongY(float fUnit)
{
	m_vCameraPos += m_vUp * fUnit;
	m_vLookatPos += m_vUp * fUnit;
}

//-------------------------------------------【TranslateAlongY()函数】---------------------------------------
// Desc:沿观察向量平移fUnit个单位
//-----------------------------------------------------------------------------------------------------------
VOID CameraClass::TranslateAlongZ(float fUnit)
{
	m_vCameraPos += m_vLook * fUnit;
	m_vLookatPos += m_vLook * fUnit;
}

//-------------------------------------------【CameraRotationX()函数】---------------------------------------
// Desc:沿着摄像机右方向向量顺时针旋转fangle.
//-----------------------------------------------------------------------------------------------------------
D3DXMATRIX CameraClass::CameraRotationX(float fAngle)
{
	static D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vRight, fAngle);		//计算出绕轴旋转的矩阵
	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &R);			//在上分量方向乘以该矩阵变换
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &R);		//在观察分量方向乘以该矩阵变换
	m_vLookatPos = m_vCameraPos + m_vLook * m_iDistance;

	return R;
}

//-------------------------------------------【CameraRotationY()函数】---------------------------------------
// Desc:沿着摄像机上方向向量顺时针旋转fangle.
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

//-------------------------------------------【CameraRotationZ()函数】---------------------------------------
// Desc:沿着摄像机观察方向向量顺时针旋转fangle.
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

//-------------------------------------------【~CameraClass()函数】------------------------------------------
// Desc:析构函数
//-----------------------------------------------------------------------------------------------------------
CameraClass::~CameraClass()
{

}