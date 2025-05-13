#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instancing;

END

BEGIN(Client)


class CItemGainParticle final : public CPartObject
{
public:
	struct PARTICLE_ITEMGAIN_DESC : CPartObject::PARTOBJECT_DESC
	{
		_float4  pStartPos;	
		_float3* pCurvePos; 
		class CTransform* pPlayerTransform;	
	};
private:
	CItemGainParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemGainParticle(const CItemGainParticle& Prototype);
	virtual ~CItemGainParticle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Blur() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instancing* m_pVIBufferCom = { nullptr };
	//CGameObject* m_pPlayer = { nullptr };		
	CTransform* m_pPlayerTransform = { nullptr };
	_float4 m_fStartPos = {};

	_float  m_fAccTimeDelta = {}; 

public:
	HRESULT Ready_Components();

public:
	static CItemGainParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END