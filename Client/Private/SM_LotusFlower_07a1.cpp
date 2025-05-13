#include "pch.h"
#include "SM_LotusFlower_07a1.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CSM_LotusFlower_07a1::CSM_LotusFlower_07a1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSM_LotusFlower_07a1::CSM_LotusFlower_07a1(const  CSM_LotusFlower_07a1& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CSM_LotusFlower_07a1::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CSM_LotusFlower_07a1::Initialize(void* pArg)
{
    if (m_szName == nullptr)    
        strcpy_s(m_szName, "Prototype_GameObject_SM_LotusFlower_07a1");     

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

#pragma region 이벤트 객체 요소 설치 
    if (!strcmp(m_szName, "Water_Step_01"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_1"), this)))    
            return E_FAIL;  
    }

    else if (!strcmp(m_szName, "Water_Step_02"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_1"), this)))
            return E_FAIL;
    }


    else if (!strcmp(m_szName, "Water_Step_03"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_1"), this)))
            return E_FAIL;
    }

    else if (!strcmp(m_szName, "Water_Step_04"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_1"), this)))
            return E_FAIL;
    }
#pragma endregion 


    //m_pTransformCom->Scaling(_float3{ 0.001f,0.001f,0.001f });

    return S_OK;
}

void  CSM_LotusFlower_07a1::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CSM_LotusFlower_07a1::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);

    __super::Update(fTimeDelta);

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));     
}

void  CSM_LotusFlower_07a1::Late_Update(_float fTimeDelta)
{

    m_fTimeDelta = fTimeDelta;  

    if (m_bTriggerOnOff)    
        IsTriggerOn(TEXT("Stage_1"));   

    if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance))
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    }
}

HRESULT  CSM_LotusFlower_07a1::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }

    //m_pColliderCom->Render();   

    return S_OK;
}

HRESULT  CSM_LotusFlower_07a1::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_LotusFlower_07a1"),    
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

   /* Com_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};    


    SphereDesc.fRadius = 90.f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius-130.f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("Step");
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this); 


    return S_OK;
}

HRESULT  CSM_LotusFlower_07a1::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CSM_LotusFlower_07a1::IsTriggerOn(_wstring _EventName) 
{
    if (_EventName == TEXT("Stage_1"))
    {
        m_bTriggerOnOff = true;


        _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);   

        if (vCurPos.m128_f32[1] < -185.f)   
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta * 3.f));
        else
            m_bTriggerOnOff = false;    
    }
}

CSM_LotusFlower_07a1* CSM_LotusFlower_07a1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSM_LotusFlower_07a1* pInstance = new  CSM_LotusFlower_07a1(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SM_LotusFlower_07a1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSM_LotusFlower_07a1::Clone(void* pArg)
{
    CSM_LotusFlower_07a1* pInstance = new  CSM_LotusFlower_07a1(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : SM_LotusFlower_07a1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSM_LotusFlower_07a1::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
