/*-----------------------------------------------------------------
��Cpp�ļ�����TerrainClass.cpp    Create by Beyond Ray,2013��2��
����������һ����װ�˵��θ߶�ͼ���룬��ʼ������Ⱦ���ʵ��
-------------------------------------------------------------------*/

#include"TerrainClass.h"

//-------------------------------------------��TerrainClass()������----------------------------------------------
// Desc:���캯��
//---------------------------------------------------------------------------------------------------------------
TerrainClass::TerrainClass(IDirect3DDevice9 * pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_pTexture9 = NULL;
	m_pVertexBuffer9 = NULL;
	m_pIndexBuffer9 = NULL;

	m_iVerticesRow = 0;
	m_iVerticesCol = 0;
	m_iCellsRow = 0;
	m_iCellsCol = 0;
	m_iVertexNums = 0;
	m_fTerrainWidth = 0.0f;
	m_fTerrainDepth = 0.0f;
	m_fTerrainScale = 0.0f;
	m_fCellSpace = 0.0f;
}

//-------------------------------------------��LoadTerrainFromFile()������---------------------------------------
// Desc:���ļ��м��ص��θ߶�ͼ������
//---------------------------------------------------------------------------------------------------------------
BOOL TerrainClass::LoadTerrainFromFile(TCHAR * pFileName, TCHAR * pTextureName)
{
	ifstream infile;
	infile.open(pFileName, ios::binary);
	infile.seekg(0, ios::end);
	vector<BYTE> inData(infile.tellg());		//�������ֽ�Ϊ��λ�����鲢�����С�����ļ�����

	infile.seekg(0, ios::beg);
	infile.read((char *)&inData[0], inData.size());		
	infile.close();								//��ȡ�ļ��еĵ��θ߶�����,���ر��ļ�

	m_vHeightInfo.resize(inData.size());		//���·������������

	for (unsigned int i = 0; i < inData.size(); i++)
	{
		m_vHeightInfo[i] = (FLOAT)inData[i];	//�����ݸ�ʽת��ΪFLOAT�������ڳ�Ա�����У��Ա����ű���
	}
	
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice9, pTextureName, &m_pTexture9)))
		return FALSE;							//���ļ��ж�ȡ������Ϣ
	return TRUE;					
}

