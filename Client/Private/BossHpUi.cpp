#include "pch.h"
#include "BossHpUi.h" 
#include "GameInstance.h"
#include "Boss.h"

CBossHpUi::CBossHpUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CBossHpUi::CBossHpUi(const CBossHpUi& Prototype)    
    :CUIObject(Prototype)
{
}

HRESULT CBossHpUi::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    //이벤트에 넣어서 해당 이벤트 클리어하면 fTimeDelta로 화면 밖으로 빠져나가는 모습 연출하자. 

    return S_OK;
}

HRESULT CBossHpUi::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);


    if (pDesc->szName == nullptr)   
        strcpy_s(m_szName, "Prototype_GameObject_BossHpUi");    

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

    wsprintf(m_szMessage, TEXT("크자카"));     

    m_pBossHp = dynamic_cast<CMonster*>(m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Boss")))->Get_MonsterHp();

    m_fCurrentHpState = 1.f;    

    return S_OK;
}

void CBossHpUi::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
}

void CBossHpUi::Update(_float fTimeDelta)
{
    /* 여기서 보스 체력 바에 따른 비율 조정하기 */
    _float CurrentHp = *m_pBossHp / 100.f;   

    if (CurrentHp < m_fCurrentHpState)  
        m_fCurrentHpState -= fTimeDelta * 0.4f;        

}

void CBossHpUi::Late_Update(_float fTimeDelta)
{

    if (m_pBoss->Get_BossStart())
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CBossHpUi::Render()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_HpState", &m_fCurrentHpState, sizeof(_float))))   
        return E_FAIL;  

    m_pShaderCom->Begin(1);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(740.f, 12.5f)
        , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.7f);

    return S_OK;
}

HRESULT CBossHpUi::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hp_Bar_UI"),
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

void CBossHpUi::IsTriggerOn(_wstring _EventName)
{

}

CBossHpUi* CBossHpUi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBossHpUi* pInstance = new CBossHpUi(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : BossHpUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBossHpUi::Clone(void* pArg)
{
    CBossHpUi* pInstance = new CBossHpUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : BossHpUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBossHpUi::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
