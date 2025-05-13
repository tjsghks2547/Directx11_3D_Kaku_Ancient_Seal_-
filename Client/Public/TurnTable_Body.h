#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CTurnTable_Body final : public CGameObject
{
private:
	CTurnTable_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTurnTable_Body(const CTurnTable_Body& Prototype);
	virtual ~CTurnTable_Body() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel*  m_pModelCom = { nullptr };

	_float   m_fTimeDelta = { 0.f };
	_float   m_fMoveHeightDistance = { 211.f };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void IsTriggerOn(_wstring _EventName);


public:
	static CTurnTable_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



