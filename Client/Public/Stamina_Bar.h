
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

class CStamina_Bar final : public CUIObject
{
private:
	CStamina_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStamina_Bar(const CStamina_Bar& Prototype);
	virtual ~CStamina_Bar() = default;


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

	_float* m_fStamina = { nullptr };	
	_float m_fCurrentStaminaState = 0.f;				
public:
	HRESULT Ready_Components();


public:
	static  CStamina_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END