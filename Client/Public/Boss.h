#pragma once

#include "Monster.h"

BEGIN(Engine)
class CTexture;
END

class CBoss_Left_Arm;	
class CBoss_Right_Arm;		

class CBoss final : public CMonster
{
public:
	enum BOSS_STATE
	{
		STATE_IDLE,
		STATE_ATTACK_PARTTERN_1,
		STATE_ATTACK_PARTTERN_2,
		STATE_ATTACK_PARTTERN_3,
		STATE_ATTACK_PARTTERN_4,
		STATE_ATTACK_AL,
		STATE_ATTACK_AR,
		STATE_ATTACK_BL,
		STATE_ATTACK_BR,
		STATE_ATTACK_DOUBLE_PUNCH,
		STATE_DEAD,
	};


private:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss(const CBoss& Prototype);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	_bool     m_bStart = { false };
	_bool     m_bDead = { false };
	_float    m_fAccTimeDeltaDissove = { 0.f };
	ULONGLONG m_dwtime = {};

	CTexture* m_pTextureCom = { nullptr };
	_bool      m_bBossStart = { false };
	_bool      m_bBossAttackOnOff = { false };

	CBoss_Left_Arm* m_pBoss_Left_Arm = { nullptr };
	CBoss_Right_Arm* m_pBoss_Right_Arm = { nullptr };

	_float     m_fPatternTimeAcc = { };
	_float     m_fBreathTimeAcc = {};
	_float     m_fBreathIngTiem = {};


	_uint      m_iPatternIndex = {};

public:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	_bool Get_BossStart() { return m_bBossStart; }


public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

