
#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END


class CM_Guardian_Model_D_Arm : public CGameObject
{
public:
	enum STATE
	{
		STATE_IDLE = 1,
		STATE_WALK = 2,
		STATE_ATTACK = 3,
		STATE_ATTACK2 = 4,
		STATE_DEAD = 5,
	};

private:
	CM_Guardian_Model_D_Arm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CM_Guardian_Model_D_Arm() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Parent_State_Ptr(_uint* _iState) { m_iState = _iState; }
	void SetTransform(CTransform* _pTransform) { m_pTransformCom = _pTransform; }

private:
	_uint* m_iState = { nullptr };
	_bool  m_isDamage = false;

private:
	_float     m_fAccTimeDelta = { 0.f };	
	CShader*   m_pShaderCom = { nullptr };
	CCollider* m_pColliderAttackCom = { nullptr };

public:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	static CM_Guardian_Model_D_Arm* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

