#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;	
END


class CSM_LotusFlower_07a1 final : public CGameObject	
{
private:
	CSM_LotusFlower_07a1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSM_LotusFlower_07a1(const CSM_LotusFlower_07a1& Prototype);
	virtual ~CSM_LotusFlower_07a1() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*   m_pShaderCom   = { nullptr };
	CModel*	   m_pModelCom    = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_float     m_fTimeDelta   = { 0.f };


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	/* 1월 20일 추가 */
	virtual void IsTriggerOn(_wstring _EventName) override;	

public:
	static CSM_LotusFlower_07a1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



