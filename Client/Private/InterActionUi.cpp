#include "pch.h"
#include "InterActionUi.h"
#include "GameInstance.h"

CInterActionUi::CInterActionUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CInterActionUi::CInterActionUi(const CInterActionUi& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CInterActionUi::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInterActionUi::Initialize(void* pArg)
{
    UIOBJECT_DESC  Desc{};

    Desc.fSizeX = 100.f;
    Desc.fSizeY = 100.f;
    Desc.fX = 0.f;
    Desc.fY = 0.f;
    Desc.fNear = 0.f;
    Desc.fFar = 1.f;

    Desc.fSpeedPerSec = 5.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);


    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 100.f,-150.f,0.f,1.f });   

    wsprintf(m_szMessage, TEXT("µ¹¸®±â")); 

    return S_OK;
}

void CInterActionUi::Priority_Update(_float fTimeDelta)
{

}

void CInterActionUi::Update(_float fTimeDelta)
{


}

void CInterActionUi::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CInterActionUi::Render()
{

    //_float CurrentHp = *m_iHp / 10.f;


    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;


    m_pShaderCom->Begin(6);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(925.f, 590.f)
        , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.55f);

    return S_OK;
}

HRESULT CInterActionUi::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_F"),   
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

  

    return S_OK;
}

CInterActionUi* CInterActionUi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInterActionUi* pInstance = new CInterActionUi(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CInterActionUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInterActionUi::Clone(void* pArg)
{
    CInterActionUi* pInstance = new CInterActionUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CInterActionUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInterActionUi::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
