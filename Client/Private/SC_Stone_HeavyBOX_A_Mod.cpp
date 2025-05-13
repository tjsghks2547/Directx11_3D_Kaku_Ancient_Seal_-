#include "pch.h"
#include "SC_Stone_HeavyBOX_A_Mod.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CSC_Stone_HeavyBOX_A_Mod::CSC_Stone_HeavyBOX_A_Mod(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSC_Stone_HeavyBOX_A_Mod::CSC_Stone_HeavyBOX_A_Mod(const  CSC_Stone_HeavyBOX_A_Mod& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CSC_Stone_HeavyBOX_A_Mod::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CSC_Stone_HeavyBOX_A_Mod::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SC_Stone_HeavyBOX_A_Mod");

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

    //m_pTransformCom->Scaling(_float3{ 0.001f,0.001f,0.001f });
    m_pTransformCom->Set_Speed(10.f);


    if (!strcmp(m_szName, "Heavy_Box_1"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Object"), this)))     
            return E_FAIL;  

        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Event2"), this))) 
            return E_FAIL;
    }

    return S_OK;
}

void  CSC_Stone_HeavyBOX_A_Mod::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CSC_Stone_HeavyBOX_A_Mod::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    if (m_bTriggerOnOff)
        IsTriggerOn(TEXT("Stage_2_Object"));    

    if(m_bTriggerEvent2OnOff)
        IsTriggerOn(TEXT("Stage_2_Event2"));

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));     
}

void  CSC_Stone_HeavyBOX_A_Mod::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CSC_Stone_HeavyBOX_A_Mod::Render()
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

HRESULT  CSC_Stone_HeavyBOX_A_Mod::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Stone_HeavyBOX_A_Mod"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    /* Com_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 120.f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("HEAVY_BOX");
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);

    return S_OK;
}

HRESULT  CSC_Stone_HeavyBOX_A_Mod::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CSC_Stone_HeavyBOX_A_Mod::OnCollisionEnter(CGameObject* _pOther)
{
}

void CSC_Stone_HeavyBOX_A_Mod::OnCollision(CGameObject* _pOther)
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    CTransform* pPlayerTransform = _pOther->Get_Transfrom();
    _vector vDir = XMVector3Normalize(pPlayerTransform->Get_State(CTransform::STATE_LOOK)); 

    if (GetKeyState('F') & 0x8000 && GetKeyState('W') & 0x8000)
    {
        //m_pTransformCom->Go_Straight(m_fTimeDelta);
        vCurPos.m128_f32[0] += m_fTimeDelta * 10.f * vDir.m128_f32[0];
        vCurPos.m128_f32[2] += m_fTimeDelta * 10.f * vDir.m128_f32[2];

        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
      
        
    }
    else  if (GetKeyState('F') & 0x8000 && GetKeyState('S') & 0x8000)
    {
        vCurPos.m128_f32[0] -= m_fTimeDelta * 10.f * vDir.m128_f32[0];
        vCurPos.m128_f32[2] -= m_fTimeDelta * 10.f * vDir.m128_f32[2];

        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
    }
}   

void CSC_Stone_HeavyBOX_A_Mod::OnCollisionExit(CGameObject* _pOther)
{
}

void CSC_Stone_HeavyBOX_A_Mod::IsTriggerOn(_wstring _EventName)
{
    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);   


    if (_EventName == TEXT("Stage_2_Object"))
    {
        m_bTriggerOnOff = true;

        m_fMoveDistance -= m_fTimeDelta * 3.f;

        if (m_fMoveDistance > 0.1f)
            m_pTransformCom->Go_Straight(m_fTimeDelta * 3.f);

        else
            m_bTriggerOnOff = false;
    }


    if (_EventName == TEXT("Stage_2_Event2"))   
    {

        m_bTriggerEvent2OnOff = true;

        m_fMoveHeightDistance -= m_fTimeDelta * 2.f;      

        if (m_fMoveHeightDistance > 0.1f)   
        {
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta * 2.f));
        }
            
        else
            m_bTriggerEvent2OnOff = false;  
    }

}

CSC_Stone_HeavyBOX_A_Mod* CSC_Stone_HeavyBOX_A_Mod::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSC_Stone_HeavyBOX_A_Mod* pInstance = new  CSC_Stone_HeavyBOX_A_Mod(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SC_Stone_HeavyBOX_A_Mod");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSC_Stone_HeavyBOX_A_Mod::Clone(void* pArg)
{
    CSC_Stone_HeavyBOX_A_Mod* pInstance = new  CSC_Stone_HeavyBOX_A_Mod(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : SC_Stone_HeavyBOX_A_Mod");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSC_Stone_HeavyBOX_A_Mod::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
