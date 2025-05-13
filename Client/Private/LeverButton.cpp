#include "pch.h"
#include "LeverButton.h"
#include "GameInstance.h"
#include "Animation.h"

CLeverButton::CLeverButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
	//XMMatrixDecompose();

}

CLeverButton::CLeverButton(const CLeverButton& Prototype)
	: CGameObject( Prototype)
{
}

HRESULT CLeverButton::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeverButton::Initialize(void* pArg)
{
	if (m_szName == nullptr)	
		strcpy_s(m_szName, "Prototype_GameObject_TurnTable_Head");	

	_char* NoName = " ";	

	if (pArg != nullptr)	
	{
		CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);	
		{
			if (strcmp(pObjectDesc->szName, NoName))	
				strcpy_s(m_szName, pObjectDesc->szName);	
		}

	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(0, false);


	/*if (!strcmp(m_szName, "Lever_Button_1"))
	{
		if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Door"), this)))
			return E_FAIL;

		m_pTransformCom->Set_Speed(10.f);
	}*/

	if (!strcmp(m_szName, "Lever_Button_2"))	
	{
	
		m_pTransformCom->Set_RotationSpeed(10.f);
		m_pTransformCom->Turn_Degree(_vector{ 1.f,0.f,0.f,0.f }, XMConvertToRadians(-90.f));
		m_pTransformCom->Turn_Degree(_vector{ 0.f,0.f,1.f,0.f }, XMConvertToRadians(90.f));
	}

	return S_OK;
}

void CLeverButton::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CLeverButton::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta * m_fAnimationSpeed))	
		int a = 10;
	
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
}

void CLeverButton::Late_Update(_float fTimeDelta)
{

	m_fTimeDelta = fTimeDelta;
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	
}

HRESULT CLeverButton::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{


		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))	
			return E_FAIL;	

		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))	
			return E_FAIL;

		m_pShaderCom->Begin(0);	
		m_pModelCom->Render(i);	
	}

#ifdef _DEBUG	

		//m_pColliderCom->Render();	
#endif

	return S_OK;
}

HRESULT CLeverButton::Ready_Components()
{


	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeverButton"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	
	/* Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


	SphereDesc.fRadius = 100.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;


	m_pColliderCom->Set_Collider_Name("Lever_Button");	
	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);
	

	return S_OK;
}

HRESULT CLeverButton::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;	
}

void CLeverButton::OnCollisionEnter(CGameObject* _pOther)
{
}

void CLeverButton::OnCollision(CGameObject* _pOther)
{
	if (!strcmp(m_szName, "Lever_Button_1"))
	{
		if (m_pGameInstance->isKeyEnter(DIK_G))
		{
			m_pGameInstance->Event_Activate(TEXT("Stage_2_Object"));

			m_fAnimationSpeed = 1.f;

		}
	}


	if (!strcmp(m_szName, "Lever_Button_2"))
	{
		if (m_pGameInstance->isKeyEnter(DIK_G))	
		{
			m_pGameInstance->Event_Activate(TEXT("Stage_2_Door"));	

			m_fAnimationSpeed = 1.f;	

		}
	}
}

void CLeverButton::OnCollisionExit(CGameObject* _pOther)
{
}

CLeverButton* CLeverButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeverButton* pInstance = new CLeverButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CLeverButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeverButton::Clone(void* pArg)
{
	CLeverButton* pInstance = new CLeverButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CLeverButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeverButton::Free()
{
	__super::Free();


	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}
