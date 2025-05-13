#include "pch.h"
#include "Water.h"
#include "GameInstance.h"


CWater::CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CWater::CWater(const CWater& Prototype)
	:CGameObject(Prototype)
	, m_fTimeDelta(0.f)
{
}

HRESULT CWater::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	if(m_szName == nullptr)
		strcpy_s(m_szName, "Prototype_GameObject_Water");

	_char* NoName = " ";	

	if (pArg != nullptr)
	{
		CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
		{
			if(strcmp(pObjectDesc->szName, NoName))	
				strcpy_s(m_szName, pObjectDesc->szName);
		}
		
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_famplitude = 0.5f;
	m_fwavelength = 3.0f;
	m_fSpeed = 0.2f;


	if (!strcmp(m_szName, "Water_Stage_01"))
	{
		if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_1"), this)))	
			return E_FAIL;	
	}

	if (!strcmp(m_szName, "Water_Stage_02"))
	{
		if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Event2"), this)))
			return E_FAIL;
	}


	return S_OK;
}

void CWater::Priority_Update(_float fTimeDelta)
{
	int  a = 10;
}

void CWater::Update(_float fTimeDelta)
{
	m_faccumulatedTime += fTimeDelta;
}

void CWater::Late_Update(_float fTimeDelta)
{
	m_fTimeDelta += fTimeDelta * 0.1f;

	m_fRealTimeDelta = fTimeDelta;

	if (m_bTriggerOnOff)
		IsTriggerOn(TEXT("Stage_1"));	

	if (m_bTriggerEvent2OnOff)
		IsTriggerOn(TEXT("Stage_2_Event2"));	


	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	
	
}

HRESULT CWater::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Bind_InputAssembler();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CWater::Ready_Component()
{
	/* 버퍼 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, TEXT("Terrain_Com"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* 셰이더 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Water")
		, TEXT("Shader_Com"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* 텍스처 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Water_Texture")
		, TEXT("Texture_Com"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;



	return S_OK;
}

void CWater::IsTriggerOn(_wstring _EventName)
{

	/* 최초 들어올시 */
	if (!strcmp(m_szName, "Water_Stage_01") && _EventName == TEXT("Stage_1"))
	{

		m_bTriggerOnOff = true;


		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (vCurPos.m128_f32[1] < -185.f)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fRealTimeDelta* 3.f));
		else
			m_bTriggerOnOff = false;
	}
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fRealTimeDelta));

	// y값 -195면 stop하기 m_bTriggerOnOff = false로 


	if (!strcmp(m_szName, "Water_Stage_02") && _EventName == TEXT("Stage_2_Event2"))		
	{

		m_bTriggerEvent2OnOff= true;	

		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		m_fHeightDistance -= m_fRealTimeDelta * 2.f;	
		 
		if (m_fHeightDistance >0.1f)	
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fRealTimeDelta * 2.f));
		else
			m_bTriggerEvent2OnOff = false;	
	}


	
	

}

HRESULT CWater::Bind_ShaderResources()
{
#pragma region 스테이지 물 
	//if (!strcmp(m_szName, "Water_Test"))
	//{
	//	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//		return E_FAIL;

	//	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
	//		return E_FAIL;

	//	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
	//		return E_FAIL;

	//	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
	//		return E_FAIL;





	//	/*파도 관련*/

	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAmplitude", &m_famplitude, sizeof(_float))))
	//		return E_FAIL;


	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_faccumulatedTime, sizeof(_float))))
	//		return E_FAIL;

	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_fWavelength", &m_fwavelength, sizeof(_float))))
	//		return E_FAIL;

	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpeed", &m_fSpeed, sizeof(_float))))
	//		return E_FAIL;


	//	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
	//		return E_FAIL;
	//}
	
#pragma region 일반 물 

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;





	/*파도 관련*/

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAmplitude", &m_famplitude, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_faccumulatedTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fWavelength", &m_fwavelength, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpeed", &m_fSpeed, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	
#pragma endregion


	return S_OK;
}

CWater* CWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 여기서 부터 다시 작성*/
	CWater* pInstance = new CWater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWater::Clone(void* pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	/*11월 15일 임시*/
	Safe_Release(m_pTexture2D);
	////////////////////////

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
