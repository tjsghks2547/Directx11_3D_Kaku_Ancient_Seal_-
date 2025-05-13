#include "pch.h"
#include "HitParticle.h"
#include "GameInstance.h"

CHitParticle::CHitParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CHitParticle::CHitParticle(const CHitParticle& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CHitParticle::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CHitParticle::Initialize(void* pArg)
{
    PARTICLE_HITPARTICLE_DESC* pDesc = static_cast<PARTICLE_HITPARTICLE_DESC*>(pArg);

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CHitParticle::Priority_Update(_float fTimeDelta)
{
}

void CHitParticle::Update(_float fTimeDelta)
{
   
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pParentWorldMatrix) * XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

    //_matrix CombineMatrix = XMLoadFloat4x4(&m_CombinedWorldMatrix);
    //XMVector3Normalize(CombineMatrix.r[0]) * 0.5f


  /*  Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
    Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
    Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);*/

    m_pVIBufferCom->Explosion(fTimeDelta);  
    //m_pVIBufferCom->Rise(fTimeDelta);
}

void CHitParticle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CHitParticle::Render()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))   
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;


    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CHitParticle::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hit"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;

}

CHitParticle* CHitParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHitParticle* pInstance = new CHitParticle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : HitParticle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHitParticle::Clone(void* pArg)
{
    CHitParticle* pInstance = new CHitParticle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : HitParticle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHitParticle::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
