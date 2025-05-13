#include "pch.h"
#include "Button_1.h"
#include "GameInstance.h"
#include "Player.h"

CButton_1::CButton_1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CButton_1::CButton_1(const CButton_1& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CButton_1::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CButton_1::Initialize(void* pArg)
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
  
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -700.f,-400.f,0.1f,1.f });

    return S_OK;
}

void CButton_1::Priority_Update(_float fTimeDelta)
{

}

void CButton_1::Update(_float fTimeDelta)
{

}

void CButton_1::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CButton_1::Render()
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

    return S_OK;
}

HRESULT CButton_1::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Button_1"),
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

CButton_1* CButton_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CButton_1* pInstance = new CButton_1(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CButton_1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CButton_1::Clone(void* pArg)
{
    CButton_1* pInstance = new CButton_1(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CButton_1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CButton_1::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
