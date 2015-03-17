/*----------------------------------------------------------------
【头文件】：XFileModeClass.h    Create by Beyond Ray,2013年2月 
（描述）：封装了X文件模型载入类的头文件
------------------------------------------------------------------*/

//【编译指令】
#pragma once

//【包含头文件】
#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

//【XFileModelClass类】
class XFileModelClass
{
private:
	LPDIRECT3DDEVICE9	 m_pd3dDevice9;			//Direct3D设备接口（COM接口对象）
	LPD3DXMESH			 m_pMesh;				//网格对象（COM接口对象）
	D3DMATERIAL9	   * m_pMaterial9;			//保存材质信息
	LPDIRECT3DTEXTURE9 * m_ppd3dTexture9;		//保存纹理信息（COM接口对象）
	DWORD				 m_dwMtrlNums;			//材质或纹理的数量
	D3DXMATRIX			 m_matWorld;			//世界变换矩阵

public:
	XFileModelClass(LPDIRECT3DDEVICE9 pd3dDevice9);		//构造函数
	~XFileModelClass();									//析构函数

public:
	HRESULT XFileModelLoadFromFile(LPCWSTR pFileName);				//从文件中加载模型（包括网格，材质，纹理）
	VOID SetTransform(float fScale, float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f,
		int iAngleX = 0, int iAngleY = 0, int iAngleZ = 0);
																	//直接设置世界变换矩阵，方便渲染
	D3DXMATRIX SetTransformNotExactly(float fScale, float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f,
		int iAngleX = 0, int iAngleY = 0, int iAngleZ = 0);			//设置世界变换矩阵前半部分，方便调整
	HRESULT RenderModel();											//模型的渲染
};