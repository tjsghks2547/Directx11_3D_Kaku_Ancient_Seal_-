#include "pch.h"
#include "M_Guardian_Model_C_Arm.h"
#include "Collider.h"
#include "GameInstance.h"   
#include "Client_Defines.h"

CM_Guardian_Model_C_Arm::CM_Guardian_Model_C_Arm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

HRESULT CM_Guardian_Model_C_Arm::Initialize(void* pArg)
{
    strcpy_s(m_szName, "M_Guardian_Model_C_ARM");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;  
    
    Safe_Release(m_pTransformCom); // 이거 안해주면 Transform 만들어지고 안지워져서 Leak 발생함. 
        

    if (FAILED(Ready_Components())) 
        return E_FAIL;  

    if (FAILED(Bind_ShaderResources())) 
        return E_FAIL;  

   
    return S_OK;
}   

void CM_Guardian_Model_C_Arm::Priority_Update(_float fTimeDelta)
{
    
}

void CM_Guardian_Model_C_Arm::Update(_float fTimeDelta)
{
    m_fAccTimeDelta += fTimeDelta;   

    if (m_isDamage)
    {
        if(m_fAccTimeDelta > 5.0f)
        {
            m_fAccTimeDelta = 0.f;
            m_isDamage = false;
        }
    }


    if (*m_iState == STATE::STATE_ATTACK && m_isDamage == false)
        m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);

    else
    {
        m_pGameInstance->Sub_ObjCollider(GROUP_TYPE::MONSTER, this);
    }

    m_pColliderAttackCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));       
}

void CM_Guardian_Model_C_Arm::Late_Update(_float fTimeDelta)
{

}

HRESULT CM_Guardian_Model_C_Arm::Render()
{
    if(*m_iState == STATE::STATE_ATTACK)
        m_pColliderAttackCom->Render();     

    return S_OK;
}

HRESULT CM_Guardian_Model_C_Arm::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))    
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;    
}

HRESULT CM_Guardian_Model_C_Arm::Ready_Components()
{

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),   
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))    
        return E_FAIL;  

    /* Com_Attack_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereAttackDesc{};      


    SphereAttackDesc.fRadius = 10.f;
    SphereAttackDesc.vCenter = _float3(0.f, SphereAttackDesc.fRadius, 10.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),  
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderAttackCom), &SphereAttackDesc)))    
        return E_FAIL;  

    m_pColliderAttackCom->Set_Collider_Name("M_Guardian_Model_C_Attack");   
    

    return S_OK;
}

void CM_Guardian_Model_C_Arm::OnCollisionEnter(CGameObject* _pOther)
{
    m_isDamage = true;  
}

void CM_Guardian_Model_C_Arm::OnCollision(CGameObject* _pOther)
{
}

void CM_Guardian_Model_C_Arm::OnCollisionExit(CGameObject* _pOther)
{
}

CM_Guardian_Model_C_Arm* CM_Guardian_Model_C_Arm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CM_Guardian_Model_C_Arm* pInstance = new CM_Guardian_Model_C_Arm(pDevice, pContext);

    if (FAILED(pInstance->Initialize(nullptr)))    
    {
        MSG_BOX("Failed to Created : CM_Guardian_Model_C_Arm"); 
        Safe_Release(pInstance);    
    }

    return pInstance;   

}

CGameObject* CM_Guardian_Model_C_Arm::Clone(void* pArg)
{
    return nullptr;
}

void CM_Guardian_Model_C_Arm::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderAttackCom);     
}
