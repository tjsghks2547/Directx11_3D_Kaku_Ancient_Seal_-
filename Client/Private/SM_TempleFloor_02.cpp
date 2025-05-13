#include "pch.h"
#include "SM_TempleFloor_02.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CSM_TempleFloor_02::CSM_TempleFloor_02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSM_TempleFloor_02::CSM_TempleFloor_02(const  CSM_TempleFloor_02& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CSM_TempleFloor_02::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT  CSM_TempleFloor_02::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SM_TempleFloor_02");

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



   /* if (!strcmp(m_szName, "Box_Lift_1"))
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
    }*/

    return S_OK;
}

void  CSM_TempleFloor_02::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CSM_TempleFloor_02::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    //m_fTimeDelta = fTimeDelta;

  /*  if (m_bTriggerOnOff)
        IsTriggerOn(TEXT("Stage_2_Object"));


    if (m_bTriggerEvent2OnOff)
        IsTriggerOn(TEXT("Stage_2_Event2"));*/
}

void  CSM_TempleFloor_02::Late_Update(_float fTimeDelta)
{
     m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CSM_TempleFloor_02::Render()
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

HRESULT  CSM_TempleFloor_02::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleFloor_02"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT  CSM_TempleFloor_02::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CSM_TempleFloor_02::IsTriggerOn(_wstring _EventName)
{

    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

}

CSM_TempleFloor_02* CSM_TempleFloor_02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSM_TempleFloor_02* pInstance = new  CSM_TempleFloor_02(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CSM_TempleFloor_02");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSM_TempleFloor_02::Clone(void* pArg)
{
    CSM_TempleFloor_02* pInstance = new  CSM_TempleFloor_02(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CSM_TempleFloor_02");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSM_TempleFloor_02::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
