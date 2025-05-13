#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CNavigation; 
END


class CSC_Object_StoneBall final : public CGameObject
{
public:
	struct STONE_BALL_DESC: public CGameObject::GAMEOBJECT_DESC
	{
		_float startTime;	
		_float LifeTime;
		_wstring  EventName;
	};

private:
	CSC_Object_StoneBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSC_Object_StoneBall(const CSC_Object_StoneBall& Prototype);
	virtual ~CSC_Object_StoneBall() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;	

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

	_float   m_fTimeDelta = { 0.f };
	_float   m_fMoveHeightDistance = { 211.f };


	_float m_fstartTime = {0.f};
	_float m_fAccTimeDelta = { 0.f };
	_float m_fLifeTime = {0.f};	
	

	_float4 m_fLookDir = {};	
	_wstring m_wEventName = {};

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void IsTriggerOn(_wstring _EventName);


public:
	static CSC_Object_StoneBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



