#include "pch.h"
#include "SC_TotemB.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "TotemActionUi.h"
#include "Portal.h"


CSC_TotemB::CSC_TotemB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSC_TotemB::CSC_TotemB(const CSC_TotemB& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CSC_TotemB::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSC_TotemB::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SC_TotemB");

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

    m_pTransformCom->Set_Speed(10.f);
    m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));

    m_pGameInstance->Add_EventObject(TEXT("Portal_Event"), this);   

    return S_OK;
}

void CSC_TotemB::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_bisStart)
    {
        m_fEventTime += fTimeDelta; 

        if (m_fEventTime > 2.f)
        {
            m_pPortal->Priority_Update(fTimeDelta);
        }
    }
}

void CSC_TotemB::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    if (m_bTriggerOnOff)
    {
        IsTriggerOn(TEXT("Portal_Event"));
    }

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
    
    if (m_bisStart)
        m_pPortal->Update(fTimeDelta);  

    if (m_pPortal->Get_StateDead())
        m_pGameInstance->Add_DeadObject(TEXT("Layer_Control"), this);
}

void  CSC_TotemB::Late_Update(_float fTimeDelta)
{
    if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance))
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    }

    if(m_bisStart)
     m_pPortal->Late_Update(fTimeDelta);


    if (m_isCollision && !m_bisStart) // 접근시에만 Ui 나타나도록 설정 
        m_pTotemActionUi->Late_Update(fTimeDelta);
}

HRESULT  CSC_TotemB::Render()
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

        m_pShaderCom->Begin(1);

        m_pModelCom->Render(i);
    }


    //m_pColliderCom->Render();

   


 
    return S_OK;
}

HRESULT CSC_TotemB::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_TotemB"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Noise Texture */
    /* Com Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


    ///* Com_InterAction_UI */
    m_pTotemActionUi = CTotemActionUi::Create(m_pDevice, m_pContext);    
    m_pTotemActionUi->Initialize(nullptr);          

    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};    


    SphereDesc.fRadius = 300.f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("CTotemB");
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);


    /* Com_HpBar */

    m_pPortal = CPortal::Create(m_pDevice, m_pContext);     
    m_pPortal->SetParentPtr(this);  
    m_pPortal->Initialize(nullptr);     

    return S_OK;
}

HRESULT CSC_TotemB::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveAmount, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTimeDeltaDissove, sizeof(_float))))
        return E_FAIL;


    return S_OK;
}

void CSC_TotemB::IsTriggerOn(_wstring _EventName)
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    if (_EventName == TEXT("Portal_Event"))
    {
        m_bTriggerOnOff = true;

        m_fAccTimeDeltaDissove += m_fTimeDelta;   
        
        if (m_fDissolveAmount > 1.f)
        {
            m_fDissolveAmount = 2.f;
            m_bTriggerOnOff = false;
        }
        else
        {
            m_fDissolveAmount += m_fTimeDelta * 0.5f;   
        }   
    }

}

void CSC_TotemB::OnCollisionEnter(CGameObject* _pOther)
{
   
}

void CSC_TotemB::OnCollision(CGameObject* _pOther)
{
    m_isCollision = true;   

    if(m_pGameInstance->isKeyEnter(DIK_F))
    {
        m_pGameInstance->Event_Activate(TEXT("Portal_Event"));

        if(m_bOnce)
        {
            m_isCollision = false;
            m_bisStart = true;


            m_bOnce = false; 
        }
        
    }
}

void CSC_TotemB::OnCollisionExit(CGameObject* _pOther)
{
    //m_isCollision = false; 
    //m_bisStart = true;  

}

CSC_TotemB* CSC_TotemB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSC_TotemB* pInstance = new  CSC_TotemB(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CSC_TotemB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSC_TotemB::Clone(void* pArg)  
{
    CSC_TotemB* pInstance = new  CSC_TotemB(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CSC_TotemB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSC_TotemB::Free()
{
    __super::Free();


    Safe_Release(m_pPortal);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTotemActionUi);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
