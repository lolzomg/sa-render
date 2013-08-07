#include "CDebug.h"
#include <stdio.h>

FILE *CDebug::m_pVertexDeclarationFile;

void CDebug::Initialize()
{
	// ��������/�������� ���� ������
#if defined(DEBUG_ENABLED)

    #if defined(DEBUG_VERTEX_DECLARATION)
	    m_pVertexDeclarationFile = fopen("sa_render_DebugVD.txt", "w");
    #endif

#endif
}

void CDebug::Shutdown()
{
	// �������� ���� ������
#if defined(DEBUG_ENABLED)

    #if defined(DEBUG_VERTEX_DECLARATION)
	    fclose(m_pVertexDeclarationFile);
	#endif

#endif
}

void CDebug::StoreVertexDeclaration(IDirect3DVertexDeclaration9 *declaration)
{
#if defined(DEBUG_ENABLED) && defined(DEBUG_VERTEX_DECLARATION)
	char temp[128];
	D3DVERTEXELEMENT9 decl[20];
	UINT numElements;
	declaration->GetDeclaration(decl, &numElements);
	for(int i = 0; i < numElements; i++)
	{
		sprintf(temp, "Stream: %3d Offset: %3d Type: %s Method: %s Usage: %s UsageIndex: %3d\n", decl[i].Stream, decl[i].Offset,
			gDeclTypes[decl[i].Type], gDeclMethods[decl[i].Method], gDeclUsages[decl[i].Usage], decl[i].UsageIndex);
		fputs(temp, m_pVertexDeclarationFile);
	}
	fputs("\n", m_pVertexDeclarationFile);
#endif
}

#if defined(DEBUG_ENABLED) && defined(DEBUG_VERTEX_DECLARATION)
// Vertex declaration stuff
char *gDeclTypes[] = {
	"FLOAT1   ",
	"FLOAT2   ",
    "FLOAT3   ",
    "FLOAT4   ",
    "D3DCOLOR ",
    "UBYTE4   ",
    "SHORT2   ",
    "SHORT4   ",
    "UBYTE4N  ",
    "SHORT2N  ",
    "SHORT4N  ",
    "USHORT2N ",
    "USHORT4N ",
    "UDEC3    ",
    "DEC3N    ",
    "FLOAT16_2",
    "FLOAT16_4",
    "UNUSED   "
};

char *gDeclMethods[] = {
	"DEFAULT         ",
	"PARTIALU        ",
    "PARTIALV        ",
    "CROSSUV         ",
    "UV              ",
    "LOOKUP          ",
    "LOOKUPPRESAMPLED"
};

char *gDeclUsages[] = {
	"D3DDECLUSAGE_POSITION    ",
	"D3DDECLUSAGE_BLENDWEIGHT ",
    "D3DDECLUSAGE_BLENDINDICES",
    "D3DDECLUSAGE_NORMAL      ",
    "D3DDECLUSAGE_PSIZE       ",
    "D3DDECLUSAGE_TEXCOORD    ",
    "D3DDECLUSAGE_TANGENT     ",
    "D3DDECLUSAGE_BINORMAL    ",
    "D3DDECLUSAGE_TESSFACTOR  ",
    "D3DDECLUSAGE_POSITIONT   ",
    "D3DDECLUSAGE_COLOR       ",
    "D3DDECLUSAGE_FOG         ",
    "D3DDECLUSAGE_DEPTH       ",
    "D3DDECLUSAGE_SAMPLE      "
};
#endif