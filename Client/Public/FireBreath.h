#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instancing;

END

BEGIN(Client)


class CFireBreath final : public CPartObject
{
public:
	struct PARTICLE_FIREBREATH_DESC : CPartObject::PARTOBJECT_DESC	
	{
		_float4    pStartPos;			
		const _float4x4* ParentSocketMarix;	 
	};
private:
	CFireBreath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFireBreath(const CFireBreath& Prototype);
	virtual ~CFireBreath() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Blur() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pAlphaTextureCom = { nullptr };
	CVIBuffer_Point_Instancing* m_pVIBufferCom = { nullptr };
	
	CTransform* m_pPlayerTransform = { nullptr };
	_float4 m_fStartPos = {};

	_float  m_fAccTimeDelta = {};
	

	/* dissolve 관련 */
	_float	m_fDissolveAmount = { 0.f };	
	_float	m_fAccTimeDeltaDissove = { 0.f };	

	_float  m_fTimeDelta = { 0.f };
	

	const _float4x4* m_pSokcetMatrix = {};	
	_float4x4  m_Matrix = {};
	 _float4x4 m_OffsetSokcetMatrix = {};	


	 _float2 m_vOffSet = {1.f/4.f, 1.f/6.f};			
	 _float  m_fDurationTime = { 0.f };
	 _uint   m_iframeIndex = 0; 

	 /* 2월 10일 작업 */
	 _float4x4 m_identityMatrix = {}; 
public:
	HRESULT Ready_Components();

public:
	static CFireBreath* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END