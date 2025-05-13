#include "pch.h"    
#include "Bullet.h"   
#include "GameInstance.h"
#include "Shader.h"
#include "Client_Defines.h" 
#include "M_Guardian_Model_A.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}


HRESULT  CBullet::Initialize(void* pArg)
{


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    //m_pTransformCom->Scaling(_float3{ 10.f,10.f,10.f });    

    return S_OK;
}

void  CBullet::Priority_Update(_float fTimeDelta)
{
    m_fAccTimeDelta += fTimeDelta;    

    m_fTimeDelta = fTimeDelta;      

    if (m_fAccTimeDelta > 0.2f)
    {
        if (*m_pParentState == CM_Guardian_Model_A::STATE_ATTACK_ANTICIPATE)
        {
            XMStoreFloat4x4(&m_SocketMatrix, XMLoadFloat4x4(m_pSocketMatrix));

            m_SocketMatrix._43 += 10.f;
            m_SocketMatrix._42 += 3.f;

            XMStoreFloat4x4(&m_CombinedWorldMatrix,
                XMLoadFloat4x4(&m_SocketMatrix) *
                XMLoadFloat4x4(m_pParentWorldMatrix));

        }

        _matrix matWorld, matView, matBill;
        matWorld = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());

        matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));

        matBill = XMMatrixIdentity();

        matBill.r[0].m128_f32[0] = matView.r[0].m128_f32[0];
        matBill.r[0].m128_f32[2] = matView.r[0].m128_f32[2];
        matBill.r[2].m128_f32[0] = matView.r[2].m128_f32[0];
        matBill.r[2].m128_f32[2] = matView.r[2].m128_f32[2];

        matBill = XMMatrixInverse(0, matBill);

        //XMStoreFloat4x4(&m_BillMatrix, (matBill* matWorld));	    
        XMStoreFloat4x4(&m_BillMatrix, matBill);
        m_pTransformCom->Set_WorldMatrix(m_BillMatrix);

        if (m_fAccTimeDelta < 2.5f)
        {
            
            m_pTransformCom->Scaling(_float3(m_fAccTimeDelta * 15.f, m_fAccTimeDelta * 15.f, m_fAccTimeDelta * 15.f));
            m_Scale = m_fAccTimeDelta * 15.f;   
        }
        else
            m_pTransformCom->Scaling(_float3(m_Scale, m_Scale, m_Scale));

        if (*m_pParentState == CM_Guardian_Model_A::STATE_ATTACK_ANTICIPATE)
        {
            _fvector vCurPos = { m_CombinedWorldMatrix._41 , m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f };
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
        }

        else if (*m_pParentState == CM_Guardian_Model_A::STATE_ATTACK)
        {

            m_Start = true;
          
        }

       
    }

    if(m_Start)
    {
        if (m_bSettingDir == false)
        {
            _fvector PlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION); 
            _fvector StartBulletPos = { m_CombinedWorldMatrix._41 , m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43 , 1.f };   

            XMStoreFloat4(&m_vBulletPos  ,StartBulletPos);  

            _fvector vDir = XMVectorSetW(PlayerPos - StartBulletPos, 0.f);      
            _fvector vRealDir = XMVectorSetY(vDir, 0.f);
            m_bSettingDir = true;           
            XMStoreFloat4(&m_vDir, vRealDir);   
        }

        m_fBulletLifeTime += fTimeDelta; 

        XMStoreFloat4(&m_vBulletPos,  XMLoadFloat4(&m_vBulletPos) + XMLoadFloat4(&m_vDir) * m_fTimeDelta * 4.f);         

        //_fvector vCurPos = { m_CombinedWorldMatrix._41 , m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43 += m_fTimeDelta * 50.f, 1.f };
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_vBulletPos));
    }

}

void  CBullet::Update(_float fTimeDelta)
{
 
    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));


    if (m_fBulletLifeTime > 5.f)
        m_pGameInstance->Sub_Effect(this);

}

void  CBullet::Late_Update(_float fTimeDelta)
{
    /* 여기서 버퍼 업데이트 해줘야겠다. */

    //m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLUR, this);

    //if (m_pGameInstance->isKeyEnter(DIK_K))
    //{
    //    m_bTest = false;
    //    m_fAccTimeDelta = { 1.f };   
    //    m_fAlpha = { 0.5f }; 
    //}

}

HRESULT CBullet::Render()
{
    

    if (m_fAlpha < 1.f && !m_bTest)
        m_fAlpha += m_fTimeDelta * 10.f;


    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(8);


    m_pVIBufferCom->Bind_InputAssembler();
    m_pVIBufferCom->Render();

    //m_pColliderCom->Render();

    return S_OK;
}

HRESULT CBullet::Render_Blur()
{
   

    if (m_fAlpha < 1.f && !m_bTest)
        m_fAlpha += m_fTimeDelta * 10.f;

  


    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(8);


    m_pVIBufferCom->Bind_InputAssembler();
    m_pVIBufferCom->Render();


    return S_OK;
}

void CBullet::OnCollisionEnter(CGameObject* _pOther)
{
}

void CBullet::OnCollision(CGameObject* _pOther)
{
}

void CBullet::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CBullet::Ready_Components()
{
    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bullet_Circle"),
        TEXT("Com_Texture0"), reinterpret_cast<CComponent**>(&m_pTextureCom0))))
        return E_FAIL;

    /* Com_Collider */  
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};    


    SphereDesc.fRadius = 0.1f; 
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius - 0.1f, 0.f); 


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),  
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))   
        return E_FAIL;  

    m_pColliderCom->Set_Collider_Name("Bullet");  
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);    

    return S_OK;
}

HRESULT CBullet::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
   /* if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))     
        return E_FAIL;  */

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom0->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBullet::Effect_Activate()
{
    if (m_fEffectLife > 0.f)
    {
        m_fEffectLife -= m_fTimeDelta;
        m_bTest = false;

    }


    return S_OK;
}



CBullet* CBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBullet* pInstance = new CBullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize(nullptr)))
    {
        MSG_BOX("Failed to Created : Bullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBullet::Clone(void* pArg)
{
    return nullptr;
}

void CBullet::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom0);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderCom);
}