//-------------------------------------------��TerrainInit()������----------------------------------------------
// Desc:�����趨�����ı���Ƹ߶ȣ��Լ����ζ������ݺ��������ݵ��趨
//--------------------------------------------------------------------------------------------------------------
HRESULT TerrainClass::TerrainInit(FLOAT fPicNums, int iVerticeRow, int iVerticesCol, int iVerticesRowTrue, FLOAT fCellSpace, FLOAT fTerrainScale)
{
	m_iVerticesRow = iVerticeRow;
	m_iVerticesCol = iVerticesCol;
	m_iCellsRow = iVerticeRow - 1;
	m_iCellsCol = iVerticesCol - 1;
	m_iVertexNums = iVerticeRow * iVerticesCol;
	m_fTerrainWidth = fCellSpace * m_iCellsRow;
	m_fTerrainDepth = fCellSpace * m_iCellsCol;
	m_fTerrainScale = fTerrainScale;
	m_fCellSpace = fCellSpace;

	//�������ı���Ƹ߶�
	for (int i = 0; i < m_vHeightInfo.size(); i++)
		m_vHeightInfo[i] *= fTerrainScale;

	//�������㻺����������������
	HR(m_pd3dDevice9->CreateVertexBuffer(m_iVertexNums * sizeof(TERRAINVERTEX), D3DUSAGE_WRITEONLY,
		TERRAINVERTEX::FVF, D3DPOOL_MANAGED,&m_pVertexBuffer9, NULL));

	HR(m_pd3dDevice9->CreateIndexBuffer(6 * m_iCellsCol * m_iCellsRow * sizeof(DWORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer9, NULL));

	//���ʲ�д�붥�㻺��
	TERRAINVERTEX * pVertices = NULL;
	HR(m_pVertexBuffer9->Lock(0, 0, (void**)&pVertices, 0));

	FLOAT fStartX, fStartZ, fEndX, fEndZ, fCoordU, fCoordV;
	fStartX = -m_fTerrainWidth / 2.0f;
	fStartZ = m_fTerrainDepth / 2.0f;
	fEndX = m_fTerrainWidth / 2.0f;
	fEndZ = -m_fTerrainDepth / 2.0f;
	fCoordU = fPicNums / m_iVerticesRow;		//����X���꣬��0��1
	fCoordV = fPicNums / m_iVerticesCol;		//����Y���꣬��0��1
	int iIndex = 0, i = 0, j = 0;
	for (float z = fStartZ; z >= fEndZ; z -= m_fCellSpace, i++)
	{
		j = 0;
		for (float x = fStartX; x <= fEndX; x += m_fCellSpace, j++)
		{
			pVertices[iIndex] = TERRAINVERTEX(x, m_vHeightInfo[i * iVerticesRowTrue+j], z,
				fCoordU * j, -fCoordV * i);
			iIndex++;
		}
	}

	m_pVertexBuffer9->Unlock();

	//���ʲ�д����������
	
	WORD * pIndices = NULL;
	HR(m_pIndexBuffer9->Lock(0, 0, (void**)&pIndices, 0));

	iIndex = 0;
	for (int i = 0; i < m_iVerticesCol - 1; i++)			//����ÿ��
	{
		for (int j = 0; j < m_iVerticesRow - 1; j++)		//����ÿ��		
		{
			pIndices[iIndex] = i * m_iVerticesRow + j;
			pIndices[iIndex + 1] = i * m_iVerticesRow + (j + 1);
			pIndices[iIndex + 2] = (i + 1) * m_iVerticesRow + (j + 1);
			pIndices[iIndex + 3] = i * m_iVerticesRow + j;
			pIndices[iIndex + 4] = (i + 1) * m_iVerticesRow + (j + 1);
			pIndices[iIndex + 5] = (i + 1)* m_iVerticesRow + j;
			iIndex += 6;
		}
	}
	m_pIndexBuffer9->Unlock();

	return S_OK;
}

//-------------------------------------------��TerrainRender()������----------------------------------------------
// Desc:�ڸú����н�����ͼ����Ⱦ���ƣ���ѡ���Ƿ�����߿�ע�����ڳ�������������������任֮��
//----------------------------------------------------------------------------------------------------------------
VOID TerrainClass::TerrainRender(D3DXMATRIX * matWorld, BOOL bWireFrame)
{
	//������Ⱦ��ˮ�ߣ����������ʽ��������������
	m_pd3dDevice9->SetStreamSource(0, m_pVertexBuffer9, 0, sizeof(TERRAINVERTEX));
	m_pd3dDevice9->SetFVF(TERRAINVERTEX::FVF);
	m_pd3dDevice9->SetIndices(m_pIndexBuffer9);

	//�رչ��գ������õ�������
	m_pd3dDevice9->SetRenderState(D3DRS_LIGHTING, false);
	m_pd3dDevice9->SetTexture(0, m_pTexture9);

	//��ʼ������Ⱦ
	m_pd3dDevice9->SetTransform(D3DTS_WORLD, matWorld);
	m_pd3dDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNums, 0, 2 * m_iCellsRow * m_iCellsCol);

	//�������գ����������
	m_pd3dDevice9->SetRenderState(D3DRS_LIGHTING, true);
	m_pd3dDevice9->SetTexture(0, NULL);

	//�ж��Ƿ�����߿򣬲�ִ����Ӧ����
	if (bWireFrame)
	{
		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pd3dDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNums, 0, 2 * m_iCellsRow * m_iCellsCol);

		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//-------------------------------------------��~TerrainClass()������---------------------------------------------
// Desc:��������
//---------------------------------------------------------------------------------------------------------------
TerrainClass::~TerrainClass()
{
	SAFE_RELEASE(m_pTexture9);
	SAFE_RELEASE(m_pVertexBuffer9);
	SAFE_RELEASE(m_pIndexBuffer9);
}