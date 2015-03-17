/*-----------------------------------------------------------------
【Cpp文件】：TerrainClass.cpp    Create by Beyond Ray,2013年2月
（描述）：一个封装了地形高度图载入，初始化和渲染类的实现
-------------------------------------------------------------------*/

#include"TerrainClass.h"

//-------------------------------------------【TerrainClass()函数】----------------------------------------------
// Desc:构造函数
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

//-------------------------------------------【LoadTerrainFromFile()函数】---------------------------------------
// Desc:从文件中加载地形高度图和纹理
//---------------------------------------------------------------------------------------------------------------
BOOL TerrainClass::LoadTerrainFromFile(TCHAR * pFileName, TCHAR * pTextureName)
{
	ifstream infile;
	infile.open(pFileName, ios::binary);
	infile.seekg(0, ios::end);
	vector<BYTE> inData(infile.tellg());		//构造以字节为单位的数组并分配大小，与文件兼容

	infile.seekg(0, ios::beg);
	infile.read((char *)&inData[0], inData.size());		
	infile.close();								//读取文件中的地形高度数据,并关闭文件

	m_vHeightInfo.resize(inData.size());		//重新分配数组的容量

	for (unsigned int i = 0; i < inData.size(); i++)
	{
		m_vHeightInfo[i] = (FLOAT)inData[i];	//将数据格式转化为FLOAT，保存在成员变量中，以便缩放比例
	}
	
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice9, pTextureName, &m_pTexture9)))
		return FALSE;							//从文件中读取纹理信息
	return TRUE;					
}

//-------------------------------------------【TerrainInit()函数】----------------------------------------------
// Desc:，按设定比例改变地势高度，以及地形顶点数据和索引数据的设定
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

	//按比例改变地势高度
	for (int i = 0; i < m_vHeightInfo.size(); i++)
		m_vHeightInfo[i] *= fTerrainScale;

	//创建顶点缓冲区和索引缓冲区
	HR(m_pd3dDevice9->CreateVertexBuffer(m_iVertexNums * sizeof(TERRAINVERTEX), D3DUSAGE_WRITEONLY,
		TERRAINVERTEX::FVF, D3DPOOL_MANAGED,&m_pVertexBuffer9, NULL));

	HR(m_pd3dDevice9->CreateIndexBuffer(6 * m_iCellsCol * m_iCellsRow * sizeof(DWORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer9, NULL));

	//访问并写入顶点缓存
	TERRAINVERTEX * pVertices = NULL;
	HR(m_pVertexBuffer9->Lock(0, 0, (void**)&pVertices, 0));

	FLOAT fStartX, fStartZ, fEndX, fEndZ, fCoordU, fCoordV;
	fStartX = -m_fTerrainWidth / 2.0f;
	fStartZ = m_fTerrainDepth / 2.0f;
	fEndX = m_fTerrainWidth / 2.0f;
	fEndZ = -m_fTerrainDepth / 2.0f;
	fCoordU = fPicNums / m_iVerticesRow;		//纹理X坐标，从0到1
	fCoordV = fPicNums / m_iVerticesCol;		//纹理Y坐标，从0到1
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

	//访问并写入索引缓存
	
	WORD * pIndices = NULL;
	HR(m_pIndexBuffer9->Lock(0, 0, (void**)&pIndices, 0));

	iIndex = 0;
	for (int i = 0; i < m_iVerticesCol - 1; i++)			//遍历每行
	{
		for (int j = 0; j < m_iVerticesRow - 1; j++)		//遍历每列		
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

//-------------------------------------------【TerrainRender()函数】----------------------------------------------
// Desc:在该函数中进行贴图，渲染地势（可选择是否添加线框）注：用于除世界矩阵其他三大矩阵变换之后
//----------------------------------------------------------------------------------------------------------------
VOID TerrainClass::TerrainRender(D3DXMATRIX * matWorld, BOOL bWireFrame)
{
	//设置渲染流水线，设置灵活顶点格式，设置索引缓存
	m_pd3dDevice9->SetStreamSource(0, m_pVertexBuffer9, 0, sizeof(TERRAINVERTEX));
	m_pd3dDevice9->SetFVF(TERRAINVERTEX::FVF);
	m_pd3dDevice9->SetIndices(m_pIndexBuffer9);

	//关闭光照，并设置地形纹理
	m_pd3dDevice9->SetRenderState(D3DRS_LIGHTING, false);
	m_pd3dDevice9->SetTexture(0, m_pTexture9);

	//开始地形渲染
	m_pd3dDevice9->SetTransform(D3DTS_WORLD, matWorld);
	m_pd3dDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNums, 0, 2 * m_iCellsRow * m_iCellsCol);

	//开启光照，并清空纹理
	m_pd3dDevice9->SetRenderState(D3DRS_LIGHTING, true);
	m_pd3dDevice9->SetTexture(0, NULL);

	//判断是否添加线框，并执行相应操作
	if (bWireFrame)
	{
		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pd3dDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNums, 0, 2 * m_iCellsRow * m_iCellsCol);

		m_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//-------------------------------------------【~TerrainClass()函数】---------------------------------------------
// Desc:析构函数
//---------------------------------------------------------------------------------------------------------------
TerrainClass::~TerrainClass()
{
	SAFE_RELEASE(m_pTexture9);
	SAFE_RELEASE(m_pVertexBuffer9);
	SAFE_RELEASE(m_pIndexBuffer9);
}