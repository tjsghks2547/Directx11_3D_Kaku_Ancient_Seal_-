#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CTexture; 

END

BEGIN(Client)	

class CItem_UI_ButtonE;		

END

class CSM_MushroomA_L final : public CGameObject
{
private:
	CSM_MushroomA_L(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSM_MushroomA_L(const CSM_MushroomA_L& Prototype);
	virtual ~CSM_MushroomA_L() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*    m_pShaderCom   = { nullptr };
	CModel*     m_pModelCom    = { nullptr };
	CCollider*  m_pColliderCom = { nullptr };
	CTexture*   m_pTextureCom  = { nullptr };
	CGameObject* m_pPlayer	   = { nullptr };

	_float  m_fTimeDelta = { 0.f };
	_float	m_fAccTimeDelta = { 0.f };
	_float  m_fMoveDistance = { 28.f };
	_float  m_fMoveHeightDistance = { 130.f };

	CItem_UI_ButtonE* m_pItemUi = { nullptr };
	_bool			  m_isCollision = { false };


	/* dissolve ฐüทร */
	_float	m_fDissolveAmount = { 0.f };	
	_float	m_fAccTimeDeltaDissove = { 0.f };	


	_bool   m_bEffectOnOff = { false };
	_bool   m_bIconOnOff = { false };
			
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void IsTriggerOn(_wstring _EventName);

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CSM_MushroomA_L* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



