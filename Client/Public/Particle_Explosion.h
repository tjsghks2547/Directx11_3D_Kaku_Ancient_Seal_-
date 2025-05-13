#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader; 
class CTexture; 
class CVIBuffer_Point_Instancing;	 
END

BEGIN(Client)

class CParticle_Explosion final : public CPartObject
{
public:
	struct PARTICLE_EXPLOSION_DESC : CPartObject::PARTOBJECT_DESC
	{

	};
private:
	CParticle_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Explosion(const CParticle_Explosion& Prototype);
	virtual ~CParticle_Explosion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instancing* m_pVIBufferCom = { nullptr };



public:
	HRESULT Ready_Components();

public:
	static CParticle_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END