#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider; 
END

class CBullet final : public CGameObject
{
private:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CBullet() = default;	

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Blur() override; 

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom0 = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	_uint* m_pParentState = { nullptr }; 
	CGameObject* m_pPlayer = { nullptr };


private:
	_float4x4				m_BillMatrix = {};
	_float					m_fAccTimeDelta = { 0.f };
	_float					m_Scale = { 0.f };
	_float					m_fTimeDelta = { 0.f };
	_float					m_fAlpha = { 0.5f };

	_bool					m_bTest = { false };
	_float					m_fEffectLife = { 0.5f };

	_float4					m_fPosition = {};
	

	const _float4x4*		m_pSocketMatrix = {nullptr};			
	const _float4x4*        m_pParentWorldMatrix = { nullptr };	

	_float4x4				m_SocketMatrix = { };	
	_float4x4				m_CombinedWorldMatrix = {};
	

	_bool					m_Start = { false };
	_bool					m_bSettingDir = { false };
	_float4				    m_vDir = {};

	_float4					m_vBulletPos = {}; 
	_float					m_fBulletLifeTime = {}; 
public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);


public:
	HRESULT Ready_Components();	
	HRESULT Bind_ShaderResources();	

	HRESULT Effect_Activate();	
	void    Set_Poisition(_float4 _position) { m_fPosition = _position; }	
	void    Set_Socket_Matrix(const _float4x4* _pSocketMatrix) { m_pSocketMatrix = _pSocketMatrix; }
	void    Set_Parent_Matrix(const _float4x4* _pParentMatrix) { m_pParentWorldMatrix = _pParentMatrix; }	
	void    Set_Parent_State(_uint* _pParentState) { m_pParentState = _pParentState; }
	void    Set_Player_Ptr(CGameObject* pGameObject) { m_pPlayer = pGameObject; }
		
public:
	static  CBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
