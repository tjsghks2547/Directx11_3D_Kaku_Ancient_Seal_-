#pragma once

#include "Client_Defines.h"
#include "GameObject.h"	

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END


class CMonsterStunBar final : public CGameObject
{
private:
	CMonsterStunBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterStunBar(const CMonsterStunBar& Prototype);
	virtual ~CMonsterStunBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void    SetMonsterPtr(CGameObject* _pMonster) { m_pMonster = _pMonster; }

private:
	HRESULT Ready_Components();

	CGameObject* m_pMonster = { nullptr };
	CTransform* m_pMonsterTrasnformCom = { nullptr };

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float4x4		m_BillMatrix = {};

public:
	static CMonsterStunBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};