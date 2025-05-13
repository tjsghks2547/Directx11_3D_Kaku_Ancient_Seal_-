#include "pch.h"
#include "ItemBag.h"
#include "GameInstance.h"
#include "Player.h"

CItemBag::CItemBag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice, pContext)
{
}

CItemBag::CItemBag(const CItemBag& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CItemBag::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemBag::Initialize(void* pArg)
{

   UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg); 
   //    

   ///* Desc.fSizeX = 100.f;
   // Desc.fSizeY = 100.f;
   // Desc.fX = 0.f;
   // Desc.fY = 0.f;
   // Desc.fNear = 0.f;
   // Desc.fFar = 1.f;

   // Desc.fSpeedPerSec = 5.f;
   // Desc.fRotationPerSec = XMConvertToRadians(90.f);*/
    
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

    _float z = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2] +0.2f;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION), z));
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ Desc->fX,Desc->fY,0.1f,1.f });

    return S_OK;
}

void CItemBag::Priority_Update(_float fTimeDelta)
{

}

void CItemBag::Update(_float fTimeDelta)
{

}

void CItemBag::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CItemBag::Render()
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

HRESULT CItemBag::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ItemBag"),
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

CItemBag* CItemBag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemBag* pInstance = new CItemBag(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : ItemBag");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemBag::Clone(void* pArg)
{
    CItemBag* pInstance = new CItemBag(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : ItemBag");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemBag::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
