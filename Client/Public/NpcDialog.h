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


class CNpcDialog final : public CUIObject
{
private:
	CNpcDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpcDialog(const CNpcDialog& Prototype);
	virtual ~CNpcDialog() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CShader*        m_pShaderCom = { nullptr };
	CTexture*       m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_tchar			m_szMessage[MAX_PATH] = {};
	_tchar			m_szTemp[MAX_PATH] = {};
	_float			m_fAccTimeDelta = {}; 

	

public:
	HRESULT Ready_Components();


public:
	static  CNpcDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END