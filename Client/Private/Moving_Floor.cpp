#include "pch.h"
#include "Moving_Floor.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CMoving_Floor::CMoving_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CMoving_Floor::CMoving_Floor(const  CMoving_Floor& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CMoving_Floor::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CMoving_Floor::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_Moving_Floor");

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



    if (!strcmp(m_szName, "Moving_Floor_01"))
    {
         if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_3"), this)))
             return E_FAIL;

        
         m_pTransformCom->Set_Speed(10.f);
    }



    return S_OK;
}

void  CMoving_Floor::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CMoving_Floor::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

     if (m_bTriggerOnOff)   
          IsTriggerOn(TEXT("Stage_3")); 

}

void  CMoving_Floor::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CMoving_Floor::Render()
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

    
#ifdef _DEBUG
    //m_pColliderCom->Render();   
#endif // _DEBUG

  
    return S_OK;
}

HRESULT  CMoving_Floor::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleFloor_02"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    /* Com_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 200.f;
    SphereDesc.vCenter = _float3(20.f, SphereDesc.fRadius-150.f, -10.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),  
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))   
        return E_FAIL;  


    m_pColliderCom->Set_Collider_Name("Moving_Floor");    
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);     


    return S_OK;
}

HRESULT  CMoving_Floor::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CMoving_Floor::IsTriggerOn(_wstring _EventName)
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


    if (!strcmp(m_szName, "Moving_Floor_01") && _EventName == TEXT("Stage_3"))
    {
        m_bTriggerOnOff = true;

        m_fMoveHeightDistance -= m_fTimeDelta * 5.f;     // 이것도 수정  

        if (m_fMoveHeightDistance > 0.1f)   
        {
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta * 5.f));
        }
        else
        {
            m_bTriggerOnOff = false;
            m_bFirstEventEnd = true;
        }
            
    }


}

void CMoving_Floor::OnCollisionEnter(CGameObject* _pOther)
{
}

void CMoving_Floor::OnCollision(CGameObject* _pOther)
{
    if (m_bFirstEventEnd) // 처음에 false니깐 인정.    
    {
        CNavigation* pPlayerNavigation = dynamic_cast<CNavigation*>(_pOther->Find_Component(TEXT("Com_Navigation")));
        if (pPlayerNavigation == nullptr)
            int a = 4; // 여기 걸리면 넌 바부다 

        if (m_bEventEnd) {
            pPlayerNavigation->Set_CurCellIndex(204);
            m_bEventEnd = false;
        }
    }
}

void CMoving_Floor::OnCollisionExit(CGameObject* _pOther)
{
}

CMoving_Floor* CMoving_Floor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMoving_Floor* pInstance = new  CMoving_Floor(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CMoving_Floor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMoving_Floor::Clone(void* pArg)
{
    CMoving_Floor* pInstance = new  CMoving_Floor(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CMoving_Floor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CMoving_Floor::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
