#include "pch.h"
#include "FadeInOut.h"
#include "GameInstance.h"

CFadeInOut::CFadeInOut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CFadeInOut::CFadeInOut(const CFadeInOut& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CFadeInOut::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    //이벤트에 넣어서 해당 이벤트 클리어하면 fTimeDelta로 화면 밖으로 빠져나가는 모습 연출하자. 

    return S_OK;
}

HRESULT CFadeInOut::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);


    if (pDesc->szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_Fade_InOut");  

    else
    {
        strcpy_s(m_szName, pDesc->szName);
    }




    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CFadeInOut::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
}

void CFadeInOut::Update(_float fTimeDelta)
{
    m_fAlpah += fTimeDelta * 0.2f;

    if (m_fAlpah > 1.f)
        m_pGameInstance->Add_DeadObject(TEXT("Layer_UI"), this);
}

void CFadeInOut::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CFadeInOut::Render()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpah, sizeof(_float))))      
        return E_FAIL;      


    m_pShaderCom->Begin(9);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();


    return S_OK;
}

HRESULT CFadeInOut::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FadeInOut"),    
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

void CFadeInOut::IsTriggerOn(_wstring _EventName)
{

}

CFadeInOut* CFadeInOut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFadeInOut* pInstance = new CFadeInOut(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CQuestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFadeInOut::Clone(void* pArg)
{
    CFadeInOut* pInstance = new CFadeInOut(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : QuestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFadeInOut::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
