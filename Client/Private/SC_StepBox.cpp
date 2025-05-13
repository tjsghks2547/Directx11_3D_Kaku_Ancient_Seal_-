#include "pch.h"
#include "SC_StepBox.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CSC_StepBox::CSC_StepBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSC_StepBox::CSC_StepBox(const CSC_StepBox& Prototype)  
    :CGameObject(Prototype) 
{

}

HRESULT CSC_StepBox::Initialize_Prototype() 
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSC_StepBox::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SC_StepBox");

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

    m_pGameInstance->Add_EventObject(TEXT("Last_Event"), this); 
   
    return S_OK;
}

void  CSC_StepBox::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CSC_StepBox::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    if (m_bTriggerOnOff)
    {
        IsTriggerOn(TEXT("Last_Event"));    
    }

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
}

void  CSC_StepBox::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CSC_StepBox::Render()
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

HRESULT  CSC_StepBox::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),   
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))    
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_StepBox"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 150.f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("CStepBox");    
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);     

    return S_OK;
}

HRESULT CSC_StepBox::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))    
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CSC_StepBox::IsTriggerOn(_wstring _EventName)
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    if (_EventName == TEXT("Last_Event"))   
    {
        m_bTriggerOnOff = true;     


        _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);   
        m_fMoveHeightDistance -= m_fTimeDelta * 10.f; 

        if (m_fMoveHeightDistance > 0.f)
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta * 10.f));
        else
            m_bTriggerOnOff = false;
    }

}

void CSC_StepBox::OnCollisionEnter(CGameObject* _pOther)
{
    m_pGameInstance->Event_Activate(TEXT("Last_Event"));
}

void CSC_StepBox::OnCollision(CGameObject* _pOther)
{
}

void CSC_StepBox::OnCollisionExit(CGameObject* _pOther)
{
}

CSC_StepBox* CSC_StepBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSC_StepBox* pInstance = new  CSC_StepBox(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CSC_StepBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSC_StepBox::Clone(void* pArg)
{
    CSC_StepBox* pInstance = new  CSC_StepBox(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CSC_StepBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSC_StepBox::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
