/*--------------------------------------------------------------
��ͷ�ļ�����TerrainClass.h    Create by Beyond Ray,2013��2��
����������һ����װ�˵��θ߶�ͼ���룬��ʼ������Ⱦ����
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
	LPDIRECT3DDEVICE9			m_pd3dDevice9;		//Direct3D�豸�ӿڶ���COM�ӿڶ���
	LPDIRECT3DTEXTURE9		  	m_pTexture9;		//������Ϣ��COM�ӿڶ���
	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer9;	//���㻺�棨COM�ӿڶ���
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuffer9;	//�������棨COM�ӿڶ���

	int				m_iVerticesRow;			//ÿ�еĶ������
	int				m_iVerticesCol;			//ÿ�еĶ������
	int				m_iCellsRow;			//ÿ�еĸ�����
	int				m_iCellsCol;			//ÿ�еĸ�����
	int				m_iVertexNums;			//�ܵĵ��ζ�����
	FLOAT			m_fCellSpace;			//ÿ�����ӵĿռ䳤��
	FLOAT			m_fTerrainWidth;		//���εĿ��(X�ᣩ
	FLOAT			m_fTerrainDepth;		//���ε����(Z�ᣩ
	FLOAT			m_fTerrainScale;		//���εĹ�ģ�����ű���ϵ����
	vector<float>	m_vHeightInfo;			//����ĸ߶���Ϣ��Y�ᣩ

	struct TERRAINVERTEX 
	{
		float x, y, z;		//��������
		float u, v;			//������ͼ����

		TERRAINVERTEX(float _x, float _y, float _z, float _u, float _v) :
			x(_x), y(_y), z(_z), u(_u), v(_v){}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};

public:
	explicit TerrainClass(IDirect3DDevice9 * pd3dDevice9);	//���캯��
	~TerrainClass();										//��������

public:
	BOOL LoadTerrainFromFile(TCHAR * pFileName, TCHAR * pTextureName);		//������εĸ߶�ͼ������
	HRESULT TerrainInit(FLOAT fPicNums, int iVerticeRow, int iVerticesCol,
		int iVerticesRowTrue,  FLOAT fCellSpace, FLOAT fTerrainScale = 8.0); //��������ĳ�ʼ��
	VOID TerrainRender(D3DXMATRIX * matWorld, BOOL bWireFrame);       //���ε���Ⱦ
};