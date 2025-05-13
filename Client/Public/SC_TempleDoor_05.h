#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CSC_TempleDoor_05 final : public CGameObject
{
private:
	CSC_TempleDoor_05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSC_TempleDoor_05(const CSC_TempleDoor_05& Prototype);
	virtual ~CSC_TempleDoor_05() = default;	
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	_float  m_fTimeDelta = { 0.f };
	_float  m_fMoveDistance = { 28.f };
	_float  m_fMoveHeightDistance = { 55.f };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void IsTriggerOn(_wstring _EventName);


public:
	static CSC_TempleDoor_05* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



