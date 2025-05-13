#include "pch.h"
#include "Box_Lift.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CBox_Lift::CBox_Lift(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CBox_Lift::CBox_Lift(const  CBox_Lift& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CBox_Lift::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CBox_Lift::Initialize(void* pArg)
{
  
    if (m_szName == nullptr)    
        strcpy_s(m_szName, "Prototype_GameObject_Box_Lift");    

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



    if (!strcmp(m_szName, "Box_Lift_1"))        
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Object"), this)))        
            return E_FAIL;      

        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Event2"), this))) 
            return E_FAIL;  

        m_pTransformCom->Set_Speed(10.f);
    }

    if (!strcmp(m_szName, "Box_Lift_2"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Door"), this)))       
            return E_FAIL;      

        m_pTransformCom->Set_Speed(10.f);       
    }

    return S_OK;
}

void  CBox_Lift::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CBox_Lift::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;  

    if (m_bTriggerOnOff)
        IsTriggerOn(TEXT("Stage_2_Object"));


    if (m_bTriggerEvent2OnOff)
        IsTriggerOn(TEXT("Stage_2_Event2"));    
}

void  CBox_Lift::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CBox_Lift::Render()
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

    return S_OK;
}

HRESULT  CBox_Lift::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Box_Lift"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT  CBox_Lift::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CBox_Lift::IsTriggerOn(_wstring _EventName)    
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    
    //  Stage 2  Object  Event 일때 
    if (!strcmp(m_szName, "Box_Lift_1") && _EventName == TEXT("Stage_2_Object"))    
    {
        m_bTriggerOnOff = true; 

        m_fMoveDistance -= m_fTimeDelta * 3.f;    

        if (m_fMoveDistance > 0.1f)
            m_pTransformCom->Go_Straight(m_fTimeDelta * 3.f );

        else
            m_bTriggerOnOff = false;

    }

    if (!strcmp(m_szName, "Box_Lift_1") && _EventName == TEXT("Stage_2_Event2"))    
    {
        m_bTriggerEvent2OnOff = true;   

        m_fMoveHeightDistance -= m_fTimeDelta * 2.f;     // 이것도 수정 

        if (m_fMoveHeightDistance > 0.1f)
        {
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta*2.f));
        }
        else
            m_bTriggerEvent2OnOff = false;  

    }
   

}

CBox_Lift* CBox_Lift::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBox_Lift* pInstance = new  CBox_Lift(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBox_Lift");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBox_Lift::Clone(void* pArg)
{
    CBox_Lift* pInstance = new  CBox_Lift(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBox_Lift");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CBox_Lift::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
