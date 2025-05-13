#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CNavigation; 
class CCollider; 
END

BEGIN(Client)
class CPlayer;		


class CMonster abstract : public CGameObject
{
protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_int		 m_iHp = { 0 };	
	_uint		 m_iState = { 0 };	

protected:
	CShader*	 m_pShaderCom = { nullptr };
	CModel*		 m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CCollider*   m_pColliderCom = { nullptr };
	CGameObject* m_pPlayer = { nullptr };		

	const _float4x4* m_pRootMatrix = { nullptr };

	_float       m_fTimeDelta = {};
	_float		 m_fAccTimeDelta = {};
	_float		 m_fAnimationSpeed = {};
	_float		 m_DamagedDelay = {};	

	/* Dissolve 관련 효과 */
	_float		 m_fDissolveAmount = { 0.f };
	_float       m_fEdgeWidth = { 0.f };
	_float4      m_fEdgeColor = {};
	_float       m_fTime = {0.f};	

	_bool        m_bMonsterActive = { false };
			
public:
	virtual HRESULT Ready_Components() = 0;
	virtual HRESULT Bind_ShaderResources() = 0;

	

public:
	_int* Get_MonsterHp() { return &m_iHp; }

public:
	virtual void Free() override;

};
END