/*----------------------------------------------------------------
【Cpp文件】：XFileModeClass.cpp    Create by Beyond Ray,2013年2月
（描述）：封装了X文件模型载入类的实现
------------------------------------------------------------------*/

//【包含头文件】
#include"XFileModelClass.h"
#include<tchar.h>
//---------------------------【XFileModelClass()函数】-------------------------------------------------
//  Desc:构造函数的实现
//-----------------------------------------------------------------------------------------------------
XFileModelClass::XFileModelClass(LPDIRECT3DDEVICE9 pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
}

//---------------------------【~XFileModelClass()函数】------------------------------------------------
//  Desc:析构函数的实现
//-----------------------------------------------------------------------------------------------------
XFileModelClass::~XFileModelClass()
{
	if (m_ppd3dTexture9)
	{
		for (DWORD i = 0; i < m_dwMtrlNums; i++)
			SAFE_RELEASE(m_ppd3dTexture9[i]);		
		SAFE_DELETE_ARRAY(m_ppd3dTexture9);			//释放网格模型纹理
	}

	SAFE_DELETE_ARRAY(m_pMaterial9);				//释放网格模型材质对象
	SAFE_RELEASE(m_pMesh);						    //释放网格模型对象
}

//----------------------------【SetTransform()函数】---------------------------------------------------
// Desc:直接设置世界变换矩阵，方便渲染(注意：角度是以度为单位的，不是弧度）
//-----------------------------------------------------------------------------------------------------
VOID XFileModelClass::SetTransform(float fScale, float fX, float fY, float fZ,
	int iAngleX, int iAngleY, int iAngleZ)
{
	static D3DXMATRIX matScaling, matTrans, matRX, matRY, matRZ;
	D3DXMatrixScaling(&matScaling, fScale, fScale, fScale);
	D3DXMatrixTranslation(&matTrans, fX, fY, fZ);
	D3DXMatrixRotationX(&matRX, iAngleX * 2 * D3DX_PI / 360.0f);
	D3DXMatrixRotationY(&matRY, iAngleY * 2 * D3DX_PI / 360.0f);
	D3DXMatrixRotationZ(&matRZ, iAngleZ * 2 * D3DX_PI / 360.0f);
	m_matWorld = matScaling * matRY * matRX * matRZ * matTrans;
	m_pd3dDevice9->SetTransform(D3DTS_WORLD, &m_matWorld);
}

//----------------------------【SetTransformEx()函数】---------------------------------------------------
// Desc:设置世界变换矩阵前半部分，返回该矩阵, 方便调整并渲染(注意：角度是以度为单位的，不是弧度）
//-----------------------------------------------------------------------------------------------------
D3DXMATRIX XFileModelClass::SetTransformNotExactly( float fScale, float fX, float fY, float fZ,
	int iAngleX, int iAngleY, int iAngleZ)
{
	D3DXMATRIX matScaling, matTrans, matRX, matRY, matRZ;
	D3DXMatrixScaling(&matScaling, fScale, fScale, fScale);
	D3DXMatrixTranslation(&matTrans, fX, fY, fZ);
	D3DXMatrixRotationX(&matRX, iAngleX * 2 * D3DX_PI / 360.0f);
	D3DXMatrixRotationY(&matRY, iAngleY * 2 * D3DX_PI / 360.0f);
	D3DXMatrixRotationZ(&matRZ, iAngleZ * 2 * D3DX_PI / 360.0f);
	m_matWorld = matScaling * matRY * matRX * matRZ, matTrans;
	return m_matWorld;
}

//----------------------------【XFileModleLoadFromFile()函数】-----------------------------------------
//  Desc:从文件中加载模型（包括网格，材质，纹理）
//-----------------------------------------------------------------------------------------------------
HRESULT XFileModelClass::XFileModelLoadFromFile(LPCWSTR pFileName)
{
	//加载模型网格
	LPD3DXBUFFER pAdjBuffer = NULL;		//邻接信息缓冲区
	LPD3DXBUFFER pMtrlBuffer = NULL;	//材质和纹理信息缓冲区
	D3DXLoadMeshFromX(pFileName, D3DXMESH_MANAGED, m_pd3dDevice9, &pAdjBuffer, &pMtrlBuffer,
		NULL, &m_dwMtrlNums, &m_pMesh);
	
	//读取材质和纹理信息
	m_pMaterial9 = new D3DMATERIAL9[m_dwMtrlNums];
	m_ppd3dTexture9 = new LPDIRECT3DTEXTURE9[m_dwMtrlNums];

	D3DXMATERIAL * pD3DXMtrls = (D3DXMATERIAL *)pMtrlBuffer->GetBufferPointer();
	for (DWORD i = 0; i < m_dwMtrlNums; i++)
	{
		m_pMaterial9[i] = pD3DXMtrls[i].MatD3D;
		m_pMaterial9[i].Ambient = m_pMaterial9[i].Diffuse;
		m_ppd3dTexture9[i] = NULL;
		if (FAILED(D3DXCreateTextureFromFileA(m_pd3dDevice9, pD3DXMtrls[i].pTextureFilename, &m_ppd3dTexture9[i])))
		{
			//MessageBox(NULL, _T("网格模型的纹理载入失败~！！！"), _T("Beyond Ray的消息窗口"), MB_OK);
			//return E_FAIL;
		}
	}

	//网格模型的优化
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_STRIPREORDER,
		(DWORD *)pAdjBuffer->GetBufferPointer(), NULL, NULL, NULL);

	//释放不用的COM接口资源
	SAFE_RELEASE(pAdjBuffer);
	SAFE_RELEASE(pMtrlBuffer);

	return S_OK;
}

//----------------------------【RenderModel()函数】---------------------------------------------------
//  Desc:模型的渲染
//----------------------------------------------------------------------------------------------------
HRESULT XFileModelClass::RenderModel()
{
	//设置材质和纹理，并绘画模型
	for (DWORD i = 0; i < m_dwMtrlNums; i++)
	{
		m_pd3dDevice9->SetMaterial(&m_pMaterial9[i]);
		m_pd3dDevice9->SetTexture(0, m_ppd3dTexture9[i]);
		m_pMesh->DrawSubset(i);
	}
	return S_OK;
}
