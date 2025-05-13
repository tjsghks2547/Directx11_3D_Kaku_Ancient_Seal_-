#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "InterActionUi.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END


class CTurnTable_Head final : public CGameObject
{
private:
	CTurnTable_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTurnTable_Head(const CTurnTable_Head& Prototype);
	virtual ~CTurnTable_Head() = default;
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

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


private:
	_bool	 m_isCollision    = { false };
	_float   m_fTimeDelta    = { 0.f };
	_float   m_fAngle = { 0.f };
	_float   m_fAccTimeDelta = { 0.f };
	_float   m_fAccTimeDeltaHandle2 = { 0.f };
	_float   m_fAccTimeDeltaHandle3 = { 0.f };

	_float  m_fMoveHeightDistance = { 211.f };


	_bool   m_bEventEnd = { true };	


	_bool   m_bStage_1 = { false };
	_bool   m_bStage_2_Event2 = { false };
	_bool   m_bStage_3 = { false };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	virtual void IsTriggerOn(_wstring _EventName);		

private:
	CInterActionUi* m_pInterActionUi = { nullptr };

public:
	static CTurnTable_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



