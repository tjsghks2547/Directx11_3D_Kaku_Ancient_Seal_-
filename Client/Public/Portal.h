#pragma once

#include "Client_Defines.h"
#include "GameObject.h"	

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
class CCollider;	
END


class CPortal final : public CGameObject
{
private:
	CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPortal(const CPortal& Prototype);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void    SetParentPtr(CGameObject* _pMonster) { m_pParent = _pMonster; }

private:
	HRESULT Ready_Components();

	CGameObject* m_pParent = { nullptr };
	CTransform* m_pParentTrasnformCom = { nullptr };

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader*        m_pShaderCom   = { nullptr };
	CTexture*       m_pTextureCom  = { nullptr };
	CCollider*      m_pColliderCom = { nullptr };	

	_float4x4		m_BillMatrix = {};

	_int* m_iHp = { nullptr };
	_float m_fCurrentHpState = 0.f;

	_float m_fRadian = { 0.f };

	_bool  isDead = { false };
public:
	_bool Get_StateDead() { return isDead; }

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);	
	virtual void OnCollision(CGameObject* _pOther);	
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};