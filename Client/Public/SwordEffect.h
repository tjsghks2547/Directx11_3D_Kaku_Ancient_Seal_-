#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CSwordTrail_Buffer;	
class CShader;	
class CTexture; 
END

class CSwordEffect final : public CGameObject
{
public:
	struct TrailPoint
	{
		_float3	tip;  // 검 끝 위치 
		_float3 hilt; // 검 손잡이 위치
		_float  age;  // 생성된 후 시간 ( 혹은 프레임 카운트 ) 
	};

	struct TrailVertex	
	{
		_float3 position;		
		_float2 texcoord;		
		float age;		
	};


private:
	CSwordEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CSwordEffect() = default;	

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*			m_pShaderCom     = { nullptr };	
	CSwordTrail_Buffer* m_pViBufferCom   = { nullptr };
	CTexture*			m_pTextureCom    = { nullptr };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

private:	
	vector<TrailPoint>  m_vecTrailPoint;	 
	vector<TrailVertex>	m_vecVertices; 
	vector<_uint>       m_vecIndices; 


	_uint				m_NumVertices = { 0 };
	_uint				m_NumIndices  = { 0 };




public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Add_Trail_Point(TrailPoint _pTrailPoint);
	void    ReSet_Trail();

public:
	static  CSwordEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override; 

};

