#include "pch.h" 
#include "Player.h"
#include "Body_Player.h" 
#include "GameInstance.h"
#include "Animation.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))    
        return E_FAIL;  

    return S_OK;    
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    CBody_Player::BODY_PLAYER_DESC* pDesc = static_cast<CBody_Player::BODY_PLAYER_DESC*>(pArg); 

    m_pParentState = pDesc->pParentState;   
        
    if (FAILED(__super::Initialize(pArg)))  
        return E_FAIL;  

    if (FAILED(Ready_Components())) 
        return E_FAIL;  

    // m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.f, 0.f, 1.f));
    //m_pTransformCom->Scaling(_float3(0.1f, 0.1f, 0.1f));
    m_pModelCom->SetUp_Animation(0, true);      

    //m_MotionWorldMatrix = m_pModelCom->Get_RootMotionMatrix("kaku");            

    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
}

void CBody_Player::Update(_float fTimeDelta)
{
  
   
    if (*m_pParentState & CPlayer::STATE_IDLE)
    {
        m_pModelCom->SetUp_Animation(6, true);
        m_fAnimSpeed = 1.0f;
    }

    else if (*m_pParentState & CPlayer::STATE_WALK)
    {
        m_pModelCom->SetUp_Animation(3, true);
        m_fAnimSpeed = 1.25f;
    }

    else if (*m_pParentState & CPlayer::STATE_ATTACK)
    {
        m_pModelCom->SetUp_Animation(1, false);
        m_fAnimSpeed = 1.5f;
    }

    else if (*m_pParentState & CPlayer::STATE_ATTACK2)
    {
        m_pModelCom->SetUp_Animation(5, false);
        m_fAnimSpeed = 1.2f;
    }

    else if (*m_pParentState & CPlayer::STATE_JUMP) 
    {
        m_pModelCom->SetUp_Animation(7, false);
        m_fAnimSpeed = 1.2f;
    }
    else if (*m_pParentState & CPlayer::STATE_JUMP2)    
    {
        m_pModelCom->SetUp_Animation(2, false);
        m_fAnimSpeed = 2.0f;
    }

    else if(*m_pParentState & CPlayer::STATE_RUN)
    {
        m_pModelCom->SetUp_Animation(4, true);
        m_fAnimSpeed = 1.0f;
    }

    else if (*m_pParentState & CPlayer::STATE_ROLL) 
    {
        m_pModelCom->SetUp_Animation(10, true); 
        m_fAnimSpeed = 1.0f;
    }

    else if (*m_pParentState & CPlayer::STATE_HURT)
    {
        m_pModelCom->SetUp_Animation(9, true);
        m_fAnimSpeed = 1.0f;
    }

    else if (*m_pParentState & CPlayer::STATE_FALLING)
    {
        m_pModelCom->SetUp_Animation(8, true);
        m_fAnimSpeed = 0.9f;
    }

    else if (*m_pParentState & CPlayer::STATE_PUSH)     
    {
        m_pModelCom->SetUp_Animation(11, true);
        m_fAnimSpeed = 1.0f;
    }
    
    else if (*m_pParentState & CPlayer::STATE_PULL) 
    {
        m_pModelCom->SetUp_Animation(12, true);
        m_fAnimSpeed = 1.0f;
    }


    else if (*m_pParentState & CPlayer::STATE_COMBOATTACK)  
    {
        m_pModelCom->SetUp_Animation(13, false);
        m_fAnimSpeed = 1.0f;
    }



    if (m_pModelCom->Get_VecAnimation().at(10)->isAniMationFinish() && *m_pParentState & CPlayer::STATE_ROLL)
    {
        *m_pParentState ^= STATE_ROLL;  
        *m_pParentState |= STATE_IDLE;  
        m_pModelCom->SetUp_Animation(3, true);
    }

    if (m_pModelCom->Get_VecAnimation().at(9)->isAniMationFinish() && *m_pParentState & CPlayer::STATE_HURT)
    {
        *m_pParentState ^= STATE_HURT;  
        *m_pParentState |= STATE_IDLE;  
        m_pModelCom->SetUp_Animation(3, true);
    }

    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed);
   
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
   
    
}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);   
}

HRESULT CBody_Player::Render()
{
    if (FAILED(Bind_ShaderResources())) 
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {


        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))  
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))         
            return E_FAIL;          

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // ���⼭ �̵����� ������߰ٳ�
            return E_FAIL;

        m_pShaderCom->Begin(0); 
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_Player::Render_Shadow()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(2);
        m_pModelCom->Render(i);
    }

    return S_OK;    
}

HRESULT CBody_Player::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
