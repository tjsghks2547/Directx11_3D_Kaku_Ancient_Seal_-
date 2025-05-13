#include "pch.h"
#include "SC_Object_StoneBall.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CSC_Object_StoneBall::CSC_Object_StoneBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSC_Object_StoneBall::CSC_Object_StoneBall(const  CSC_Object_StoneBall& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CSC_Object_StoneBall::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CSC_Object_StoneBall::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SC_Object_StoneBall");


  /*  _char* NoName = " ";

    if (pArg != nullptr)
    {
        CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
        {
            if (strcmp(pObjectDesc->szName, NoName))
                strcpy_s(m_szName, pObjectDesc->szName);
        }

    }*/

    if (pArg != nullptr)
    {
        STONE_BALL_DESC* pStoneBall_Desc = static_cast<STONE_BALL_DESC*>(pArg);

        if (pStoneBall_Desc != nullptr)
        {
            m_fstartTime = pStoneBall_Desc->startTime;  
            m_fLifeTime = pStoneBall_Desc->LifeTime;    
            m_wEventName = pStoneBall_Desc->EventName;  
        }
    }


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pGameInstance->Add_EventObject(m_wEventName, this);


    m_pTransformCom->Set_Speed(80.f);
    m_pTransformCom->Set_RotationSpeed(3.f);

   

   XMStoreFloat4(&m_fLookDir,m_pTransformCom->Get_State(CTransform::STATE_LOOK));    


    return S_OK;
}

void  CSC_Object_StoneBall::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CSC_Object_StoneBall::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

   
    

    m_fTimeDelta = fTimeDelta;

 

    if (m_bTriggerOnOff)
    {
        m_fAccTimeDelta += fTimeDelta;
        IsTriggerOn(TEXT("Stone_Ball_Stage1"));
    }

    _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition) + 25.f));

}

void  CSC_Object_StoneBall::Late_Update(_float fTimeDelta)
{
    if (m_fstartTime < m_fAccTimeDelta)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);   
    }

    if(m_fLifeTime < m_fAccTimeDelta)  
        m_pGameInstance->Add_DeadObject(TEXT("Layer_Control"), this);       
}

HRESULT  CSC_Object_StoneBall::Render()
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

HRESULT CSC_Object_StoneBall::Render_Shadow()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))    
        return E_FAIL;
    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))    
        return E_FAIL;  

    _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();   

    for (size_t i = 0; i < iNumMeshes; i++) 
    {
        //if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))  
        //    return E_FAIL;  

        //if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))   
        //    return E_FAIL;  

        m_pShaderCom->Begin(2); 

        m_pModelCom->Render(i); 
    }   

    return S_OK;    
}

HRESULT  CSC_Object_StoneBall::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Object_StoneBall"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Navigation */
    CNavigation::NAVIGATION_DESC   Desc{};  
    Desc.iCurrentCellIndex = 245;   

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
        return E_FAIL;


    return S_OK;
}

HRESULT  CSC_Object_StoneBall::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CSC_Object_StoneBall::IsTriggerOn(_wstring _EventName)
{

    m_bTriggerOnOff = true;
    if (m_fstartTime < m_fAccTimeDelta)
    {
        _vector vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

        m_pTransformCom->Turn(_fvector{ 1.f,0.f,0.f,0.f }, m_fTimeDelta);
        _vector vDir = _fvector{ 0.f, 0.f, 1.f, 0.f };

        m_pTransformCom->Go_Dir(vDir, m_pNavigationCom, m_fTimeDelta);
    }

   
}

CSC_Object_StoneBall* CSC_Object_StoneBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSC_Object_StoneBall* pInstance = new  CSC_Object_StoneBall(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SC_Object_StoneBall");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSC_Object_StoneBall::Clone(void* pArg)
{
    CSC_Object_StoneBall* pInstance = new  CSC_Object_StoneBall(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : SC_Object_StoneBall");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSC_Object_StoneBall::Free()  
{
    __super::Free();

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pShaderCom);     
    Safe_Release(m_pModelCom);      
}
