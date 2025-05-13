#include "pch.h"    
#include "Crack.h"   
#include "GameInstance.h"
#include "Shader.h"
#include "Client_Defines.h" 

CCrack::CCrack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}


HRESULT  CCrack::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    //m_pTransformCom->Scaling(_float3{ 10.f,10.f,10.f });


    m_pTransformCom->Scaling(_float3(75.f, 75.f, 1.f));
    m_pTransformCom->Turn_Degree(_fvector{ 1.f,0.f,0.f,0.f }, XMConvertToRadians(90.f));
   
    
    /* 스프라이트 관련 */
    /*m_fDuration = 0.2f; 
    m_iFrame    = 0; 
    m_vOffSet   = { 1.f / 3.f, 1.f / 2.f };
    m_iCountX = 3; 
    m_iCountY = 2;

    m_iTotalFrame = 6;*/

    return S_OK;
}

void  CCrack::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
    m_fAccTimeDelta += fTimeDelta;  
    
    if (m_fDuration < m_fAccTimeDelta)
    {
        m_fAccTimeDelta = 0.f; 

    }


    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fPosition));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION),m_fPosition.y -4.f));
}

void  CCrack::Update(_float fTimeDelta)
{

}

void  CCrack::Late_Update(_float fTimeDelta)
{
    /* 여기서 버퍼 업데이트 해줘야겠다. */

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CCrack::Render()
{
   

    //if (m_fAlpha < 1.f && !m_bTest)
    //    m_fAlpha += m_fTimeDelta * 10.f;

    //else
    //{
    //    m_bTest = true;
    //    m_fAlpha -= m_fTimeDelta * 10.f;

    //    if (m_fAlpha < 0.f)
    //        m_pGameInstance->Sub_Effect(this);

    //}


    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(10);


    m_pVIBufferCom->Bind_InputAssembler();
    m_pVIBufferCom->Render();


    return S_OK;
}

void CCrack::OnCollisionEnter(CGameObject* _pOther)
{
}

void CCrack::OnCollision(CGameObject* _pOther)
{
}

void CCrack::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CCrack::Ready_Components()
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
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Crack"),    
        TEXT("Com_Texture0"), reinterpret_cast<CComponent**>(&m_pTextureCom0))))
        return E_FAIL;



    return S_OK;
}

HRESULT CCrack::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom0->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vTexcoord", &m_vOffSet, sizeof(_float2))))    
    //   return E_FAIL;   

    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vOffSetTexcoord", &m_vTexcoord, sizeof(_float2))))
    //    return E_FAIL;


    return S_OK;
}

HRESULT CCrack::Effect_Activate()
{
    if (m_fEffectLife > 0.f)
    {
        m_fEffectLife -= m_fTimeDelta;
        m_bTest = false;

    }


    return S_OK;
}



CCrack* CCrack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCrack* pInstance = new CCrack(pDevice, pContext);

    if (FAILED(pInstance->Initialize(nullptr)))
    {
        MSG_BOX("Failed to Created : CCrack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCrack::Clone(void* pArg)
{
    return nullptr;
}

void CCrack::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom0);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
