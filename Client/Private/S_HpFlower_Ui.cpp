#include "pch.h"
#include "S_HpFlower_Ui.h"
#include "GameInstance.h"
#include "Player.h"

CS_HpFlower_Ui::CS_HpFlower_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CS_HpFlower_Ui::CS_HpFlower_Ui(const CS_HpFlower_Ui& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CS_HpFlower_Ui::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CS_HpFlower_Ui::Initialize(void* pArg)
{
    if (m_szName == nullptr)    
        strcpy_s(m_szName, "Prototype_GameObject_S_HpFlower_Ui");       
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


    //wsprintf(m_szMessage, TEXT("È¹µæ"));

    m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")));

    return S_OK;
}

void CS_HpFlower_Ui::Priority_Update(_float fTimeDelta)
{

}

void CS_HpFlower_Ui::Update(_float fTimeDelta)
{

    m_iCount = m_pGameInstance->Count_Item(TEXT("Prototype_GameObject_S_HpFlower_Ui"));
    
    wsprintf(m_szMessage, TEXT("%d"), m_iCount);  
   
    if (m_pGameInstance->isKeyEnter(DIK_1))
    {
        if(*m_pPlayer->Get_PlayerHp() < 10.f)
            m_pPlayer->Control_PlayerHp(1);
        m_pGameInstance->Sub_Item(TEXT("Prototype_GameObject_S_HpFlower_Ui"));

    }

}

void CS_HpFlower_Ui::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Count_Item(TEXT("Prototype_GameObject_S_HpFlower_Ui")) > 0)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);


    }
}

HRESULT CS_HpFlower_Ui::Render()
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

    //m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(925.f, 590.f)
    //    , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.55f);

    if (m_pGameInstance->Count_Item(TEXT("Prototype_GameObject_S_HpFlower_Ui")) > 0)
    {
        m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(120.f, 810.f)
            , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.4f);
    }
    return S_OK;
}

HRESULT CS_HpFlower_Ui::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_S_HpFlower_Ui"),
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

CS_HpFlower_Ui* CS_HpFlower_Ui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CS_HpFlower_Ui* pInstance = new CS_HpFlower_Ui(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : S_HpFlower_Ui");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CS_HpFlower_Ui::Clone(void* pArg)
{
    CS_HpFlower_Ui* pInstance = new CS_HpFlower_Ui(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : S_HpFlower_Ui");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CS_HpFlower_Ui::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
