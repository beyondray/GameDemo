/*--------------------------------------------------------------
【头文件】：TornadoClass.h    Create by Beyond Ray,2013年2月
（描述）：一个封装了旋转的彩色龙卷风渲染的类
----------------------------------------------------------------*/

#pragma once

#define EXIST_WIDTH		50000
#define EXIST_HEIGHT	50000
#define EXIST_DEPTH		50000
#define TORNADONUMS		1801

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

struct TORNADOVERTEX
{
	float x, y, z;		//顶点坐标
	float u, v;			//纹理坐标
	TORNADOVERTEX(float _x, float _y, float _z, float _u, float _v)
		:x(_x), y(_y), z(_z), u(_u), v(_v){}
};

#define D3DFVF_TORNADO (D3DFVF_XYZ | D3DFVF_TEX1)

class TornadoClass
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice9;				//Direct3D设备接口（COM接口对象）
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer9;			//顶点缓存对象（COM接口对象）
	LPDIRECT3DTEXTURE9		* m_ppTexture9;				//指向纹理对象的指针（COM接口对象）
	D3DXMATRIX				m_matWorld;					//世界变换矩阵
	int						m_iTextures;				//记录纹理对象数目

	struct Tornado
	{
		float   x, y, z;				//随机坐标
		float	fR;						//随机旋转半径（绕随机坐标点旋转）			
		float	fFallSpeed;				//降落速度
		float	fRotateRSpeed;			//以旋转半径旋转的速度
		float	fRotateYSpeed;			//自身旋转的速度
		float   fRAngle;				//以旋转半径旋转的角度
		float	fYAngle;				//自身旋转的角度
		D3DXMATRIX matTrans;			//平移变换矩阵
		D3DXMATRIX matRRotation;		//以旋转半径旋转的旋转变换矩阵
		D3DXMATRIX matYRotation;		//自身绕轴旋转的旋转半径
		LPDIRECT3DTEXTURE9 pTexture9;	//动态切换保存的纹理贴图
	};

	Tornado	m_Tornado[TORNADONUMS];		//龙卷风粒子数组

public:
	TornadoClass(LPDIRECT3DDEVICE9 pd3dDevice9);		//构造函数
	~TornadoClass();									//析构函数

public:
	VOID LoadTextureFromFile(TCHAR *pFileName[], int iNums = 10);		//从文件中加载动态纹理，最多可加载iNums张
	HRESULT TornadoInit(float fSize, float fSpeed);						//龙卷风粒子的初始化
	VOID TornadoUpdate(float fElapsedTime);								//龙卷风粒子的更新
	VOID TornadoRender();												//龙卷风粒子的渲染

};