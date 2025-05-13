#include "pch.h"
#include "QuestMaskUi.h"
#include "GameInstance.h"

CQuestMaskUi::CQuestMaskUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CQuestMaskUi::CQuestMaskUi(const CQuestMaskUi& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CQuestMaskUi::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    //이벤트에 넣어서 해당 이벤트 클리어하면 fTimeDelta로 화면 밖으로 빠져나가는 모습 연출하자. 

    return S_OK;
}

HRESULT CQuestMaskUi::Initialize(void* pArg)
{
    /*UIOBJECT_DESC  Desc{};

    Desc.fSizeX = 100.f;
    Desc.fSizeY = 100.f;
    Desc.fX = 0.f;
    Desc.fY = 0.f;
    Desc.fNear = 0.f;
    Desc.fFar = 1.f;

    Desc.fSpeedPerSec = 5.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);*/


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 615.f,300.f,0.f,1.f });


    /*wsprintf(m_szMessage, TEXT("획득"));*/

    return S_OK;
}

void CQuestMaskUi::Priority_Update(_float fTimeDelta)
{

}

void CQuestMaskUi::Update(_float fTimeDelta)
{

}

void CQuestMaskUi::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CQuestMaskUi::Render()
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

   /* m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(1500.f, 140.f)
        , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.55f);*/

    return S_OK;
}

HRESULT CQuestMaskUi::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QuestMaskUi"),
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



CQuestMaskUi* CQuestMaskUi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CQuestMaskUi* pInstance = new CQuestMaskUi(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CQuestMaskUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuestMaskUi::Clone(void* pArg)
{
    CQuestMaskUi* pInstance = new CQuestMaskUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : QuestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuestMaskUi::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
