#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;	
END


class CSC_Stone_HeavyBOX_A_Mod final : public CGameObject
{
private:
	CSC_Stone_HeavyBOX_A_Mod(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSC_Stone_HeavyBOX_A_Mod(const CSC_Stone_HeavyBOX_A_Mod& Prototype);
	virtual ~CSC_Stone_HeavyBOX_A_Mod() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*   m_pShaderCom     = { nullptr };
	CModel*    m_pModelCom      = { nullptr };
	CCollider* m_pColliderCom   = { nullptr };

	_float     m_fTimeDelta = { 0.f };
	_float     m_fMoveDistance = { 28.f };
	_float     m_fMoveHeightDistance = { 27.f };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

	virtual void IsTriggerOn(_wstring _EventName) override;

public:
	static CSC_Stone_HeavyBOX_A_Mod* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



