/*--------------------------------------------------------------
【Cpp文件】：TornadoClass.cpp    Create by Beyond Ray,2013年2月
（描述）：一个封装了旋转的彩色龙卷风渲染的类的实现
----------------------------------------------------------------*/

#include"TornadoClass.h"

//------------------------------------------【TornadoClass()函数】----------------------------------------------
// Desc:构造函数
//--------------------------------------------------------------------------------------------------------------
TornadoClass::TornadoClass(LPDIRECT3DDEVICE9 pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_pVertexBuffer9 = NULL;
	m_ppTexture9 = NULL;
	for (int i = 0; i < TORNADONUMS; i++)
	{
		::ZeroMemory(&m_Tornado[i], sizeof(Tornado));
	}
}

//---------------------------------------【LoadTextureFromFile()函数】------------------------------------------
// Desc:从文件中加载动态纹理，最多可加载10张
//--------------------------------------------------------------------------------------------------------------
VOID TornadoClass::LoadTextureFromFile(TCHAR *pFileName[], int iNums /* = 10 */)
{
	m_iTextures = iNums;
	m_ppTexture9 = new LPDIRECT3DTEXTURE9[m_iTextures];
	for (int i = 0; i < m_iTextures; i++)
		D3DXCreateTextureFromFile(m_pd3dDevice9, pFileName[i], &m_ppTexture9[i]);
}

