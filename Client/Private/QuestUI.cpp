#include "pch.h"
#include "QuestUI.h"
#include "GameInstance.h"

CQuestUI::CQuestUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CQuestUI::CQuestUI(const CQuestUI& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CQuestUI::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    //이벤트에 넣어서 해당 이벤트 클리어하면 fTimeDelta로 화면 밖으로 빠져나가는 모습 연출하자. 

    return S_OK;
}

HRESULT CQuestUI::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);   

    
    if (pDesc->szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_Quest_Ui");   

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

   

   

    if (!strcmp(m_szName, "Quest_01"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_1"), this)))
            return E_FAIL;

        wsprintf(m_szMessage, TEXT("Stage 1 주변 사물을 이용하기")); 
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 675.f,300.f,0.f,1.f });    
        m_pTransformCom->Set_Speed(10.f);   
    }


    if (!strcmp(m_szName, "Quest_02"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_2_Door"), this)))
            return E_FAIL;

        wsprintf(m_szMessage, TEXT("Stage 2 발판의 문양을 살펴보자")); 
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 675.f,200.f,0.f,1.f });    
        m_pTransformCom->Set_Speed(10.f);   
    }

    return S_OK;
}

void CQuestUI::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
}

void CQuestUI::Update(_float fTimeDelta)
{


    if (m_bTriggerOnOff)    
    {
        if (!strcmp(m_szName, "Quest_01"))  
        {
            IsTriggerOn(TEXT("Stage_1"));
        }   

        if (!strcmp(m_szName, "Quest_02"))  
        {
            IsTriggerOn(TEXT("Stage_2_Door"));  
        }
    }
}

void CQuestUI::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CQuestUI::Render()
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

    if (!m_bQuestFinish)
    {
        if (!strcmp(m_szName, "Quest_01"))
        {
            m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(1330.f, 143.f)
                , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.4f);
        }

        if (!strcmp(m_szName, "Quest_02"))
        {
            m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szMessage, _float2(1330.f, 243.f)
                , _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.f,0.f }, 0.4f);
        }
    }

    return S_OK;
}

HRESULT CQuestUI::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QuestUi"),
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

void CQuestUI::IsTriggerOn(_wstring _EventName)
{
    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);   

    m_bQuestFinish = true;

    if (_EventName == TEXT("Stage_1"))     
    {
        m_bTriggerOnOff = true; 
         

        m_fAccTimeDelta += m_fTimeDelta;    

        vCurPos.m128_f32[0] += 0.5f;       
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);        

        if (m_fAccTimeDelta > 5.f)
        {
            
            if(m_bTriggerOnOff)
                m_pGameInstance->Add_DeadObject(TEXT("Layer_UI"), this);    

            m_bTriggerOnOff = false;
            
            
            
        }
    }


    if (_EventName == TEXT("Stage_2_Door"))
    {
        m_bTriggerOnOff = true;


        m_fAccTimeDelta += m_fTimeDelta;

        vCurPos.m128_f32[0] += 0.5f;
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);    

        if (m_fAccTimeDelta > 5.f)  
        {
            m_bTriggerOnOff = false;    
            m_pGameInstance->Add_DeadObject(TEXT("Layer_UI"), this);        

        }
    }
}

CQuestUI* CQuestUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CQuestUI* pInstance = new CQuestUI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CQuestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuestUI::Clone(void* pArg)
{
    CQuestUI* pInstance = new CQuestUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : QuestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuestUI::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
