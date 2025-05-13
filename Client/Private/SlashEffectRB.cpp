#include "pch.h"    
#include "SlashEffectRB.h"   
#include "GameInstance.h"
#include "Shader.h"
#include "Client_Defines.h" 
#include "SlashEffect.h"
#include "SlashEffect2.h"

CSlashEffectRB::CSlashEffectRB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}


HRESULT  CSlashEffectRB::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    m_ParentWorldMatrix = pDesc->_ParentMatrix;     
    m_fPosition = pDesc->fPosition;
    //m_vecSlash.resize(30);  

   
    /* 각 이펙트들 넣어놓기 */


     /* 2번 이펙트  "/" 이 검기 */
    CSlashEffect2* pSlashEffect2 = CSlashEffect2::Create(m_pDevice, m_pContext);    
    if (pSlashEffect2 == nullptr)   
        return E_FAIL;  

    //_float4 pos = { vCurPosition.m128_f32[0],vCurPosition.m128_f32[1] - 10.f,vCurPosition.m128_f32[2],1.f };      

    pSlashEffect2->Set_Poisition(m_fPosition);  
    pSlashEffect2->Set_WorldMatrix(m_ParentWorldMatrix);    
        
    PutInEffect(pSlashEffect2); 


    /* 1번 이펙트  "-" 이 검기 */
    //CSlashEffect* pSlashEffect = CSlashEffect::Create(m_pDevice, m_pContext);       
    //if (pSlashEffect == nullptr)
    //    return E_FAIL;

    ////_float4 pos = { vCurPosition.m128_f32[0],vCurPosition.m128_f32[1] - 10.f,vCurPosition.m128_f32[2],1.f };  
  
    //pSlashEffect->Set_Poisition(m_fPosition);   
    //pSlashEffect->Set_WorldMatrix(m_ParentWorldMatrix); 
    //
    //PutInEffect(pSlashEffect);


   





    /* 이걸 나중에 Add 해주기 각각 vec에서 호출하면 됨. */
    //m_pGameInstance->Add_Effect(pSlashEffect);  
 

    return S_OK;    
}



void  CSlashEffectRB::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta; 
    m_fAccTimeDelta += fTimeDelta; 

    if (m_bFirst)   
    {
        m_pGameInstance->Add_Effect(m_vecSlash.at(0));  

        m_bFirst = false; 
    }
}

void  CSlashEffectRB::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (m_fAccTimeDelta > 3.f)
        m_pGameInstance->Sub_Effect(this);

}

void  CSlashEffectRB::Late_Update(_float fTimeDelta)
{
  
}

HRESULT CSlashEffectRB::Render()
{
 
    return S_OK;
}

HRESULT CSlashEffectRB::Render_Blur()
{
    return S_OK;
}

void CSlashEffectRB::OnCollisionEnter(CGameObject* _pOther)
{
}

void CSlashEffectRB::OnCollision(CGameObject* _pOther)
{
}

void CSlashEffectRB::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CSlashEffectRB::Ready_Components()
{
    return S_OK;
}

HRESULT CSlashEffectRB::Bind_ShaderResources()
{

    return S_OK;    
}

HRESULT CSlashEffectRB::PutInEffect(CGameObject* _pEffect)    
{
    if (_pEffect == nullptr)
        return E_FAIL;  

    m_vecSlash.push_back(_pEffect);
    Safe_AddRef(_pEffect);


    return S_OK;    
}


CSlashEffectRB* CSlashEffectRB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
    CSlashEffectRB* pInstance = new CSlashEffectRB(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CSlashEffectRB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSlashEffectRB::Clone(void* pArg)
{
    return nullptr;
}

void CSlashEffectRB::Free()
{
    __super::Free();


    for (auto& iter : m_vecSlash)   
        Safe_Release(iter); 

    m_vecSlash.clear();     
}
