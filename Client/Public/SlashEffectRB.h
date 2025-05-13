#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CModel;
class CCollider;
END

class CSlashEffectRB final : public CGameObject
{
private:
	CSlashEffectRB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSlashEffectRB() = default;	

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Blur() override;

private:

private:

	_float			m_fAccTimeDelta = { 0.f };
	_float			m_fTimeDelta = { 0.f };
	_float			m_fAlpha = { 0.5f };

	_bool			m_bTest = { false };
	_float	        m_fEffectLife = { 0.5f };

	_float			m_fShaderAccTimeDelta = { 0.f };

	_float4x4       m_ParentWorldMatrix = {};
	_float4			m_fPosition = {};

	_bool			m_bFirst = { true };
	_float4x4		m_BillMatrix = {};
public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	HRESULT Ready_Components();	
	HRESULT Bind_ShaderResources();	
		
	void    Set_Poisition(_float4 _position) { m_fPosition = _position; }	
	void    Set_WorldMatrix(_float4x4 _Matrix) { m_ParentWorldMatrix = _Matrix; }	

	HRESULT PutInEffect(class CGameObject* _pEffect);


private:
	vector<class CGameObject*> m_vecSlash;	

public:
	static  CSlashEffectRB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);		
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override;	

};
