#include "pch.h"
#include "SC_WaterTemple_Parts_b.h"    
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"



CSC_WaterTemple_Parts_b::CSC_WaterTemple_Parts_b(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSC_WaterTemple_Parts_b::CSC_WaterTemple_Parts_b(const CSC_WaterTemple_Parts_b& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CSC_WaterTemple_Parts_b::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSC_WaterTemple_Parts_b::Initialize(void* pArg)
{
    strcpy_s(m_szName, "Prototype_GameObject_SC_WaterTemple_Parts_b");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CSC_WaterTemple_Parts_b::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CSC_WaterTemple_Parts_b::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);
}

void CSC_WaterTemple_Parts_b::Late_Update(_float fTimeDelta)
{
    if (true == m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), g_fCullingDistance))
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    }
}

HRESULT CSC_WaterTemple_Parts_b::Render()
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

HRESULT CSC_WaterTemple_Parts_b::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_WaterTemple_Parts_b"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CSC_WaterTemple_Parts_b::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

CSC_WaterTemple_Parts_b* CSC_WaterTemple_Parts_b::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSC_WaterTemple_Parts_b* pInstance = new CSC_WaterTemple_Parts_b(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SC_WaterTemple_Parts_b");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSC_WaterTemple_Parts_b::Clone(void* pArg)
{
    CSC_WaterTemple_Parts_b* pInstance = new CSC_WaterTemple_Parts_b(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : SC_WaterTemple_Parts_b");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSC_WaterTemple_Parts_b::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
