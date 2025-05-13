#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END


BEGIN(Client)


class CFadeInOut final : public CUIObject
{
private:
	CFadeInOut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFadeInOut(const CFadeInOut& Prototype);
	virtual ~CFadeInOut() = default;


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
	CGameObject* m_pPlayer = { nullptr };


	_tchar				m_szMessage[MAX_PATH] = {};
	_float				m_fAccTimeDelta = {};
	_float				m_fTimeDelta = {};
	_bool				m_bTriggerOnOff = { false };
	_bool               m_bQuestFinish = { false };

	_float				m_fAlpah = {};


public:
	HRESULT Ready_Components();
	virtual void IsTriggerOn(_wstring _EventName);


public:
	static  CFadeInOut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END