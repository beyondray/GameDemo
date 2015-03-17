/*----------------------------------------------------------------
��Cpp�ļ�����XFileModeClass.cpp    Create by Beyond Ray,2013��2��
������������װ��X�ļ�ģ���������ʵ��
------------------------------------------------------------------*/

//������ͷ�ļ���
#include"XFileModelClass.h"
#include<tchar.h>
//---------------------------��XFileModelClass()������-------------------------------------------------
//  Desc:���캯����ʵ��
//-----------------------------------------------------------------------------------------------------
XFileModelClass::XFileModelClass(LPDIRECT3DDEVICE9 pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
}

//---------------------------��~XFileModelClass()������------------------------------------------------
//  Desc:����������ʵ��
//-----------------------------------------------------------------------------------------------------
XFileModelClass::~XFileModelClass()
{
	if (m_ppd3dTexture9)
	{
		for (DWORD i = 0; i < m_dwMtrlNums; i++)
			SAFE_RELEASE(m_ppd3dTexture9[i]);		
		SAFE_DELETE_ARRAY(m_ppd3dTexture9);			//�ͷ�����ģ������
	}

	SAFE_DELETE_ARRAY(m_pMaterial9);				//�ͷ�����ģ�Ͳ��ʶ���
	SAFE_RELEASE(m_pMesh);						    //�ͷ�����ģ�Ͷ���
}

//----------------------------��SetTransform()������---------------------------------------------------
// Desc:ֱ����������任���󣬷�����Ⱦ(ע�⣺�Ƕ����Զ�Ϊ��λ�ģ����ǻ��ȣ�
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

//----------------------------��SetTransformEx()������---------------------------------------------------
// Desc:��������任����ǰ�벿�֣����ظþ���, �����������Ⱦ(ע�⣺�Ƕ����Զ�Ϊ��λ�ģ����ǻ��ȣ�
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

//----------------------------��XFileModleLoadFromFile()������-----------------------------------------
//  Desc:���ļ��м���ģ�ͣ��������񣬲��ʣ�����
//-----------------------------------------------------------------------------------------------------
HRESULT XFileModelClass::XFileModelLoadFromFile(LPCWSTR pFileName)
{
	//����ģ������
	LPD3DXBUFFER pAdjBuffer = NULL;		//�ڽ���Ϣ������
	LPD3DXBUFFER pMtrlBuffer = NULL;	//���ʺ�������Ϣ������
	D3DXLoadMeshFromX(pFileName, D3DXMESH_MANAGED, m_pd3dDevice9, &pAdjBuffer, &pMtrlBuffer,
		NULL, &m_dwMtrlNums, &m_pMesh);
	
	//��ȡ���ʺ�������Ϣ
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
			//MessageBox(NULL, _T("����ģ�͵���������ʧ��~������"), _T("Beyond Ray����Ϣ����"), MB_OK);
			//return E_FAIL;
		}
	}

	//����ģ�͵��Ż�
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_STRIPREORDER,
		(DWORD *)pAdjBuffer->GetBufferPointer(), NULL, NULL, NULL);

	//�ͷŲ��õ�COM�ӿ���Դ
	SAFE_RELEASE(pAdjBuffer);
	SAFE_RELEASE(pMtrlBuffer);

	return S_OK;
}

//----------------------------��RenderModel()������---------------------------------------------------
//  Desc:ģ�͵���Ⱦ
//----------------------------------------------------------------------------------------------------
HRESULT XFileModelClass::RenderModel()
{
	//���ò��ʺ��������滭ģ��
	for (DWORD i = 0; i < m_dwMtrlNums; i++)
	{
		m_pd3dDevice9->SetMaterial(&m_pMaterial9[i]);
		m_pd3dDevice9->SetTexture(0, m_ppd3dTexture9[i]);
		m_pMesh->DrawSubset(i);
	}
	return S_OK;
}
