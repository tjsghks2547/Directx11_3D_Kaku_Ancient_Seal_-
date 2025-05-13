#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END


class CTemple_Ground final : public CGameObject
{
private:
	CTemple_Ground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTemple_Ground(const CTemple_Ground& Prototype);
	virtual ~CTemple_Ground() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*   m_pShaderCom   = { nullptr };
	CModel*    m_pModelCom    = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	
	_float     m_fMoveDistance = { 0.f };
	_float	   m_fTimeDelta    = { 0.f };
	_float	   m_fAccTimeDelta = { 0.f };


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

	virtual void IsTriggerOn(_wstring _EventName);	

public:
	static CTemple_Ground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

