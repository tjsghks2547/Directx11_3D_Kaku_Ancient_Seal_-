#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CTexture;		

END
class CPortal;

BEGIN(Client)
class CTotemActionUi;


class CSC_TotemB final : public CGameObject	
{
private:
	CSC_TotemB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CSC_TotemB(const CSC_TotemB& Prototype);	
	virtual ~CSC_TotemB() = default;	
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*		  m_pShaderCom     = { nullptr };	
	CModel*			  m_pModelCom      = { nullptr };	
	CCollider*		  m_pColliderCom   = { nullptr };	
	CTotemActionUi*   m_pTotemActionUi = { nullptr };	
	CTexture*		  m_pTextureCom    = { nullptr };
	CPortal*		  m_pPortal = { nullptr };

	_float  m_fTimeDelta           = { 0.f };
	_float	m_fAccTimeDelta        = { 0.f };
	_float  m_fMoveDistance        = { 28.f };
	_float  m_fMoveHeightDistance  = { 130.f };

	_float	m_fDissolveAmount      = { 0.f };
	_float	m_fAccTimeDeltaDissove = { 0.f };

	_bool	 m_isCollision = { false };		
	_bool    m_bisStart = { false };

	_bool    m_bOnce = { true };
	_float   m_fEventTime = {};
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void IsTriggerOn(_wstring _EventName);

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CSC_TotemB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

