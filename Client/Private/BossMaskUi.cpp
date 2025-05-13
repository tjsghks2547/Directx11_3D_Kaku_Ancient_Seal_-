#include "pch.h"
#include "BossMaskUi.h" 
#include "GameInstance.h"
#include "Boss.h"

CBossMaskUi::CBossMaskUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CBossMaskUi::CBossMaskUi(const CBossMaskUi& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CBossMaskUi::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    //이벤트에 넣어서 해당 이벤트 클리어하면 fTimeDelta로 화면 밖으로 빠져나가는 모습 연출하자. 

    return S_OK;
}

HRESULT CBossMaskUi::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);


    if (pDesc->szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_BossMaskUi");

    else
    {
        strcpy_s(m_szName, pDesc->szName);
    }


    //Desc.fSizeX = 100.f;
    //Desc.fSizeY = 100.f;
    //Desc.fX = 0.f;
    //Desc.fY = 0.f;
    //Desc.fNear = 0.f;
    //Desc.fFar = 1.f;
    //
    //Desc.fSpeedPerSec = 5.f;
    //Desc.fRotationPerSec = XMConvertToRadians(90.f);*/


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pBoss = dynamic_cast<CBoss*>(m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Boss")));            

    return S_OK;
}

void CBossMaskUi::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;  
}

void CBossMaskUi::Update(_float fTimeDelta)
{

}

void CBossMaskUi::Late_Update(_float fTimeDelta)
{

    if(m_pBoss->Get_BossStart())
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);    
}

HRESULT CBossMaskUi::Render()
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

HRESULT CBossMaskUi::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BossMaskUi"),
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

void CBossMaskUi::IsTriggerOn(_wstring _EventName)
{
 
}

CBossMaskUi* CBossMaskUi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBossMaskUi* pInstance = new CBossMaskUi(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBossMaskUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBossMaskUi::Clone(void* pArg)
{
    CBossMaskUi* pInstance = new CBossMaskUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBossMaskUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBossMaskUi::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
