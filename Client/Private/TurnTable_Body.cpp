#include "pch.h"
#include "TurnTable_Body.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CTurnTable_Body::CTurnTable_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CTurnTable_Body::CTurnTable_Body(const  CTurnTable_Body& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CTurnTable_Body::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CTurnTable_Body::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_TurnTable_Body");

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


    if (!strcmp(m_szName, "Body_4"))    
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_3"), this)))    
            return E_FAIL;  


        m_pTransformCom->Set_Speed(10.f);   
    }



    return S_OK;
}

void  CTurnTable_Body::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CTurnTable_Body::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);


    m_fTimeDelta = fTimeDelta; 

    if(m_bTriggerOnOff)
        IsTriggerOn(TEXT("Stage_3"));   
    
}

void  CTurnTable_Body::Late_Update(_float fTimeDelta)
{
    if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance))
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this); 
    }
}

HRESULT  CTurnTable_Body::Render()
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

HRESULT  CTurnTable_Body::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TurnTable_Body"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT  CTurnTable_Body::Bind_ShaderResources()    
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CTurnTable_Body::IsTriggerOn(_wstring _EventName)  
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


    if (!strcmp(m_szName, "Body_4") && _EventName == TEXT("Stage_3"))
    {
        m_bTriggerOnOff = true; 

        m_fMoveHeightDistance -= m_fTimeDelta * 5.f;     // 이것도 수정      

        if (m_fMoveHeightDistance > 0.1f)   
        {
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta * 5.f));    
        }
        else
            m_bTriggerOnOff = false;    


    }
}

CTurnTable_Body* CTurnTable_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTurnTable_Body* pInstance = new  CTurnTable_Body(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : TurnTable_Body");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTurnTable_Body::Clone(void* pArg)
{
    CTurnTable_Body* pInstance = new  CTurnTable_Body(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : TurnTable_Body");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CTurnTable_Body::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
