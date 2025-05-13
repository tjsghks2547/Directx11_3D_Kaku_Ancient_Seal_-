#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)

class CBulletGainEffectParticle final : public CPartObject
{
public:
	struct PARTICLE_EXPLOSION_DESC : CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* ParentSocketMarix;	
		const _uint* pParentState;	
	};
private:
	CBulletGainEffectParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBulletGainEffectParticle(const CBulletGainEffectParticle& Prototype);
	virtual ~CBulletGainEffectParticle() = default;

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

	const _float4x4* m_pSocketMatrix = { nullptr };	
	_float4x4		 m_SocketMatrix = {};
	const _uint*			 m_pParentState = {};
	


public:
	HRESULT Ready_Components();
	

public:
	static CBulletGainEffectParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END