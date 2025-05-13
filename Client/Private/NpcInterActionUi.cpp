#include "pch.h"
#include "NpcInterActionUi.h"
#include "GameInstance.h"

CNpcInterActionUi::CNpcInterActionUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CNpcInterActionUi::CNpcInterActionUi(const CNpcInterActionUi& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CNpcInterActionUi::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNpcInterActionUi::Initialize(void* pArg)
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

    wsprintf(m_szMessage, TEXT("대화하기"));

    return S_OK;
}

void CNpcInterActionUi::Priority_Update(_float fTimeDelta)
{

}

void CNpcInterActionUi::Update(_float fTimeDelta)
{

}

void CNpcInterActionUi::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CNpcInterActionUi::Render()
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

HRESULT CNpcInterActionUi::Ready_Components()
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

CNpcInterActionUi* CNpcInterActionUi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNpcInterActionUi* pInstance = new CNpcInterActionUi(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : NpcInterActionUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNpcInterActionUi::Clone(void* pArg)
{
    CNpcInterActionUi* pInstance = new CNpcInterActionUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : NpcInterActionUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNpcInterActionUi::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom); 
    Safe_Release(m_pVIBufferCom);   
    Safe_Release(m_pTextureCom);    
}
