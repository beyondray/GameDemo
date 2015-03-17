/*--------------------------------------------------------------
【Cpp文件】：SkyBoxClass.cpp    Create by Beyond Ray,2013年2月
（描述）：一个封装了天空盒贴图的类的实现
----------------------------------------------------------------*/

#include"SkyBoxClass.h"

//---------------------------------------【SkyBoxClass()函数】---------------------------------------------------
// Desc:构造函数
//---------------------------------------------------------------------------------------------------------------
SkyBoxClass::SkyBoxClass(IDirect3DDevice9 * pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_pVertexBuffer9 = NULL;
	for (unsigned int i = 0; i < 6; i++)
		m_pTexture9[i] = NULL;
	m_fLength = 0.0f;
}

//---------------------------------------【SkyBoxInit()函数】----------------------------------------------------
// Desc:天空盒初始化（顶点缓冲区的创建，顶点缓存数组的设置）
//---------------------------------------------------------------------------------------------------------------
HRESULT SkyBoxClass::SkyBoxInit(float fLength)
{
	m_fLength = fLength;

	HR(m_pd3dDevice9->CreateVertexBuffer(24 * sizeof(SKYBOXVERTEX), D3DUSAGE_WRITEONLY,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pVertexBuffer9, NULL));

	//设置顶点缓存数组
	float fX_Left, fX_Right, fY_Top, fY_Bottom, fZ_In, fZ_Out;
	fX_Left = fY_Bottom = fZ_Out = -m_fLength / 2; 
	fX_Right = fY_Top = fZ_In = m_fLength / 2;

	SKYBOXVERTEX Vertices[] =
	{
		//前面的天空
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_In, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_In, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_In, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_In, 1.0f, 0.0f),

		//后面的天空
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_Out, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_Out, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_Out, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_Out, 1.0f, 0.0f),

		//左边的天空
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_Out, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_Out, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Left, fY_Bottom, fZ_In, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_In, 1.0f, 0.0f),

		//右边的天空
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_In, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_In, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Bottom, fZ_Out, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_Out, 1.0f, 0.0f),

		//顶部的天空
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_In, 0.0f, 1.0f),
		SKYBOXVERTEX(fX_Left, fY_Top, fZ_Out, 0.0f, 0.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_In, 1.0f, 1.0f),
		SKYBOXVERTEX(fX_Right, fY_Top, fZ_Out, 1.0f, 0.0f),

		//底部的天空
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

//-----------------------------------------【LoadTextureFromFile()函数】----------------------------------------
// Desc:从图形文件中加载5个天空纹理贴图
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

//-----------------------------------------【SkyBoxRender()函数】-----------------------------------------------
// Desc:天空盒的渲染（世界矩阵的设置，渲染，线框选择描绘）
//--------------------------------------------------------------------------------------------------------------
VOID SkyBoxClass::SkyBoxRender(D3DXMATRIX * pMatWorld, BOOL bWireFrame, int iSkyNums)
{
	//将纹理颜色混合的第一个参数的颜色值作为输出
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//将纹理颜色混合的第一个参数的颜色值取为颜色值
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
		m_pd3dDevice9->SetTexture(0, NULL);		//清空纹理层
		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		for (int i = 0; i < iSkyNums; i++)
			m_pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//-----------------------------------------【~SkyBoxClass()函数】----------------------------------------------
// Desc:析构函数
//-------------------------------------------------------------------------------------------------------------
SkyBoxClass::~SkyBoxClass()
{
	SAFE_RELEASE(m_pVertexBuffer9);
	for (unsigned int i = 0; i < 6; i++)
		SAFE_RELEASE(m_pTexture9[i]);
}