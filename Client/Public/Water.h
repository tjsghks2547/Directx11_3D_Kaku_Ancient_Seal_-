#pragma once	
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
END


BEGIN(Client)
class CWater final : public CGameObject
{
private:
	CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWater(const CWater& Prototype);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;

	/* 10월 31일 추가*/
	virtual HRESULT Ready_Component();

	/* 1월 20일 추가 */
	virtual void IsTriggerOn(_wstring _EventName);

private:
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

	/* 11월 16일 임시*/
private:
	VTXNORTEX* pVertices = { nullptr };
	_uint* pIndices = { nullptr };
	bool       m_btest = { true };
	size_t     m_iTotalTriangle = {};
	CVIBuffer_Terrain* pTerrain = { nullptr };
	bool	   m_isButtonPressed = { false };

	/* 11월 15일 임시 */
private:
	ID3D11Texture2D* m_pTexture2D = { nullptr };
	_uint							m_iWinSizeX, m_iWinSizeY;

	_float							m_fTimeDelta = {};
	_float							m_famplitude = {};
	_float							m_fwavelength = {};
	_float							m_fSpeed = {};

	_float							m_faccumulatedTime = {};
	_float							m_fHeight = {};
	_float							m_fRealTimeDelta = {};
	_float							m_fHeightDistance = { 28.f };



public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
