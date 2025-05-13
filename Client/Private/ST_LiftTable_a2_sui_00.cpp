#include "pch.h"
#include "ST_LiftTable_a2_sui_00.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Animation.h"

CST_LiftTable_a2_sui_00::CST_LiftTable_a2_sui_00(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CST_LiftTable_a2_sui_00::CST_LiftTable_a2_sui_00(const CST_LiftTable_a2_sui_00& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CST_LiftTable_a2_sui_00::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CST_LiftTable_a2_sui_00::Initialize(void* pArg)
{
    strcpy_s(m_szName, "LIFT_TABLE");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    //m_pModelCom->SetUp_Animation(2, false);
    
    //m_pTransformCom->Scaling(_float3{ 0.001f,0.001f,0.001f });    

    return S_OK;
}

void  CST_LiftTable_a2_sui_00::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_pModelCom->SetUp_Animation(2, false);  

}

void  CST_LiftTable_a2_sui_00::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);


    __super::Update(fTimeDelta);    

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

    //if (true == m_pModelCom->Play_Animation(fTimeDelta))
    //    int a = 10;

    if (m_bisCollision && !m_pModelCom->Get_VecAnimation().at(2)->isAniMationFinish())
    {
        m_pModelCom->Play_Animation(fTimeDelta * 0.3f);
    }

   
}

void  CST_LiftTable_a2_sui_00::Late_Update(_float fTimeDelta)
{
    if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance))
         m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);       
}

HRESULT  CST_LiftTable_a2_sui_00::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if(!m_bisCollision)
    {
        _uint  iNumMeshes = m_pModelNoAnimCom->Get_NumMeshes();

        for (size_t i = 0; i < iNumMeshes; i++)
        {
            if (FAILED(m_pModelNoAnimCom->Bind_Material(m_pShaderNoAnimCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))  
                return E_FAIL;

            if (FAILED(m_pModelNoAnimCom->Bind_Material(m_pShaderNoAnimCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
                return E_FAIL;


            if (!m_bisCollisionEnd)
            {
                m_pShaderNoAnimCom->Begin(0);

                m_pModelNoAnimCom->Render(i);
            }
        }
    }
    
   
    else
    {
        _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

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
    }


#ifdef _DEBUG
    //m_pColliderCom->Render();       
#endif // _DEBUG

    return S_OK;
}

HRESULT  CST_LiftTable_a2_sui_00::Ready_Components()        
{
    /* Animaiton Model */

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ST_LiftTable_a2_sui_00"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /////////////////////////////////////////////////////////////////////
    /* NoAnimation Model */

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),   
        TEXT("Com_Shader2"), reinterpret_cast<CComponent**>(&m_pShaderNoAnimCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ST_LiftTable_a2_sui_01"),
        TEXT("Com_Model2"), reinterpret_cast<CComponent**>(&m_pModelNoAnimCom))))
        return E_FAIL;





    /* Com_Collider */  
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};        
    
    
    SphereDesc.fRadius = 100.f;  
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);     
    
    
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),      
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))       
        return E_FAIL;  
    
    
    m_pColliderCom->Set_Collider_Name("LIFT_TABLE");    
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this); 



    return S_OK;
}

HRESULT  CST_LiftTable_a2_sui_00::Bind_ShaderResources()
{
    
   
    
    if (m_bisCollision) 
    {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 35.f));

        if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
            return E_FAIL;
    }

    else
    {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 25.f));

        if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderNoAnimCom, "g_WorldMatrix")))
            return E_FAIL;
        if (FAILED(m_pShaderNoAnimCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
            return E_FAIL;
        if (FAILED(m_pShaderNoAnimCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
            return E_FAIL;
    }


   /* if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;*/

    ////////////////////////////////////////////////////


    //if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))    
    //    return E_FAIL;
    //if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
    //    return E_FAIL;
    //if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
    //    return E_FAIL;
    //
    //const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    //if (nullptr == pLightDesc)
    //    return E_FAIL;
    //
    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))   
    //    return E_FAIL;
    //
    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)))) 
    //    return E_FAIL;
    //
    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)))) 
    //    return E_FAIL;  
    //
    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))   
    //    return E_FAIL;
    //
    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))    
    //    return E_FAIL;  
    //
    //return S_OK;


    return S_OK;
}

void CST_LiftTable_a2_sui_00::OnCollisionEnter(CGameObject* _pOther)
{
    m_bisCollision = true;  
}

void CST_LiftTable_a2_sui_00::OnCollision(CGameObject* _pOther)
{
    m_bisCollision = true;  
    //CTransform* pPlayerTransform = dynamic_cast<CTransform*>(_pOther->Find_Component(TEXT("Com_Transform")));
    //
    //pPlayerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 35.f));


    //여기서 플레이어의  state 를 바꾸자. 
}

void CST_LiftTable_a2_sui_00::OnCollisionExit(CGameObject* _pOther)
{
    m_bisCollision = false; 
    m_bisCollisionEnd = true;   
}

CST_LiftTable_a2_sui_00* CST_LiftTable_a2_sui_00::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CST_LiftTable_a2_sui_00* pInstance = new  CST_LiftTable_a2_sui_00(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SM_LotusFlower_07a1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CST_LiftTable_a2_sui_00::Clone(void* pArg)
{
    CST_LiftTable_a2_sui_00* pInstance = new  CST_LiftTable_a2_sui_00(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : SM_LotusFlower_07a1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CST_LiftTable_a2_sui_00::Free()
{
    __super::Free();
    
    
    Safe_Release(m_pColliderCom);

    Safe_Release(m_pShaderNoAnimCom);   
    Safe_Release(m_pModelNoAnimCom);    

    Safe_Release(m_pShaderCom); 
    Safe_Release(m_pModelCom);

}
