#include "pch.h"
#include "StrongMan.h"	
#include "GameInstance.h"
#include "Animation.h"
#include "NpcInterActionUi.h"		
#include "NpcQuestionUi.h"
#include "NpcDialog.h"

CStrongMan::CStrongMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
	
}

CStrongMan::CStrongMan(const CStrongMan& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CStrongMan::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStrongMan::Initialize(void* pArg)
{
	if (m_szName == nullptr)
		strcpy_s(m_szName, "Prototype_GameObject_StrongMan");

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

	m_pModelCom->SetUp_Animation(2, true);


	/*if (!strcmp(m_szName, "Lever_Button_1"))
	{
		if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Door"), this)))
			return E_FAIL;

		m_pTransformCom->Set_Speed(10.f);
	}*/

	//if (!strcmp(m_szName, "Lever_Button_2"))
	//{

	//	m_pTransformCom->Set_RotationSpeed(10.f);
	//	m_pTransformCom->Turn_Degree(_vector{ 1.f,0.f,0.f,0.f }, XMConvertToRadians(-90.f));
	//	m_pTransformCom->Turn_Degree(_vector{ 0.f,0.f,1.f,0.f }, XMConvertToRadians(90.f));
	//}

	return S_OK;
}

void CStrongMan::Priority_Update(_float fTimeDelta)
{
	//m_pModelCom->SetUp_Animation(0, true);
	if(m_bQuestionUiOnOff)
		m_pNpcQuestionUi->Priority_Update(fTimeDelta);


	if (m_bDialogUiOnOff)
		m_pNpcDialog->Priority_Update(fTimeDelta);

	int a = 10;
}

void CStrongMan::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
}

void CStrongMan::Late_Update(_float fTimeDelta)
{

	m_fTimeDelta = fTimeDelta;
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	if (m_isCollision) // 접근시에만 Ui 나타나도록 설정 
		m_pInterActionUi->Late_Update(fTimeDelta);

	if(m_bQuestionUiOnOff)
		m_pNpcQuestionUi->Late_Update(fTimeDelta);

	if (m_bDialogUiOnOff)
		m_pNpcDialog->Late_Update(fTimeDelta);	

}

HRESULT CStrongMan::Render()
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

HRESULT CStrongMan::Ready_Components()
{


	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StrongMan"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	///* Com_InterAction_UI */
	m_pInterActionUi = CNpcInterActionUi::Create(m_pDevice, m_pContext);	
	m_pInterActionUi->Initialize(nullptr);			


	///* Com_NpcDialog_UI */
	m_pNpcDialog = CNpcDialog::Create(m_pDevice, m_pContext);	
	m_pNpcDialog->Initialize(nullptr);	


	/* Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


	SphereDesc.fRadius = 30.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;


	m_pColliderCom->Set_Collider_Name("CStrongMan");	
	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);	


	/* Com_QuestionUi */

	m_pNpcQuestionUi =CNpcQuestionUi::Create(m_pDevice, m_pContext);	
	m_pNpcQuestionUi->SetNpcPtr(this);			
	m_pNpcQuestionUi->Initialize(nullptr);		

	return S_OK;
}

HRESULT CStrongMan::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CStrongMan::OnCollisionEnter(CGameObject* _pOther)
{
}

void CStrongMan::OnCollision(CGameObject* _pOther)
{

	m_isCollision = true;	
	if (!strcmp(_pOther->Get_Name(), "PLAYER"))
	{
		if (m_pGameInstance->isKeyEnter(DIK_F))	
		{
			//m_pGameInstance->Event_Activate(TEXT("Stage_2_Object"));	

			m_pModelCom->SetUp_Animation(1, false);	
			m_pGameInstance->Event_Activate(TEXT("Stone_Ball_Stage1"));
			m_bQuestionUiOnOff = false; 
			m_bDialogUiOnOff = true;
			
		}
	}


	if (m_pModelCom->Get_Current_Animation_Index() == 1)
	{
		if (m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish())
			m_pModelCom->SetUp_Animation(0, true);
	}

}

void CStrongMan::OnCollisionExit(CGameObject* _pOther)
{
	m_isCollision = false; 
	m_bDialogUiOnOff = false; 
}

CStrongMan* CStrongMan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStrongMan* pInstance = new CStrongMan(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStrongMan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStrongMan::Clone(void* pArg)
{
	CStrongMan* pInstance = new CStrongMan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStrongMan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStrongMan::Free()
{
	__super::Free();

	Safe_Release(m_pNpcDialog);	
	Safe_Release(m_pNpcQuestionUi);	
	Safe_Release(m_pInterActionUi);	
	Safe_Release(m_pColliderCom);	
	Safe_Release(m_pShaderCom);		
	Safe_Release(m_pModelCom);	

}
