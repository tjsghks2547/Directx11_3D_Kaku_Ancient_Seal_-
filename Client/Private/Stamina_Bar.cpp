#include "pch.h"
#include "Stamina_Bar.h"
#include "GameInstance.h"
#include "Player.h"

CStamina_Bar::CStamina_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CStamina_Bar::CStamina_Bar(const CStamina_Bar& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CStamina_Bar::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CStamina_Bar::Initialize(void* pArg)
{
    UIOBJECT_DESC  Desc{};

    Desc.fSizeX = 150.f;
    Desc.fSizeY = 8.0f;
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

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -650.f,380.f,0.f,1.f });

    m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

    m_fStamina = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerStamina();  

    m_fCurrentStaminaState = 1.f;

    return S_OK;
}

void CStamina_Bar::Priority_Update(_float fTimeDelta)
{
}

void CStamina_Bar::Update(_float fTimeDelta)
{
    _float CurrentStamina = *m_fStamina / 100.f; 



    if (CurrentStamina < m_fCurrentStaminaState)    
        m_fCurrentStaminaState -= fTimeDelta * 0.2f;        

    if (CurrentStamina > m_fCurrentStaminaState)
        m_fCurrentStaminaState += fTimeDelta * 0.3f;

}

void CStamina_Bar::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);    
}

HRESULT CStamina_Bar::Render()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MpState", &m_fCurrentStaminaState, sizeof(_float))))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CStamina_Bar::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Stamina_Bar_UI"),
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

CStamina_Bar* CStamina_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CStamina_Bar* pInstance = new CStamina_Bar(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CStamina_Bar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStamina_Bar::Clone(void* pArg)
{
    CStamina_Bar* pInstance = new CStamina_Bar(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CStamina_Bar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStamina_Bar::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
