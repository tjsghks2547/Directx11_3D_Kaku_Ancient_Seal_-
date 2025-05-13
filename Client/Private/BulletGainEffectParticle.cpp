#include "pch.h"
#include "BulletGainEffectParticle.h"
#include "GameInstance.h"
#include "M_Guardian_Model_A.h"

CBulletGainEffectParticle::CBulletGainEffectParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CBulletGainEffectParticle::CBulletGainEffectParticle(const CBulletGainEffectParticle& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBulletGainEffectParticle::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBulletGainEffectParticle::Initialize(void* pArg)
{
    PARTICLE_EXPLOSION_DESC* pDesc = static_cast<PARTICLE_EXPLOSION_DESC*>(pArg);

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    //m_pSokcetMatrix = pDesc->ParentSocketMarix;     

    m_pSocketMatrix = pDesc->ParentSocketMarix;    
    m_pParentState = pDesc->pParentState;

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

    return S_OK;
}

void CBulletGainEffectParticle::Priority_Update(_float fTimeDelta)
{
}

void CBulletGainEffectParticle::Update(_float fTimeDelta)
{

    _fvector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetZ(vPosition, vPosition.m128_f32[2] + 10.f));

    XMStoreFloat4x4(&m_SocketMatrix, XMLoadFloat4x4(m_pSocketMatrix));  

    m_SocketMatrix._43 += 10.f;    
    m_SocketMatrix._42 += 3.f;     

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr())* 
        XMLoadFloat4x4(&m_SocketMatrix)*
        XMLoadFloat4x4(m_pParentWorldMatrix)
    );  

    if (*m_pParentState == CM_Guardian_Model_A::STATE_ATTACK)
        m_pGameInstance->Add_DeadObject(TEXT("Layer_Particle"), this);  

    

    //XMStoreFloat4x4(&m_CombinedWorldMatrix,
    //    XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
    //    SocketMatrix *  /* 로컬 스페이스 영역 */
    //    XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */

    //m_pVIBufferCom->Explosion(fTimeDelta);  
    m_pVIBufferCom->Collect(fTimeDelta);
}

void CBulletGainEffectParticle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLUR, this); 
}

HRESULT CBulletGainEffectParticle::Render()
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


    m_pShaderCom->Begin(3);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBulletGainEffectParticle::Render_Blur()    
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


    m_pShaderCom->Begin(3);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();


    return S_OK;
}

HRESULT CBulletGainEffectParticle::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bullet_Circle"),    
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))  
        return E_FAIL;  

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing_Bullet"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;

}

CBulletGainEffectParticle* CBulletGainEffectParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBulletGainEffectParticle* pInstance = new CBulletGainEffectParticle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBulletGainEffectParticle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBulletGainEffectParticle::Clone(void* pArg)
{
    CBulletGainEffectParticle* pInstance = new CBulletGainEffectParticle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBulletGainEffectParticle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBulletGainEffectParticle::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
