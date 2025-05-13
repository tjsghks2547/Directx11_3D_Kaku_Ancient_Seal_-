#pragma once

#include "Client_Defines.h"
#include "GameObject.h"	

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CNpcQuestionUi final : public CGameObject
{
private:
	CNpcQuestionUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpcQuestionUi(const CNpcQuestionUi& Prototype);
	virtual ~CNpcQuestionUi() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void    SetNpcPtr(CGameObject* _pNpc) { m_pNpc = _pNpc; }	

private:
	HRESULT Ready_Components();

	CGameObject* m_pNpc = { nullptr };
	CTransform* m_pNpcTrasnformCom = { nullptr };

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float4x4		m_BillMatrix = {};



public:
	static CNpcQuestionUi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END	