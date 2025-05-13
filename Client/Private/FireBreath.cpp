#include "pch.h"
#include "FireBreath.h"
#include "GameInstance.h"

CFireBreath::CFireBreath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CFireBreath::CFireBreath(const CFireBreath& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CFireBreath::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CFireBreath::Initialize(void* pArg)
{
    PARTICLE_FIREBREATH_DESC* pDesc = static_cast<PARTICLE_FIREBREATH_DESC*>(pArg);

    if (FAILED(__super::Initialize(pDesc))) 
        return E_FAIL;

    if (FAILED(Ready_Components())) 
        return E_FAIL;

    
    m_pSokcetMatrix = pDesc->ParentSocketMarix;     

    XMStoreFloat4x4(&m_Matrix ,XMLoadFloat4x4(pDesc->ParentSocketMarix));       

    XMStoreFloat4x4(&m_identityMatrix, XMMatrixIdentity()); 



    return S_OK;
}

void CFireBreath::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
}

void CFireBreath::Update(_float fTimeDelta)
{
    
    /* 더 쉽게가자 CombinedMatrix를 업데이트를 안하면되네 ㅋㅋㅋ */
    /*XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pSokcetMatrix) *
        XMLoadFloat4x4(m_pParentWorldMatrix) * 
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));*/


     XMStoreFloat4x4(&m_CombinedWorldMatrix,
         XMLoadFloat4x4(&m_Matrix) *
         XMLoadFloat4x4(m_pParentWorldMatrix) *
         XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));   

  
    m_fAccTimeDelta += fTimeDelta;  
    

    m_pVIBufferCom->FireBreath(fTimeDelta,XMLoadFloat4x4(&m_CombinedWorldMatrix));
        
    /*  2월 10일 작업 */


    if (m_fAccTimeDelta > 3.f)
        m_pGameInstance->Add_DeadObject(TEXT("Layer_Particle"),this);

    /* 저 Combined Matrix를 이제 파티클에 넣어줘야 하는데. */
    // 해당점은 


}

void CFireBreath::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLUR, this);     
    //m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CFireBreath::Render()
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



    if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(m_pShaderCom, "g_AlphaMaskTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveAmount, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTimeDeltaDissove, sizeof(_float))))
        return E_FAIL;


    if (FAILED(m_pShaderCom->Bind_RawValue("g_vTexSize", &m_vOffSet, sizeof(_float2))))     
        return E_FAIL;      



    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Bind_InputAssembler();

    //m_pVIBufferCom->Render();
    m_pVIBufferCom->Render_Spawn(); 
    return S_OK;
}

HRESULT CFireBreath::Render_Blur()
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



    if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(m_pShaderCom, "g_AlphaMaskTexture", 0)))     
        return E_FAIL;  

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveAmount, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTimeDeltaDissove, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vTexSize", &m_vOffSet, sizeof(_float2))))   
        return E_FAIL;  


    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Bind_InputAssembler();

    //m_pVIBufferCom->Render();
    m_pVIBufferCom->Render_Spawn();     
    
    return S_OK;
}

HRESULT CFireBreath::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fire"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Fire_Instancing"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Noise Texture */
    /* Com Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Alpha_Fire"),       
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pAlphaTextureCom))))    
        return E_FAIL;

    return S_OK;

}

CFireBreath* CFireBreath::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFireBreath* pInstance = new CFireBreath(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CFireBreath");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFireBreath::Clone(void* pArg)
{
    CFireBreath* pInstance = new CFireBreath(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CFireBreath");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFireBreath::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pAlphaTextureCom);   
}
