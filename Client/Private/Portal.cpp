#include "pch.h"
#include "Portal.h"
#include "GameInstance.h"	
#include "Monster.h"	
#include "Navigation.h"
#include "FadeInOut.h"

CPortal::CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPortal::CPortal(const CPortal& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Speed(10.f);
	m_pTransformCom->Set_RotationSpeed(5.f);

	m_pParentTrasnformCom = m_pParent->Get_Transfrom();		

	m_pTransformCom->Turn_Degree(_fvector{ 1.f,0.f,0.f,0.f }, XMConvertToRadians(90.f));
	m_pTransformCom->Scaling(_float3(1.f, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSetZ(m_pTransformCom->Get_State(CTransform::STATE_LOOK), -1.f));
	

	m_fCurrentHpState = 1.f;

	return S_OK;
}

void CPortal::Priority_Update(_float fTimeDelta)
{
	

	//_matrix matWorld, matView, matBill;
	//matWorld = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());

	//matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));

	//matBill = XMMatrixIdentity();

	//matBill.r[0].m128_f32[0] = matView.r[0].m128_f32[0];
	//matBill.r[0].m128_f32[2] = matView.r[0].m128_f32[2];
	//matBill.r[2].m128_f32[0] = matView.r[2].m128_f32[0];
	//matBill.r[2].m128_f32[2] = matView.r[2].m128_f32[2];

	//matBill = XMMatrixInverse(0, matBill);

	////XMStoreFloat4x4(&m_BillMatrix, (matBill* matWorld));	
	//XMStoreFloat4x4(&m_BillMatrix, matBill);
	//m_pTransformCom->Set_WorldMatrix(m_BillMatrix);
	m_pTransformCom->Scaling(_float3(50.f, 50.f, 50.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pParentTrasnformCom->Get_State(CTransform::STATE_POSITION), m_pParentTrasnformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1]+2.f));


	m_fRadian += fTimeDelta; 
}

void CPortal::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));	
}

void CPortal::Late_Update(_float fTimeDelta)
{
	if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance))
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
	}
}

HRESULT CPortal::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_RotationAngle", &m_fRadian, sizeof(_float))))
		return E_FAIL; 

	m_pShaderCom->Begin(5);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	//m_pColliderCom->Render();

	return S_OK;

}

HRESULT CPortal::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Portal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};	


	SphereDesc.fRadius = 1.f;	
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius-1.f, 0.f);	


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;


	m_pColliderCom->Set_Collider_Name("CPortal");
	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);


	return S_OK;
}


void CPortal::OnCollisionEnter(CGameObject* _pOther)
{
}

void CPortal::OnCollision(CGameObject* _pOther)
{
 	int a = 4;

	if (m_pGameInstance->isKeyEnter(DIK_F))
	{
		CNavigation* pNavigation = dynamic_cast<CNavigation*>(_pOther->Find_Component(TEXT("Com_Navigation")));

		pNavigation->Set_CurCellIndex(24);
		CTransform* pPlayerTransfrom = _pOther->Get_Transfrom();
		pPlayerTransfrom->Set_State(CTransform::STATE_POSITION, _fvector{ 55.f,85.f,571.f,1.f });

		isDead = true; 


		CUIObject::UIOBJECT_DESC UI_Desc{};	
		UI_Desc.fSizeX = 1600.f;	
		UI_Desc.fSizeY = 900.f;	
		UI_Desc.fX = 800.f;
		UI_Desc.fY = 450.f;
		UI_Desc.fNear = 0.f;
		UI_Desc.fFar = 1.f;

		UI_Desc.fSpeedPerSec = 5.f;
		UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);
		UI_Desc.szName = "Fade_InOut_UI";


		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FadeInOut"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &UI_Desc)))
		{
			MSG_BOX("Failed to FadeInOut");
		}
	}
}

void CPortal::OnCollisionExit(CGameObject* _pOther)
{
}

CPortal* CPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPortal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPortal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
