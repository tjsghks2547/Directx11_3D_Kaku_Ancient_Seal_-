#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;	
class CShader;
class CModel;
END

BEGIN(Client)

class CLeverButton final : public CGameObject	
{

private:
	CLeverButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLeverButton(const CLeverButton& Prototype);
	virtual ~CLeverButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = { nullptr };	
	CShader*   m_pShaderCom   = { nullptr };	
	CModel*    m_pModelCom    = { nullptr };	

	_float     m_fAnimationSpeed = { 0.0f };
	_float     m_fTimeDelta = { 0.f };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CLeverButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END