#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END


class CBoss_Left_Arm : public CGameObject
{
public:	
	struct BODY_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
	};

public:
	enum STATE
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
	CBoss_Left_Arm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBoss_Left_Arm() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Attack_State_Ptr(_bool* _iState) { m_iAttackState = _iState; }	
	void Set_Parent_State_Ptr(_uint* _iState) { m_iState = _iState; }
	void SetParentWorldMatrix(const _float4x4* _pParentWorldMatrix) { m_pParentMatrix = _pParentWorldMatrix; }	

private:
	_uint* m_iState = { nullptr };	
	_bool* m_iAttackState = { nullptr };	
	_uint  m_iPreState = {};
	_bool  m_isDamage = false;	
	_float m_fPatternTimeEffect = { false };
	_bool  m_bEffectTimeStart = { false };

private:
	_float     m_fAccTimeDelta = { 0.f };	
	CShader*   m_pShaderCom = { nullptr };	
	CCollider* m_pColliderAttackCom = { nullptr };	

	const _float4x4* m_pParentMatrix = {};		
	const _float4x4* m_pSocketMatrix = {};	
	_float4x4 m_CombinedWorldMatrix = {};	

public:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	static CBoss_Left_Arm* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

