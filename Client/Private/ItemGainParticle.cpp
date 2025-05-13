#include "pch.h"
#include "ItemGainParticle.h"
#include "GameInstance.h"

CItemGainParticle::CItemGainParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CItemGainParticle::CItemGainParticle(const CItemGainParticle& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CItemGainParticle::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemGainParticle::Initialize(void* pArg)
{
    PARTICLE_ITEMGAIN_DESC* pDesc = static_cast<PARTICLE_ITEMGAIN_DESC*>(pArg); 

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    
    m_pPlayerTransform = pDesc->pPlayerTransform;   
    m_fStartPos = pDesc->pStartPos; 
    
    
    /* 베지에 곡선 */


    return S_OK;
}

void CItemGainParticle::Priority_Update(_float fTimeDelta)
{
}

void CItemGainParticle::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pParentWorldMatrix) * XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

    //m_pVIBufferCom->Explosion(fTimeDelta);  

    m_fAccTimeDelta += fTimeDelta; 

    /* Gain 효과 */
    _float3 PlayerPos = {}; 
    XMStoreFloat3(&PlayerPos, m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
    PlayerPos.y += 10.f;
    XMStoreFloat3(&PlayerPos, XMVector3TransformCoord(XMLoadFloat3(&PlayerPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_CombinedWorldMatrix))));

    m_pVIBufferCom->Gain(PlayerPos, fTimeDelta);

    /* 베지에 곡선 이용 */
  /*  _float3 PlayerPos = {};
    XMStoreFloat3(&PlayerPos, m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
    PlayerPos.y += 10.f; 
    XMStoreFloat3(&PlayerPos, XMVector3TransformCoord(XMLoadFloat3(&PlayerPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_CombinedWorldMatrix))));

    m_pVIBufferCom->Bezier(m_fStartPos,PlayerPos, fTimeDelta);    */


    // 시간 몇초가 지나는지 ACC타임을 주고 몇초 지난 뒤에 삭제 EVENT 에 넣어주면 될거같음 완성. 

    if(m_fAccTimeDelta > 1.2f)
    {
        m_pGameInstance->Add_DeadObject(TEXT("Layer_Particle"), this);
    }

}

void CItemGainParticle::Late_Update(_float fTimeDelta)
{
    //m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

    //m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONLIGHT, this); 
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLUR, this);
}

HRESULT CItemGainParticle::Render()
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


    m_pShaderCom->Begin(1);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CItemGainParticle::Render_Blur()
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


    m_pShaderCom->Begin(1);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CItemGainParticle::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Gain"),
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

CItemGainParticle* CItemGainParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemGainParticle* pInstance = new CItemGainParticle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : ItemGainParticle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemGainParticle::Clone(void* pArg)
{
    CItemGainParticle* pInstance = new CItemGainParticle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CItemGainParticle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemGainParticle::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
