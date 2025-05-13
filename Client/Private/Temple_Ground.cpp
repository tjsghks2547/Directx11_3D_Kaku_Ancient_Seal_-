#include "pch.h"
#include "Temple_Ground.h"  
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
  


CTemple_Ground::CTemple_Ground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CTemple_Ground::CTemple_Ground(const CTemple_Ground& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CTemple_Ground::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTemple_Ground::Initialize(void* pArg)
{
    if (m_szName == nullptr)
     strcpy_s(m_szName, "Prototype_GameObject_Temple_Ground");

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


    if (!strcmp(m_szName, "Temple_Ground_01"))
    {

        m_pTransformCom->Turn_Degree(_fvector{ 1.f,0.f,0.f,0.f }, XMConvertToRadians(16.f));
        m_pTransformCom->Set_Speed(20.f);

        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Temple_Ground_01"), this)))   
            return E_FAIL;  
   
    }

    //m_pTransformCom->Scaling(CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Mesh_Scale());
    //m_pTransformCom->Scaling(_float3{ 0.1f,0.5f,0.1f });

    return S_OK;
}

void CTemple_Ground::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CTemple_Ground::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
 /*   if (GetKeyState('O') & 0x8000)
        m_pTransformCom->Go_Straight(fTimeDelta);*/

    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    if (m_pColliderCom != nullptr)
    {

         

        m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

        if (m_bTriggerOnOff)
        {
            m_fAccTimeDelta += fTimeDelta;  
            IsTriggerOn(TEXT("Temple_Ground_01"));
        }
        
    }
}

void CTemple_Ground::Late_Update(_float fTimeDelta)
{
    if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance * 3.f))
         m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTemple_Ground::Render()
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

    //if(m_pColliderCom != nullptr)
    //    m_pColliderCom->Render();   

    return S_OK;
}

HRESULT CTemple_Ground::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Temple_Ground"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (!strcmp(m_szName, "Temple_Ground_01"))
    {
        CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};    


        SphereDesc.fRadius = 1500.f;  
        SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f); 


        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
            TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
            return E_FAIL;


        m_pColliderCom->Set_Collider_Name("CStrongMan");
        m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);
    }

    return S_OK;
}

HRESULT CTemple_Ground::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CTemple_Ground::OnCollisionEnter(CGameObject* _pOther)
{
    if (!strcmp(_pOther->Get_Name(),"PLAYER"))
        m_pGameInstance->Event_Activate(TEXT("Temple_Ground_01"));  
       
}

void CTemple_Ground::OnCollision(CGameObject* _pOther)
{
}

void CTemple_Ground::OnCollisionExit(CGameObject* _pOther)
{
}

void CTemple_Ground::IsTriggerOn(_wstring _EventName)
{
    if (_EventName == TEXT("Temple_Ground_01")) 
    {
        m_bTriggerOnOff = true; 


        _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);   

        m_fMoveDistance = 20.f; 

        if (m_fMoveDistance > m_fAccTimeDelta + 20.f * m_fTimeDelta)
            m_pTransformCom->Go_Straight(m_fTimeDelta);
        else
        {
            m_bTriggerOnOff = false;
            m_pGameInstance->Add_DeadObject(TEXT("Layer_Control"), this);
        }
    }
}

CTemple_Ground* CTemple_Ground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTemple_Ground* pInstance = new CTemple_Ground(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CTemple_Ground");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTemple_Ground::Clone(void* pArg)
{
    CTemple_Ground* pInstance = new CTemple_Ground(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CTemple_Ground");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTemple_Ground::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
