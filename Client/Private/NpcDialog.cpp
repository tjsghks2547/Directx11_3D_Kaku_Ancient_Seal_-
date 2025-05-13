#include "pch.h"
#include "NpcDialog.h"
#include "GameInstance.h"

CNpcDialog::CNpcDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CNpcDialog::CNpcDialog(const CNpcDialog& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CNpcDialog::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNpcDialog::Initialize(void* pArg)
{
    UIOBJECT_DESC  Desc{};

    Desc.fSizeX = 1000.f;
    Desc.fSizeY = 300.f;
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
    //m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(90.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f,-250.f,0.f,1.f });

    wsprintf(m_szMessage, TEXT("조심해! 굴러오는 돌을 피해 제단 까지 가야해."));

    int Font_Length = sizeof(m_szMessage);  
    return S_OK;
}

void CNpcDialog::Priority_Update(_float fTimeDelta)
{
    // 여기서 대화 저거 작성하면 될듯. 

     

    m_fAccTimeDelta += 0.1f;
    // m_szMessage의 앞부분 displayedChars 개의 문자를 복사
    wcsncpy_s(m_szTemp, m_szMessage, (int)m_fAccTimeDelta); 
    m_szTemp[(int)m_fAccTimeDelta] = L'\0'; // 널 종료               
}

void CNpcDialog::Update(_float fTimeDelta)
{

}

void CNpcDialog::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CNpcDialog::Render()
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


    m_pShaderCom->Begin(7);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szTemp, _float2(550.f, 690.f)
        , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.55f);
    return S_OK;
}

HRESULT CNpcDialog::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_NpcDialog_Ui"),
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

CNpcDialog* CNpcDialog::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNpcDialog* pInstance = new CNpcDialog(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : NpcDialog");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNpcDialog::Clone(void* pArg)
{
    CNpcDialog* pInstance = new CNpcDialog(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : NpcDialog");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNpcDialog::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
