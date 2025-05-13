#include "pch.h"    
#include "HitEffectCircle.h"   
#include "GameInstance.h"
#include "Shader.h"
#include "Client_Defines.h" 

CHitEffectCircle::CHitEffectCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}


HRESULT  CHitEffectCircle::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    //m_pTransformCom->Scaling(_float3{ 10.f,10.f,10.f });

    return S_OK;
}

void  CHitEffectCircle::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    _matrix matWorld, matView, matBill;
    matWorld = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());

    matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));

    matBill = XMMatrixIdentity();

    matBill.r[0].m128_f32[0] = matView.r[0].m128_f32[0];
    matBill.r[0].m128_f32[2] = matView.r[0].m128_f32[2];
    matBill.r[2].m128_f32[0] = matView.r[2].m128_f32[0];
    matBill.r[2].m128_f32[2] = matView.r[2].m128_f32[2];

    matBill = XMMatrixInverse(0, matBill);

    //XMStoreFloat4x4(&m_BillMatrix, (matBill* matWorld));	    
    XMStoreFloat4x4(&m_BillMatrix, matBill);
    m_pTransformCom->Set_WorldMatrix(m_BillMatrix);
    m_pTransformCom->Scaling(_float3(m_fAccTimeDelta * 5.f, m_fAccTimeDelta * 5.f, m_fAccTimeDelta * 5.f));


    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fPosition));
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] + 30.f));
}

void  CHitEffectCircle::Update(_float fTimeDelta)
{

}

void  CHitEffectCircle::Late_Update(_float fTimeDelta)
{
    /* 여기서 버퍼 업데이트 해줘야겠다. */

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);

    //if (m_pGameInstance->isKeyEnter(DIK_K))
    //{
    //    m_bTest = false;
    //    m_fAccTimeDelta = { 1.f };   
    //    m_fAlpha = { 0.5f }; 
    //}

}

HRESULT CHitEffectCircle::Render()
{
    m_fAccTimeDelta += m_fTimeDelta;

    if (m_fAlpha < 1.f && !m_bTest)
        m_fAlpha += m_fTimeDelta * 10.f;

    else
    {
        m_bTest = true;
        m_fAlpha -= m_fTimeDelta * 10.f;

        if (m_fAlpha < 0.f)
            m_pGameInstance->Sub_Effect(this);

    }

    //else
    //{
    //    m_fAlpha -= m_fTimeDelta * 0.5f;
    //}


    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(3);


    m_pVIBufferCom->Bind_InputAssembler();
    m_pVIBufferCom->Render();


    return S_OK;
}

void CHitEffectCircle::OnCollisionEnter(CGameObject* _pOther)
{
}

void CHitEffectCircle::OnCollision(CGameObject* _pOther)
{
}

void CHitEffectCircle::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CHitEffectCircle::Ready_Components()
{
    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HitEffect2"),
        TEXT("Com_Texture0"), reinterpret_cast<CComponent**>(&m_pTextureCom0))))
        return E_FAIL;



    return S_OK;
}

HRESULT CHitEffectCircle::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom0->Bind_ShaderResource(m_pShaderCom, "g_HitEffect0", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

HRESULT CHitEffectCircle::Effect_Activate()
{
    if (m_fEffectLife > 0.f)
    {
        m_fEffectLife -= m_fTimeDelta;
        m_bTest = false;

    }


    return S_OK;
}



CHitEffectCircle* CHitEffectCircle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHitEffectCircle* pInstance = new CHitEffectCircle(pDevice, pContext);

    if (FAILED(pInstance->Initialize(nullptr)))
    {
        MSG_BOX("Failed to Created : HitEffectCircle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHitEffectCircle::Clone(void* pArg)
{
    return nullptr;
}

void CHitEffectCircle::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom0);
    Safe_Release(m_pTextureCom1);
    Safe_Release(m_pTextureCom2);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
