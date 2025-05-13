#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CModel;
class CCollider;

END


class CMoving_Floor final : public CGameObject
{
private:
	CMoving_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CMoving_Floor(const CMoving_Floor& Prototype);
	virtual ~CMoving_Floor() = default;	
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom     = { nullptr };	
	CModel*  m_pModelCom      = { nullptr };	
	CCollider* m_pColliderCom = { nullptr };

	_float  m_fTimeDelta = { 0.f };
	_float  m_fMoveDistance = { 28.f };
	_float  m_fMoveHeightDistance = { 211.f };

	_bool   m_bEventEnd = { true };
	_bool   m_bFirstEventEnd = { false };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void IsTriggerOn(_wstring _EventName);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CMoving_Floor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



