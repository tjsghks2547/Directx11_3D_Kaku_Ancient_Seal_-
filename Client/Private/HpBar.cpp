#include "pch.h"
#include "HpBar.h"
#include "GameInstance.h"
#include "Player.h"

CHpBar::CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice,pContext)
{
}

CHpBar::CHpBar(const CHpBar& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CHpBar::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL; 

    return S_OK;    
}

HRESULT CHpBar::Initialize(void* pArg)
{
    UIOBJECT_DESC  Desc{}; 

    Desc.fSizeX = 250.f;
    Desc.fSizeY = 10.f;
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

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -600.f,400.f,0.f,1.f });   

    m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));  

    
    m_iHp = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerHp();      
    m_fCurrentHpState = 1.f;    
    return S_OK;    
}

void CHpBar::Priority_Update(_float fTimeDelta)
{
    
}

void CHpBar::Update(_float fTimeDelta)
{
    /* 현재 hp에 따른 조절 */

    _float CurrentHp = *m_iHp / 10.f;
    


    if (CurrentHp < m_fCurrentHpState)
        m_fCurrentHpState -= fTimeDelta * 0.2f;
        
    if (CurrentHp > m_fCurrentHpState)
        m_fCurrentHpState += fTimeDelta * 0.2f;

}

void CHpBar::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CHpBar::Render()
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

    return S_OK;
}

HRESULT CHpBar::Ready_Components()
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

CHpBar* CHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHpBar* pInstance = new CHpBar(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))  
    {
        MSG_BOX("Failed To Created : CHpBar");
        Safe_Release(pInstance);    
    }

    return pInstance;   
}

CGameObject* CHpBar::Clone(void* pArg)
{
    CHpBar* pInstance = new CHpBar(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {   
        MSG_BOX("Failed To Cloned : CHpBar");
        Safe_Release(pInstance);    
    }

    return pInstance;
}

void CHpBar::Free()
{
    __super::Free();    

    Safe_Release(m_pShaderCom); 
    Safe_Release(m_pVIBufferCom);   
    Safe_Release(m_pTextureCom);    
}