//------------------------------------------【TornadoInit()函数】-----------------------------------------------
// Desc:龙卷风粒子的初始化（定义粒子各种属性， 创建顶点缓冲区，设置顶点数组）
//--------------------------------------------------------------------------------------------------------------
HRESULT TornadoClass::TornadoInit(float fSize,float fSpeed)		//粒子尺寸大小，自身旋转速度
{
	srand(GetTickCount());		//初始化随机种子

	float fMaxR = min(EXIST_WIDTH, EXIST_DEPTH) / 2.0f;

	for (int i = 0; i < TORNADONUMS; i++)
	{
		//将随机的初始化左边限定在规定的空间范围内
		m_Tornado[i].x = ((float)EXIST_WIDTH * rand() / RAND_MAX) -EXIST_WIDTH / 2.0f;
		m_Tornado[i].z = ((float)EXIST_DEPTH * rand() / RAND_MAX) - EXIST_DEPTH / 2.0f;	
		m_Tornado[i].y = ((float)EXIST_HEIGHT * rand() / RAND_MAX) ;

		//给定随机的旋转半径
		m_Tornado[i].fR = fMaxR * rand() / RAND_MAX;

		//两个旋转速度和一个降落速度
		m_Tornado[i].fFallSpeed = 500.0f + (float)(rand() % 1000);
		m_Tornado[i].fRotateRSpeed = 1.0f * rand() / RAND_MAX;
		m_Tornado[i].fRotateYSpeed = fSpeed;			//自身的旋转速度为固定值

		//设定平移变换矩阵
		static float fAngle = 2 * D3DX_PI * ((float)rand() / RAND_MAX);
		D3DXMatrixTranslation(&m_Tornado[i].matTrans, m_Tornado[i].fR * cos(fAngle), 
			0.0f, m_Tornado[i].fR * sin(fAngle));

		//设定初始化纹理贴图
		m_Tornado[i].pTexture9 = m_ppTexture9[0];
	}

	HR(m_pd3dDevice9->CreateVertexBuffer(4 * sizeof(TORNADOVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_TORNADO,
		D3DPOOL_MANAGED, &m_pVertexBuffer9, NULL));

	TORNADOVERTEX * pVertices = NULL;
	HR(m_pVertexBuffer9->Lock(0, 0, (void**)&pVertices, 0));

	pVertices[0] = TORNADOVERTEX(-fSize, -fSize, 0.0f, 0.0f, 1.0f);
	pVertices[1] = TORNADOVERTEX(-fSize, fSize, 0.0f, 0.0f, 0.0f);
	pVertices[2] = TORNADOVERTEX(fSize, -fSize, 0.0f, 1.0f, 1.0f);
	pVertices[3] = TORNADOVERTEX(fSize, fSize, 0.0f, 1.0f, 0.0f);

	m_pVertexBuffer9->Unlock();

	return S_OK;
}

//-------------------------------------------【TornadoUpdate()函数】------------------------------------------
// Desc:粒子位置更新函数（包括两个旋转矩阵的更新，降落位置的更新，并带有超出边界判断）
//------------------------------------------------------------------------------------------------------------
VOID TornadoClass::TornadoUpdate(float fElapsedTime)
{
	static int iIndex ;
	iIndex = 0;
	for (int i = 0; i < TORNADONUMS; i++)
	{
		(i % 2 == 1) ? m_Tornado[i].y -= m_Tornado[i].fFallSpeed * fElapsedTime
			: m_Tornado[i].y += m_Tornado[i].fFallSpeed * fElapsedTime;	//高度值更新
		if (m_Tornado[i].y < 0.0f || m_Tornado[i].y > EXIST_HEIGHT)
			m_Tornado[i].y = ((float)EXIST_HEIGHT * rand() / RAND_MAX);

		m_Tornado[i].pTexture9 = m_ppTexture9[iIndex++];			//切换动态纹理

		if (iIndex >= m_iTextures)
			iIndex = 0;

		//两个旋转矩阵的设置
		(i % 2) == 0 ? m_Tornado[i].fRAngle += fElapsedTime * m_Tornado[i].fRotateRSpeed
			: m_Tornado[i].fRAngle -= fElapsedTime * m_Tornado[i].fRotateRSpeed;
		(i % 3) == 0 ? m_Tornado[i].fYAngle += fElapsedTime * m_Tornado[i].fRotateYSpeed
			: m_Tornado[i].fYAngle -= fElapsedTime * m_Tornado[i].fRotateYSpeed;
		D3DXMatrixRotationY(&m_Tornado[i].matRRotation, m_Tornado[i].fRAngle);
		D3DXMatrixRotationY(&m_Tornado[i].matYRotation, m_Tornado[i].fYAngle);
	}
	
}

//-------------------------------------------【TronadoRender()函数】-------------------------------------------
// Desc:龙卷风粒子的渲染（设置Alpha混合，纹理过滤，开启不剔除模式）
//-------------------------------------------------------------------------------------------------------------
VOID TornadoClass::TornadoRender()
{

	//将颜色混合的第一个参数的颜色值作为输出
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//将颜色混合的第一个参数的颜色值取为纹理值
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//设置线性纹理过滤方式
	m_pd3dDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//设置Alpha颜色混合
	m_pd3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pd3dDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pd3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//设置剔除模式为不剔除
	m_pd3dDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//准备开始渲染粒子
	m_pd3dDevice9->SetStreamSource(0, m_pVertexBuffer9, 0, sizeof(TORNADOVERTEX));
	m_pd3dDevice9->SetFVF(D3DFVF_TORNADO);

	static D3DXMATRIX matTransXYZ;
	for (unsigned int i = 0; i < TORNADONUMS; i++)
	{
		D3DXMatrixTranslation(&matTransXYZ, m_Tornado[i].x, m_Tornado[i].y, m_Tornado[i].z);
		m_matWorld = m_Tornado[i].matYRotation  * m_Tornado[i].matTrans * m_Tornado[i].matRRotation * matTransXYZ;
		m_pd3dDevice9->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pd3dDevice9->SetTexture(0, m_Tornado[i].pTexture9);
		m_pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	//将光照开启，剔除模式设为背面消隐， 关闭Alpha混合
	m_pd3dDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

//----------------------------------------【~TornadoClass()函数】-----------------------------------------------
// Desc:析构函数
//--------------------------------------------------------------------------------------------------------------
TornadoClass::~TornadoClass()
{
	for (int i = 0; i < TORNADONUMS; i++)
		m_Tornado[i].pTexture9 = NULL;

	for (int i = 0; i < m_iTextures; i++)
		SAFE_RELEASE(m_ppTexture9[i]);

	SAFE_DELETE_ARRAY(m_ppTexture9);
	SAFE_RELEASE(m_pVertexBuffer9);
}