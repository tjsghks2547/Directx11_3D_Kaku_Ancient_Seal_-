#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

class CBoss;

BEGIN(Client)

class CBossHpUi final : public CUIObject
{
private:
	CBossHpUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossHpUi(const CBossHpUi& Prototype);
	virtual ~CBossHpUi() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CShader* m_pShaderCom = { nullptr };	
	CTexture* m_pTextureCom = { nullptr };	
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };	
	CBoss* m_pBoss = { nullptr };	


	_tchar				m_szMessage[MAX_PATH] = {};
	_float				m_fAccTimeDelta = {};
	_float				m_fTimeDelta = {};
	_bool				m_bTriggerOnOff = { false };
	_bool               m_bQuestFinish = { false };

	_int*				m_pBossHp = { nullptr };
	_float				m_fCurrentHpState = 0.f;	

public:
	HRESULT Ready_Components();
	virtual void IsTriggerOn(_wstring _EventName);


public:
	static  CBossHpUi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END