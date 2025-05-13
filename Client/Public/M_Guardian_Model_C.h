#pragma once

#include "Monster.h"

BEGIN(Engine)
class CTexture;		
END


class CMonsterHpBar;		
class CMonsterStunBar;		
class CM_Guardian_Model_C_Arm;	


class CM_Guardian_Model_C final : public CMonster	
{
public:	
	enum STATE
	{
		STATE_IDLE = 1,
		STATE_WALK = 2,
		STATE_ATTACK = 3,
		STATE_ATTACK2 = 4,
		STATE_DEAD = 5,
		STATE_ASSMBLE = 6,	
	};

private:
	CM_Guardian_Model_C(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CM_Guardian_Model_C(const CM_Guardian_Model_C& Prototype);
	virtual ~CM_Guardian_Model_C() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

private:
	CTexture* m_pTextureCom = { nullptr };

private:
	_bool  m_bStart = { false };	
	_bool  m_bDead = { false };
	CM_Guardian_Model_C_Arm* m_pMonsterArm = { nullptr };	
	_float  m_fAccTimeDeltaDissove = { 0.f };
private:
	CMonsterHpBar*   m_pMonsterHpBar = { nullptr };		
	CMonsterStunBar* m_pMonsterStunBar = { nullptr };		

public:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);	
	virtual void OnCollisionExit(CGameObject* _pOther);
		

public:
	static CM_Guardian_Model_C* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

