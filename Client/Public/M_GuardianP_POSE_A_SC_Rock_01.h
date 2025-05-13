#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CM_GuardianP_POSE_A_SC_Rock_01 final : public CGameObject
{
private:
	CM_GuardianP_POSE_A_SC_Rock_01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CM_GuardianP_POSE_A_SC_Rock_01(const CM_GuardianP_POSE_A_SC_Rock_01& Prototype);
	virtual ~CM_GuardianP_POSE_A_SC_Rock_01() = default;
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

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CM_GuardianP_POSE_A_SC_Rock_01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



