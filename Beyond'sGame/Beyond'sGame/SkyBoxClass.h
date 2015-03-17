/*--------------------------------------------------------------
��ͷ�ļ�����SkyBoxClass.h    Create by Beyond Ray,2013��2��
����������һ����װ����պ���ͼ����
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
	LPDIRECT3DDEVICE9			m_pd3dDevice9;			//Direct3D�豸�ӿڶ���(COM�ӿڶ���
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer9;		//���㻺������COM�ӿڶ���
	LPDIRECT3DTEXTURE9			m_pTexture9[6];			//�����ͼ
	float						m_fLength;				//��պг���

	struct SKYBOXVERTEX
	{
		float x, y, z;		//��������
		float u, v;			//��������

		SKYBOXVERTEX(float _x, float _y, float _z, float _u, float _v)
			:x(_x), y(_y), z(_z), u(_u), v(_v){}
	};

public:
	explicit SkyBoxClass(IDirect3DDevice9 * pd3dDevice9);	//���캯��
	~SkyBoxClass();											//��������

public:
	HRESULT SkyBoxInit(float fLength);													//��ʼ����պ�
	VOID LoadTextureFromFile(TCHAR * pFrontTextureFile, TCHAR * pBackTextureFile = NULL,		
		TCHAR * pLeftTextureFile = NULL, TCHAR * pRightTextureFile = NULL,
		TCHAR * pTopTextureFile = NULL, TCHAR * pBottomTextureFile = NULL);				//�����������
	VOID SkyBoxRender(D3DXMATRIX * pMatWorld, BOOL bWireFrame,int iSkyNums = 6);		//��պ���Ⱦ

};