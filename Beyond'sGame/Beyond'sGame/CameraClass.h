/*--------------------------------------------------------------
【头文件】：CameraClass.h    Create by Beyond Ray,2013年2月
（描述）：一个封装了摄像机应用的类(取景变换，投影变换）
----------------------------------------------------------------*/

#pragma once

#ifndef WINDOW_WIDTH 
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 775
#endif

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

class CameraClass
{
private:
	LPDIRECT3DDEVICE9	m_pd3dDevice9;					//Direct3D设备接口对象(COM接口对象）
	D3DXVECTOR3			m_vUp;							//摄像机上分量向量
	D3DXVECTOR3			m_vRight;						//摄像机右分量向量	
	D3DXVECTOR3			m_vLook;						//摄像机观察方向向量
	D3DXVECTOR3			m_vCameraPos;					//摄像机的向量位置
	D3DXVECTOR3			m_vLookatPos;					//观察点的向量位置
	D3DXMATRIX			m_matView;						//取景变换矩阵
	D3DXMATRIX			m_matProj;						//投影变换矩阵
	int					m_iDistance;					//摄像机位置到观测点位置的距离				

public:
	CameraClass(LPDIRECT3DDEVICE9 pd3dDevice9);			//构造函数
	~CameraClass();										//析构函数

public:
	VOID CalculateMatrixView(D3DXMATRIX * matView);			//计算取景变换矩阵

	VOID SetCameraPos(D3DXVECTOR3 * pvCameraPos = NULL);		//设置摄像机位置
	VOID SetLookatPos(D3DXVECTOR3 * pvLookatPos = NULL);		//设置观察点位置
	VOID SetMatrixView(D3DXMATRIX * pmatView = NULL);		//设置当前取景变换矩阵
	VOID SetMatrixProj(D3DXMATRIX * pmatProj = NULL);		//设置当前投影变换矩阵

	VOID GetMarixView(D3DXMATRIX * pmatView);					//得到取景变换矩阵
	VOID GetMatrixProj(D3DXMATRIX * pmatProj);					//得到投影变换矩阵
	D3DXVECTOR3* GetCameraPos(D3DXVECTOR3 * pvCameraPos = NULL);	//返回当前摄像机向量位置
	D3DXVECTOR3* GetLookatPos(D3DXVECTOR3 * pvLookatPos = NULL);	//返回当前观测点向量位置

	//沿着摄像机的局部X,Y,Z方向平移
	VOID TranslateAlongX(float fUnit);					//沿右向量平移fUnit个单位
	VOID TranslateAlongY(float fUnit);					//沿上向量平移fUnit个单位
	VOID TranslateAlongZ(float fUnit);					//沿观察向量平移fUnit个单位

	//沿着摄像机的局部X,Y,Z轴旋转（正为顺时针）
	D3DXMATRIX CameraRotationX(float fAngle);					//沿着摄像机右向量顺时针旋转fangle.
	D3DXMATRIX CameraRotationY(float fAngle);					//沿着摄像机上向量顺时针旋转fangle.
	D3DXMATRIX CameraRotationZ(float fAngle);					//沿着摄像机观察向量顺时针旋转fangle.

};