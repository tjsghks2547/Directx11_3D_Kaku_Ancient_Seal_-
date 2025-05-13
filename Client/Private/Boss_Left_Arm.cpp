#include "pch.h"
#include "Boss_Left_Arm.h"
#include "Collider.h"
#include "GameInstance.h"   
#include "Client_Defines.h"
#include "Boss.h"   
#include "ClawEffectLeft.h"

CBoss_Left_Arm::CBoss_Left_Arm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)    
    :CGameObject(pDevice, pContext) 
{
}

HRESULT CBoss_Left_Arm::Initialize(void* pArg)  
{
    strcpy_s(m_szName, "Boss_Left_Arm");    

    BODY_DESC* pBody_Desc = static_cast<BODY_DESC*>(pArg);  
    
    m_pSocketMatrix = pBody_Desc->pSocketMatrix;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    //Safe_Release(m_pTransformCom); // 이거 안해주면 Transform 만들어지고 안지워져서 Leak 발생함. 


    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    return S_OK;
}

void CBoss_Left_Arm::Priority_Update(_float fTimeDelta)
{
    if (*m_iState == CBoss::STATE_ATTACK_PARTTERN_2 && m_iPreState != CBoss::STATE_ATTACK_PARTTERN_2)
        m_bEffectTimeStart = true;


    if (*m_iState != CBoss::STATE_ATTACK_PARTTERN_2)
        m_bEffectTimeStart = false;


    if (m_bEffectTimeStart)
        m_fPatternTimeEffect += fTimeDelta;


    if (m_fPatternTimeEffect > 0.1f)
    {
        CClawEffectLeft* pClawLeftEffect = CClawEffectLeft::Create(m_pDevice, m_pContext);
        if (pClawLeftEffect == nullptr)
            return;

        //_float4 pos = { vCurPosition.m128_f32[0],vCurPosition.m128_f32[1] - 10.f,vCurPosition.m128_f32[2],1.f };
        pClawLeftEffect->Set_WorldMatrix(&m_CombinedWorldMatrix);
        m_pGameInstance->Add_Effect(pClawLeftEffect);


        m_bEffectTimeStart = false;
        m_fPatternTimeEffect = 0.f;
    }
}

void CBoss_Left_Arm::Update(_float fTimeDelta)
{
    m_fAccTimeDelta += fTimeDelta;

    if (m_isDamage)
    {
        if (m_fAccTimeDelta > 5.0f)
        {
            m_fAccTimeDelta = 0.f;
            m_isDamage = false;
        }
    }


    if (*m_iAttackState && m_isDamage == false) 
        m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);    

    else
    {
        m_pGameInstance->Sub_ObjCollider(GROUP_TYPE::MONSTER, this);    
    }


    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    //for (size_t i = 0; i < 3; i++)  
    //    SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);  

    XMStoreFloat4x4(&m_CombinedWorldMatrix, 
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */ 
        SocketMatrix *  /* 로컬 스페이스 영역 */    
        XMLoadFloat4x4(m_pParentMatrix)   /* 월드 영역 */   
    );

    m_pColliderAttackCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

    m_iPreState = *m_iState;
}

void CBoss_Left_Arm::Late_Update(_float fTimeDelta)
{
    if (*m_iAttackState)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    }

}

HRESULT CBoss_Left_Arm::Render()
{
  
    //m_pColliderAttackCom->Render();

    return S_OK;
}

HRESULT CBoss_Left_Arm::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
   
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoss_Left_Arm::Ready_Components()
{

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Attack_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereAttackDesc{};


    SphereAttackDesc.fRadius = 80.f;
    SphereAttackDesc.vCenter = _float3(100.f, SphereAttackDesc.fRadius-100.f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderAttackCom), &SphereAttackDesc)))
        return E_FAIL;

    //m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);
    m_pColliderAttackCom->Set_Collider_Name("Boss_Left_Arm");


    return S_OK;
}

void CBoss_Left_Arm::OnCollisionEnter(CGameObject* _pOther)
{
     
}

void CBoss_Left_Arm::OnCollision(CGameObject* _pOther)
{
}

void CBoss_Left_Arm::OnCollisionExit(CGameObject* _pOther)
{
}

CBoss_Left_Arm* CBoss_Left_Arm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
    CBoss_Left_Arm* pInstance = new CBoss_Left_Arm(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pArg)))    
    {
        MSG_BOX("Failed to Created : Boss_Left_Arm");
        Safe_Release(pInstance);
    }

    return pInstance;

}

CGameObject* CBoss_Left_Arm::Clone(void* pArg)
{
    return nullptr;
}

void CBoss_Left_Arm::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderAttackCom);
}
