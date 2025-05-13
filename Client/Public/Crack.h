#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CVIBuffer_Rect;
END

class CCrack final : public CGameObject	
{
private:
	CCrack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CCrack() = default;	

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom0 = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


private:
	_float4x4		m_BillMatrix = {};
	_float			m_fAccTimeDelta = { 3.f };
	_float			m_fTimeDelta = { 0.f };
	_float			m_fAlpha = { 0.5f };

	_bool			m_bTest = { false };
	_float	        m_fEffectLife = { 0.5f };

	_float4			m_fPosition = {};


	/* 스프라이트 관련 맴버변수들*/
	_uint			m_iFrame    = {};
	_float2         m_vOffSet   = {}; 
	_float			m_fDuration = {};
	_uint			m_iCountX = {}; 
	_uint			m_iCountY = {}; 
	_uint           m_iTotalFrame = {}; 

	_float2         m_vTexcoord = {}; 


public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Effect_Activate();
	void    Set_Poisition(_float4 _position) { m_fPosition = _position; }
	


public:
	static  CCrack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
