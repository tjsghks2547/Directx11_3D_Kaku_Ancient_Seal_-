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

class CPlayer;

class CS_HpFlower_Ui final : public CUIObject
{
private:
	CS_HpFlower_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CS_HpFlower_Ui(const CS_HpFlower_Ui& Prototype);
	virtual ~CS_HpFlower_Ui() = default;


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
	CPlayer* m_pPlayer = { nullptr };	


	_tchar				m_szMessage[MAX_PATH] = {};
	_uint				m_iCount = {};

public:

	HRESULT Ready_Components();


public:
	static  CS_HpFlower_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END