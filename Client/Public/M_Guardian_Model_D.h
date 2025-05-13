#pragma once

#include "Monster.h"

BEGIN(Engine)
class CTexture;	
END


class CMonsterHpBar; 
class CMonsterStunBar;
class CM_Guardian_Model_D_Arm;	


class CM_Guardian_Model_D final : public CMonster
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
	CM_Guardian_Model_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CM_Guardian_Model_D(const CM_Guardian_Model_D& Prototype);
	virtual ~CM_Guardian_Model_D() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;


private:
	CMonsterHpBar*   m_pMonsterHpBar   = { nullptr };	
	CMonsterStunBar* m_pMonsterStunBar = { nullptr };
	CM_Guardian_Model_D_Arm* m_pMonsterArm = { nullptr };

private:
	_bool  m_bStart = { false };
	_bool  m_bDead = { false };
	_float  m_fAccTimeDeltaDissove = { 0.f };


	CTexture* m_pTextureCom = { nullptr };	
public:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	static CM_Guardian_Model_D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

