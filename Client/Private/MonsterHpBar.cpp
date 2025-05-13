#include "pch.h"
#include "MonsterHpBar.h"
#include "GameInstance.h"	
#include "Monster.h"	

CMonsterHpBar::CMonsterHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)	
{
}

CMonsterHpBar::CMonsterHpBar(const CMonsterHpBar& Prototype)
	:CGameObject(Prototype)		
{
}

HRESULT CMonsterHpBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;	


	return S_OK;	
}

HRESULT CMonsterHpBar::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Speed(10.f);

	m_pMonsterTrasnformCom = m_pMonster->Get_Transfrom();

	m_pTransformCom->Scaling(_float3(1.f, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSetZ(m_pTransformCom->Get_State(CTransform::STATE_LOOK), -1.f));


	m_iHp = dynamic_cast<CMonster*>(m_pMonster)->Get_MonsterHp();	
	m_fCurrentHpState = 1.f;

	return S_OK;	
}

void CMonsterHpBar::Priority_Update(_float fTimeDelta)
{
	_matrix matWorld, matView, matBill; 
	matWorld = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());	
	
	matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));

	matBill = XMMatrixIdentity();

	matBill.r[0].m128_f32[0] = matView.r[0].m128_f32[0];
	matBill.r[0].m128_f32[2] = matView.r[0].m128_f32[2];
	matBill.r[2].m128_f32[0] = matView.r[2].m128_f32[0];
	matBill.r[2].m128_f32[2] = matView.r[2].m128_f32[2];	

	matBill = XMMatrixInverse(0, matBill);			

	//XMStoreFloat4x4(&m_BillMatrix, (matBill* matWorld));	
	XMStoreFloat4x4(&m_BillMatrix, matBill);
	m_pTransformCom->Set_WorldMatrix(m_BillMatrix);	
	m_pTransformCom->Scaling(_float3(10.f, 1.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pMonsterTrasnformCom->Get_State(CTransform::STATE_POSITION), m_pMonsterTrasnformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] + 30.f));
}

void CMonsterHpBar::Update(_float fTimeDelta)
{
	_float CurrentHp = *m_iHp / 10.f;	



	if (CurrentHp < m_fCurrentHpState)	
		m_fCurrentHpState -= fTimeDelta * 0.2f;	

}

void CMonsterHpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);	
}

HRESULT CMonsterHpBar::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))	
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))	
		return E_FAIL;	


	if (FAILED(m_pShaderCom->Bind_RawValue("g_HpState", &m_fCurrentHpState, sizeof(_float))))
		return E_FAIL;	

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
			
}

HRESULT CMonsterHpBar::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hp_Bar_UI"),
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

	return S_OK;	
}


CMonsterHpBar* CMonsterHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonsterHpBar* pInstance = new CMonsterHpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))	
	{
		MSG_BOX("Failed to Created : CMonsterHpBar");	
		Safe_Release(pInstance);	
	}

	return pInstance;	
}

CGameObject* CMonsterHpBar::Clone(void* pArg)
{
	CMonsterHpBar* pInstance = new CMonsterHpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))	
	{
		MSG_BOX("Failed to Cloned : CMonsterHpBar");		
		Safe_Release(pInstance);	
	}

	return pInstance;	
}

void CMonsterHpBar::Free()
{
	__super::Free();
	
	Safe_Release(m_pShaderCom);	
	Safe_Release(m_pVIBufferCom);	
	Safe_Release(m_pTextureCom);	
}
