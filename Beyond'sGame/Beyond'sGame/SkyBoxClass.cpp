/*--------------------------------------------------------------
��Cpp�ļ�����SkyBoxClass.cpp    Create by Beyond Ray,2013��2��
����������һ����װ����պ���ͼ�����ʵ��
----------------------------------------------------------------*/

#include"SkyBoxClass.h"

//---------------------------------------��SkyBoxClass()������---------------------------------------------------
// Desc:���캯��
//---------------------------------------------------------------------------------------------------------------
SkyBoxClass::SkyBoxClass(IDirect3DDevice9 * pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_pVertexBuffer9 = NULL;
	for (unsigned int i = 0; i < 6; i++)
		m_pTexture9[i] = NULL;
	m_fLength = 0.0f;
}

//---------------------------------------��SkyBoxInit()������----------------------------------------------------
// Desc:��պг�ʼ�������㻺�����Ĵ��������㻺����������ã�
//---------------------------------------------------------------------------------------------------------------
HRESULT SkyBoxClass::SkyBoxInit(float fLength)
{
	m_fLength = fLength;

	HR(m_pd3dDevice9->CreateVertexBuffer(24 * sizeof(SKYBOXVERTEX), D3DUSAGE_WRITEONLY,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pVertexBuffer9, NULL));

	//���ö��㻺������
	float fX_Left, fX_Right, fY_Top, fY_Bottom, fZ_In, fZ_Out;
	fX_Left = fY_Bottom = fZ_Out = -m_fLength / 2; 
	fX_Right = fY_Top = fZ_In = m_fLength / 2;

	SKYBOXVERTEX Vertices[] =
	{
		//ǰ������
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_In, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_In, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_In, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_In, 1.0f, 0.0f),

		//��������
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_Out, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_Out, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_Out, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_Out, 1.0f, 0.0f),

		//��ߵ����
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_Out, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_Out, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_In, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_In, 1.0f, 0.0f),

		//�ұߵ����
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_In, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_In, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_Out, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_Out, 1.0f, 0.0f),

		//���������
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_In, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_Out, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_In, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_Out, 1.0f, 0.0f),

		//�ײ������
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_Out, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_In, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_Out, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_In, 1.0f, 0.0f)
	};

	VOID * pVertices = NULL;
	HR(m_pVertexBuffer9->Lock(0, 0, (void**)&pVertices, 0));
	memcpy(pVertices, Vertices, sizeof(Vertices));
	m_pVertexBuffer9->Unlock();

	return S_OK;
}

//-----------------------------------------��LoadTextureFromFile()������----------------------------------------
// Desc:��ͼ���ļ��м���5�����������ͼ
//--------------------------------------------------------------------------------------------------------------
VOID SkyBoxClass::LoadTextureFromFile(TCHAR * pFrontTextureFile , TCHAR * pBackTextureFile,
	TCHAR * pLeftTextureFile , TCHAR * pRightTextureFile , 
	TCHAR * pTopTextureFile , TCHAR * pBottomTextureFile)
{
	D3DXCreateTextureFromFile(m_pd3dDevice9, pFrontTextureFile, &m_pTexture9[0]);
	D3DXCreateTextureFromFile(m_pd3dDevice9, pBackTextureFile, &m_pTexture9[1]);
	D3DXCreateTextureFromFile(m_pd3dDevice9, pLeftTextureFile, &m_pTexture9[2]);
	D3DXCreateTextureFromFile(m_pd3dDevice9, pRightTextureFile, &m_pTexture9[3]);
	D3DXCreateTextureFromFile(m_pd3dDevice9, pTopTextureFile, &m_pTexture9[4]);
	D3DXCreateTextureFromFile(m_pd3dDevice9, pBottomTextureFile, &m_pTexture9[5]);
}

//-----------------------------------------��SkyBoxRender()������-----------------------------------------------
// Desc:��պе���Ⱦ�������������ã���Ⱦ���߿�ѡ����棩
//--------------------------------------------------------------------------------------------------------------
VOID SkyBoxClass::SkyBoxRender(D3DXMATRIX * pMatWorld, BOOL bWireFrame, int iSkyNums)
{
	//��������ɫ��ϵĵ�һ����������ɫֵ��Ϊ���
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//��������ɫ��ϵĵ�һ����������ɫֵȡΪ��ɫֵ
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pd3dDevice9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pd3dDevice9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pd3dDevice9->SetTransform(D3DTS_WORLD, pMatWorld);

	m_pd3dDevice9->SetStreamSource(0, m_pVertexBuffer9, 0, sizeof(SKYBOXVERTEX));
	m_pd3dDevice9->SetFVF(D3DFVF_SKYBOX);
	
	for (int i = 0; i < iSkyNums; i++)
	{
		m_pd3dDevice9->SetTexture(0, m_pTexture9[i]);
		m_pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	if (bWireFrame)
	{
		m_pd3dDevice9->SetTexture(0, NULL);		//��������
		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		for (int i = 0; i < iSkyNums; i++)
			m_pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//-----------------------------------------��~SkyBoxClass()������----------------------------------------------
// Desc:��������
//-------------------------------------------------------------------------------------------------------------
SkyBoxClass::~SkyBoxClass()
{
	SAFE_RELEASE(m_pVertexBuffer9);
	for (unsigned int i = 0; i < 6; i++)
		SAFE_RELEASE(m_pTexture9[i]);
}