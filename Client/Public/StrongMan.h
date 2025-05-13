#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END


BEGIN(Client)

class CNpcInterActionUi;
class CNpcQuestionUi;
class CNpcDialog;	

class CStrongMan final : public CGameObject
{

private:
	CStrongMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStrongMan(const  CStrongMan& Prototype);
	virtual ~CStrongMan() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CShader*   m_pShaderCom = { nullptr };
	CModel*    m_pModelCom = { nullptr };

	_float     m_fAnimationSpeed = { 0.0f };
	_float     m_fTimeDelta = { 0.f };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


private:
	_bool	 m_isCollision      = { false };	
	_bool    m_bDialogUiOnOff   = { false }; // 이거 false로 수정하기 
	_bool    m_bQuestionUiOnOff = { true };

	CNpcInterActionUi*  m_pInterActionUi = { nullptr };	
	CNpcQuestionUi*		m_pNpcQuestionUi = { nullptr };
	CNpcDialog*			m_pNpcDialog     = { nullptr };


public:
	static  CStrongMan* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END