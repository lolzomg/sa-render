#pragma once
#include "CRender.h"
#include "D3DDeviceReset.h"
#include "CNormalsGenerator.h"
#include "CTextureMaps.h"
#include "CVehicleRender.h"
#include "CDeferredRendering.h"
#include "CObjectRender.h"
#include "CPedsRender.h"
#include "CPatch.h"
#include "CImmediateRender.h"
#include "CLights.h"
#include "CSkyRender.h"
#include "CGrassRender.h"
#include "CGame.h"
#pragma comment(lib, "D3DDeviceReset.lib")

D3DXMATRIX CRender::m_mTransform;
IDirect3DTexture9 *CRender::defnormal;
IDirect3DTexture9 *CRender::defspec;

void CRender::Patch()
{
	DefineReset();
	CPatch::RedirectCall(0x5BD765, Setup);
	CNormalsGenerator::Patch();
	CTextureMaps::Patch();
	CVehicleRender::Patch();
	CObjectRender::Patch();
	CDeferredRendering::Patch();
	CPedsRender::Patch();
	CLights::Patch();
	CImmediateRender::Patch();
}

void CRender::Setup()
{
	D3DXCreateTextureFromFile(g_Device,"normal.tga",&defnormal);
	D3DXCreateTextureFromFile(g_Device,"spec.tga",&defspec);
	CDeferredRendering::Setup();
	CVehicleRender::Setup();
	CObjectRender::Setup();
	CPedsRender::Setup();
	CSkyRender::Setup();
	CGrassRender::Setup();
}

void CRender::DefineReset()
{
	D3DDeviceResetAdd(BeforeReset, true);
	D3DDeviceResetAdd(AfterReset, false);
}

void CRender::BeforeReset()
{
	CVehicleRender::Lost();
	CDeferredRendering::Lost();
	CObjectRender::Lost();
	CPedsRender::Lost();
	CGrassRender::Lost();
}

void CRender::AfterReset()
{
	CVehicleRender::Reset();
	CDeferredRendering::Reset();
	CObjectRender::Reset();
	CPedsRender::Reset();
	CGrassRender::Reset();
}

D3DXMATRIX *__cdecl CRender::_getComposedMatrix(D3DXMATRIX *m_out)
{
	D3DXMATRIX viewproj; // eax@1
	D3DXMATRIX *result; // eax@4
	D3DXMATRIX view,proj;
	g_Device->GetTransform(D3DTS_VIEW,&view);
	g_Device->GetTransform(D3DTS_PROJECTION,&proj);
	D3DXMatrixMultiply(&viewproj,&view,&proj);
	if ( D3D9ActiveTransform )
		result = D3DXMatrixMultiplyTranspose(m_out, D3D9Transform, &viewproj);
	else
		result = D3DXMatrixTranspose(m_out, &viewproj);

	return result;
}

void CRender::SetTextureMaps(STexture* tex, ID3DXEffect* effect) {
	IDirect3DBaseTexture9 *diffuse,*bump,*specular;
	rwD3D9SetTexture(tex, 0);
	g_Device->GetTexture(0,&diffuse);
	effect->SetTexture("gtDiffuse",diffuse);
	if(tex){
		if(tex->m_pNormalMap)
		{
			rwD3D9SetTexture(tex->m_pNormalMap, 1);
			g_Device->GetTexture(1,&bump);
			effect->SetTexture("gtNormals",bump);
			if(tex->m_pSpecMap) {
					rwD3D9SetTexture(tex->m_pSpecMap, 2);
					g_Device->GetTexture(2,&specular);
					effect->SetTexture("gtSpecular",specular);
			} else {
					effect->SetTexture("gtSpecular",defspec);
			}
		} else {
				if(tex->m_pSpecMap) {
					rwD3D9SetTexture(tex->m_pSpecMap, 2);
					g_Device->GetTexture(2,&specular);
					effect->SetTexture("gtSpecular",specular);
				} else {
					effect->SetTexture("gtSpecular", defspec);
				}
				effect->SetTexture("gtNormals", defnormal);
		}
	}
}