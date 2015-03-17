/*----------------------------------------------------------------
��ͷ�ļ�����XFileModeClass.h    Create by Beyond Ray,2013��2�� 
������������װ��X�ļ�ģ���������ͷ�ļ�
------------------------------------------------------------------*/

//������ָ�
#pragma once

//������ͷ�ļ���
#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

//��XFileModelClass�ࡿ
class XFileModelClass
{
private:
	LPDIRECT3DDEVICE9	 m_pd3dDevice9;			//Direct3D�豸�ӿڣ�COM�ӿڶ���
	LPD3DXMESH			 m_pMesh;				//�������COM�ӿڶ���
	D3DMATERIAL9	   * m_pMaterial9;			//���������Ϣ
	LPDIRECT3DTEXTURE9 * m_ppd3dTexture9;		//����������Ϣ��COM�ӿڶ���
	DWORD				 m_dwMtrlNums;			//���ʻ����������
	D3DXMATRIX			 m_matWorld;			//����任����

public:
	XFileModelClass(LPDIRECT3DDEVICE9 pd3dDevice9);		//���캯��
	~XFileModelClass();									//��������

public:
	HRESULT XFileModelLoadFromFile(LPCWSTR pFileName);				//���ļ��м���ģ�ͣ��������񣬲��ʣ�����
	VOID SetTransform(float fScale, float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f,
		int iAngleX = 0, int iAngleY = 0, int iAngleZ = 0);
																	//ֱ����������任���󣬷�����Ⱦ
	D3DXMATRIX SetTransformNotExactly(float fScale, float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f,
		int iAngleX = 0, int iAngleY = 0, int iAngleZ = 0);			//��������任����ǰ�벿�֣��������
	HRESULT RenderModel();											//ģ�͵���Ⱦ
};