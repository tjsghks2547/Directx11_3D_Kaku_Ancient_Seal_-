#include "pch.h"    
#include "SlashEffect.h"   
#include "GameInstance.h"
#include "Shader.h"
#include "Client_Defines.h" 

CSlashEffect::CSlashEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}


HRESULT  CSlashEffect::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))  
        return E_FAIL;

    if (FAILED(Ready_Components())) 
        return E_FAIL;  


    //m_pTransformCom->Scaling(_float3{ 0.1f,0.5f,0.1f });      // 지금 뒤에서보니깐 컬링하지말아줘야함  



    //m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(80.f));        
    //m_pTransformCom->Turn_Degree(_fvector{ 0.f,0.f,1.f,0.f }, XMConvertToRadians(30.f));//  , XMConvertToRadians(180.f));   

    //m_pTransformCom->Set_Speed(1.f);

    return S_OK;
}

void  CSlashEffect::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);   

    m_fTimeDelta = fTimeDelta;
    
    m_fAccTimeDelta += fTimeDelta;  

    if (m_fAccTimeDelta > 0.3f)
    {
        if (m_bFirst)
        {
            //_vector vLook = { m_ParentWorldMatrix._31, m_ParentWorldMatrix._32, m_ParentWorldMatrix._33, 0.f };
            //_vector vNoramlizeLook = XMVector3Normalize(vLook);

            

            m_pTransformCom->Set_WorldMatrix(m_ParentWorldMatrix);  

            
            //m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(45.f));
            //m_pTransformCom->Turn_Degree(_fvector{ 0.f,0.f,1.f,0.f }, XMConvertToRadians(180.f));
            m_pTransformCom->Scaling(_float3{ 0.15f,0.5f,0.15f });
           
            m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-45.f));
            m_fPosition.y += 10.f;
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fPosition)); 

        


            m_bFirst = false;

            m_pGameInstance->Play_Sound(L"H_Kaku_Atk_RRA_Distance.ogg", SOUND_EFFECT, 0.5f);
            m_pGameInstance->Play_Sound(L"H_Kaku_Atk_LLLToAtk_2.ogg", SOUND_EFFECT, 0.5f);   
        }
    }
    //m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));   

}

void  CSlashEffect::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);    

}

void  CSlashEffect::Late_Update(_float fTimeDelta)
{
    if (m_fAccTimeDelta > 0.3f)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLUR, this); 
        //m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
    }
}

HRESULT CSlashEffect::Render()
{
    if (m_fAccTimeDelta > 0.3f)
    {
        m_fAccTimeDelta += m_fTimeDelta;
        m_fShaderAccTimeDelta += m_fTimeDelta;

        if (FAILED(Bind_ShaderResources()))
            return E_FAIL;

        _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

        for (size_t i = 0; i < iNumMeshes; i++)
        {
            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
                return E_FAIL;

            m_pShaderCom->Begin(2);

            m_pModelCom->Render(i);
        }
    }

    if (m_fAccTimeDelta > 0.7f)
        m_pGameInstance->Sub_Effect(this);


    return S_OK;
}

HRESULT CSlashEffect::Render_Blur() 
{
    if (m_fAccTimeDelta > 0.3f)
    {
        m_fAccTimeDelta += m_fTimeDelta;
        m_fShaderAccTimeDelta += m_fTimeDelta;

        if (FAILED(Bind_ShaderResources()))
            return E_FAIL;

        _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

        for (size_t i = 0; i < iNumMeshes; i++)
        {
            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
                return E_FAIL;

            m_pShaderCom->Begin(2);

            m_pModelCom->Render(i);
        }
    }

    if (m_fAccTimeDelta > 0.7f)
        m_pGameInstance->Sub_Effect(this);


    return S_OK;
}

void CSlashEffect::OnCollisionEnter(CGameObject* _pOther)
{
}

void CSlashEffect::OnCollision(CGameObject* _pOther)
{
}

void CSlashEffect::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CSlashEffect::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshNoDefferd"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))    
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Slash"),      
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))      
        return E_FAIL;  

 
    return S_OK;
}

HRESULT CSlashEffect::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))    
        return E_FAIL;  

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;  
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;      

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fShaderAccTimeDelta, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}


CSlashEffect* CSlashEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSlashEffect* pInstance = new CSlashEffect(pDevice, pContext);

    if (FAILED(pInstance->Initialize(nullptr)))
    {
        MSG_BOX("Failed to Created : CSlashEffect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSlashEffect::Clone(void* pArg)        
{
    return nullptr; 
}

void CSlashEffect::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);  
    Safe_Release(m_pShaderCom);
}
