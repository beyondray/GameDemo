/*--------------------------------------------------------------
【头文件】：SkyBoxClass.h    Create by Beyond Ray,2013年2月
（描述）：一个封装了天空盒贴图的类
----------------------------------------------------------------*/

#pragma  once

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

#ifndef D3DFVF_SKYBOX
#define D3DFVF_SKYBOX	D3DFVF_XYZ | D3DFVF_TEX1
#endif

class SkyBoxClass
{
private:
	LPDIRECT3DDEVICE9			m_pd3dDevice9;			//Direct3D设备接口对象(COM接口对象）
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer9;		//顶点缓冲区（COM接口对象）
	LPDIRECT3DTEXTURE9			m_pTexture9[6];			//天空贴图
	float						m_fLength;				//天空盒长度

	struct SKYBOXVERTEX
	{
		float x, y, z;		//顶点坐标
		float u, v;			//纹理坐标

		SKYBOXVERTEX(float _x, float _y, float _z, float _u, float _v)
			:x(_x), y(_y), z(_z), u(_u), v(_v){}
	};

public:
	explicit SkyBoxClass(IDirect3DDevice9 * pd3dDevice9);	//构造函数
	~SkyBoxClass();											//析构函数

public:
	HRESULT SkyBoxInit(float fLength);													//初始化天空盒
	VOID LoadTextureFromFile(TCHAR * pFrontTextureFile, TCHAR * pBackTextureFile = NULL,		
		TCHAR * pLeftTextureFile = NULL, TCHAR * pRightTextureFile = NULL,
		TCHAR * pTopTextureFile = NULL, TCHAR * pBottomTextureFile = NULL);				//加载天空纹理
	VOID SkyBoxRender(D3DXMATRIX * pMatWorld, BOOL bWireFrame,int iSkyNums = 6);		//天空盒渲染

};