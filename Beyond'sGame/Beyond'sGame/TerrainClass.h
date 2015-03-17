/*--------------------------------------------------------------
【头文件】：TerrainClass.h    Create by Beyond Ray,2013年2月
（描述）：一个封装了地形高度图载入，初始化和渲染的类
----------------------------------------------------------------*/

#pragma  once

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;

class TerrainClass
{
private:
	LPDIRECT3DDEVICE9			m_pd3dDevice9;		//Direct3D设备接口对象（COM接口对象）
	LPDIRECT3DTEXTURE9		  	m_pTexture9;		//纹理信息（COM接口对象）
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer9;	//顶点缓存（COM接口对象）
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuffer9;	//索引缓存（COM接口对象）

	int				m_iVerticesRow;			//每行的顶点个数
	int				m_iVerticesCol;			//每列的顶点个数
	int				m_iCellsRow;			//每行的格子数
	int				m_iCellsCol;			//每列的格子数
	int				m_iVertexNums;			//总的地形顶点数
	FLOAT			m_fCellSpace;			//每个格子的空间长度
	FLOAT			m_fTerrainWidth;		//地形的宽度(X轴）
	FLOAT			m_fTerrainDepth;		//地形的深度(Z轴）
	FLOAT			m_fTerrainScale;		//地形的规模（缩放比例系数）
	vector<float>	m_vHeightInfo;			//顶点的高度信息（Y轴）

	struct TERRAINVERTEX 
	{
		float x, y, z;		//顶点坐标
		float u, v;			//纹理贴图坐标

		TERRAINVERTEX(float _x, float _y, float _z, float _u, float _v) :
			x(_x), y(_y), z(_z), u(_u), v(_v){}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};

public:
	explicit TerrainClass(IDirect3DDevice9 * pd3dDevice9);	//构造函数
	~TerrainClass();										//析构函数

public:
	BOOL LoadTerrainFromFile(TCHAR * pFileName, TCHAR * pTextureName);		//载入地形的高度图和纹理
	HRESULT TerrainInit(FLOAT fPicNums, int iVerticeRow, int iVerticesCol,
		int iVerticesRowTrue,  FLOAT fCellSpace, FLOAT fTerrainScale = 8.0); //地形网格的初始化
	VOID TerrainRender(D3DXMATRIX * matWorld, BOOL bWireFrame);       //地形的渲染
};