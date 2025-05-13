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


class CItem_UI_ButtonE final : public CUIObject
{
private:
	CItem_UI_ButtonE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_UI_ButtonE(const CItem_UI_ButtonE& Prototype);
	virtual ~CItem_UI_ButtonE() = default;


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


public:
	HRESULT Ready_Components();


public:
	static  CItem_UI_ButtonE* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END