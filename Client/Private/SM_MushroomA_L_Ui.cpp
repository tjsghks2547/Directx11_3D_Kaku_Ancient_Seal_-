#include "pch.h"
#include "SM_MushroomA_L_Ui.h"
#include "GameInstance.h"
#include "Player.h"

CSM_MushroomA_L_Ui::CSM_MushroomA_L_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CSM_MushroomA_L_Ui::CSM_MushroomA_L_Ui(const CSM_MushroomA_L_Ui& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CSM_MushroomA_L_Ui::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSM_MushroomA_L_Ui::Initialize(void* pArg)
{
    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SM_MushroomA_L_Ui");
    //UIOBJECT_DESC  Desc{};
    //
    //Desc.fSizeX = 100.f;
    //Desc.fSizeY = 100.f;
    //Desc.fX = 0.f;
    //Desc.fY = 0.f;
    //Desc.fNear = 0.f;
    //Desc.fFar = 1.f;
    //
    //Desc.fSpeedPerSec = 5.f;
    //Desc.fRotationPerSec = XMConvertToRadians(90.f);


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    //1¹øÀÚ¸® 
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -700.f,-340.f,0.f,1.f });
    m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")));


    //wsprintf(m_szMessage, TEXT("È¹µæ"));

    return S_OK;
}

void CSM_MushroomA_L_Ui::Priority_Update(_float fTimeDelta)
{

}

void CSM_MushroomA_L_Ui::Update(_float fTimeDelta)
{
    m_iCount = m_pGameInstance->Count_Item(TEXT("Prototype_GameObject_SM_MushroomA_L_Ui"));

    wsprintf(m_szMessage, TEXT("%d"), m_iCount);

    if (m_pGameInstance->isKeyEnter(DIK_2))
    {
        if(*m_pPlayer->Get_PlayerStamina() < 100.f)
            m_pPlayer->Control_PlayerStamina(10.f);
        m_pGameInstance->Sub_Item(TEXT("Prototype_GameObject_SM_MushroomA_L_Ui"));

    }
}

void CSM_MushroomA_L_Ui::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Count_Item(TEXT("Prototype_GameObject_SM_MushroomA_L_Ui")) > 0)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
    }
}

HRESULT CSM_MushroomA_L_Ui::Render()
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

    if (m_pGameInstance->Count_Item(TEXT("Prototype_GameObject_SM_MushroomA_L_Ui")) > 0)
    {
        m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(220.f, 810.f)
            , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.4f);
    }
    return S_OK;
}

HRESULT CSM_MushroomA_L_Ui::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SM_MushroomA_L_Ui"),
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

CSM_MushroomA_L_Ui* CSM_MushroomA_L_Ui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSM_MushroomA_L_Ui* pInstance = new CSM_MushroomA_L_Ui(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SM_MushroomA_L_Ui");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSM_MushroomA_L_Ui::Clone(void* pArg)
{
    CSM_MushroomA_L_Ui* pInstance = new CSM_MushroomA_L_Ui(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CSM_MushroomA_L_Ui");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSM_MushroomA_L_Ui::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
