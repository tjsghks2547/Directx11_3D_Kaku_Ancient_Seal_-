#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CModel; /* 루트 모션 때매 추가 */
class CCollider;	
class CNavigation;	
END


BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum STATE
	{
		STATE_IDLE		= 0x00000001,	
		STATE_WALK		= 0x00000002,	
		STATE_ATTACK	= 0x00000004,	
		STATE_ATTACK2	= 0x00000008,	
		STATE_JUMP      = 0x00000010,	
		STATE_JUMP2     = 0x00000020,	
		STATE_RUN       = 0x00000040,	
		STATE_ROLL      = 0x00000080,	
		STATE_HURT      = 0x00000100,
		STATE_FALLING   = 0x00000200,	
		STATE_PUSH      = 0x00000400,	
		STATE_PULL      = 0x00000800,	
		STATE_COMBOATTACK = 0x00001000,
	};


private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default; 

public:
	virtual HRESULT Initialize_Prototype() override;	
	virtual HRESULT Initialize(void* pArg) override;	
	virtual void Priority_Update(_float fTimeDelta) override;	
	virtual void Update(_float fTimeDelta) override;	
	virtual void Late_Update(_float fTimeDelta) override;	
	virtual HRESULT Render() override;	
	
public:
	HRESULT Ready_Components();	
	HRESULT Ready_PartObjects();	

private:
	_uint								m_iState         = { STATE_IDLE };
	_uint								m_iPreState		 = { STATE_IDLE };
	



	CNavigation*						m_pNavigationCom = { nullptr };	
	CCollider*							m_pColliderCom   = { nullptr };
	CModel*								m_pModel         = { nullptr };
	const _float4x4*					m_pRootMatrix    = { nullptr };
	const _float4x4*					m_CombinedMatrix = { nullptr };


	_bool								m_bIsRootMotionApplied = { false };
	_float4x4 					        m_CurrentTransformWorldMatrix = {};
	_float4x4 							m_FinalTransformWorldMatrix   = {};	

	_bool								m_bRootMotion  = { false };
	_uint								m_iAttackComboCount = { 0 };


	_bool								m_bFalling = { false };
	_bool								m_bGround = { true };

	_float								m_fFootStepTime = {}; 
	_bool								m_bFootStep1Foot_1_time = {true}; 
	_bool								m_bFootStep1Foot_2_time = {true};

private:
	_float	m_fGravity = { 9.8f };	
	_float  m_fJumpPower = { 3.5f };
	//_float  m_fJumpPower = { 10.f };	
	_float  m_fJumpTime = { 0.0f };	

	_float  m_fmaxFallSpeed = { 3.f };

	_int		m_iHp = { 0 };		
	_float    m_fStamina = { 0 };	

	_float  m_fTimeDelta = { 0.f };

	// 테스트 
	_int    m_iTestACount = { 0 };
	_int    m_iTestBCount = { 0 };


	_bool   m_bMonsterActive = { false };

public:
	_int*	Get_PlayerHp() { return &m_iHp; }	
	_float* Get_PlayerStamina() { return &m_fStamina; }
	_bool   Get_MonsterActivateOnOff() { return m_bMonsterActive; }		
	void  Control_PlayerHp(_int _hp)
	{
		m_iHp += _hp;
	}

	void  Control_PlayerStamina(_int _Stamina)
	{
		m_fStamina += _Stamina;	
	}

	

	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override;	

};

END