#include "pch.h"
#include "SM_MushroomA_L.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Item_UI_ButtonE.h"
#include "ItemGainParticle.h"



CSM_MushroomA_L::CSM_MushroomA_L(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSM_MushroomA_L::CSM_MushroomA_L(const CSM_MushroomA_L& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CSM_MushroomA_L::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSM_MushroomA_L::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_SM_MushroomA_L");

    _char* NoName = " ";

    if (pArg != nullptr)
    {
        CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
        {
            if (strcmp(pObjectDesc->szName, NoName))
                strcpy_s(m_szName, pObjectDesc->szName);
        }

    }

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));  

    m_pTransformCom->Set_Speed(10.f);


    return S_OK;
}

void  CSM_MushroomA_L::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    /* 디졸브 관련 */
    if (m_isCollision)
    {
        if (m_pGameInstance->isKeyEnter(DIK_E)) 
        {
            m_bEffectOnOff = true; 

            CItemGainParticle::PARTICLE_ITEMGAIN_DESC EffectDesc{}; 
           
            _float4  startpos = {}; 
            XMStoreFloat4(&startpos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

            CTransform* pPlayerTransform = m_pPlayer->Get_Transfrom();  
            //XMStoreFloat3(&pPlayerPos, m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));  
            EffectDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();	   
            EffectDesc.pPlayerTransform = pPlayerTransform;     
            EffectDesc.pStartPos = startpos;    

            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_ItemGainParticle"), LEVEL_GAMEPLAY, TEXT("Layer_Particle"), &EffectDesc)))
            {
                MSG_BOX("Failed to Effect : ItemGain");
            }

        }
    }
}

void  CSM_MushroomA_L::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    if (m_bEffectOnOff) 
    {
        m_fAccTimeDeltaDissove += m_fTimeDelta; 
        m_fAccTimeDelta += m_fTimeDelta;

        if (m_fDissolveAmount > 1.f)    
            m_fDissolveAmount = 1.f;    
        else
        {
            m_fDissolveAmount += m_fTimeDelta * 0.8f;   
        } 

        if (m_fAccTimeDelta > 2.f)
            m_pGameInstance->Add_DeadObject(TEXT("Layer_Temple"), this);
    }

   

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
}

void  CSM_MushroomA_L::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

    if (m_isCollision && m_pGameInstance->isKeyEnter(DIK_E))
    {
        m_bIconOnOff = true;    
        /* 아이템 추가 코드 */
        m_pGameInstance->Add_Item(TEXT("Prototype_GameObject_SM_MushroomA_L_Ui"));
    }
    else if (m_isCollision && !m_bIconOnOff) // 접근시에만 Ui 나타나도록 설정 
        m_pItemUi->Late_Update(fTimeDelta);
}

HRESULT  CSM_MushroomA_L::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(1);

        m_pModelCom->Render(i);
    }

    //m_pColliderCom->Render();


    //if (m_isCollision) // 접근시에만 Ui 나타나도록 설정 
    //    m_pItemUi->Render(); 

    return S_OK;
}

HRESULT  CSM_MushroomA_L::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_MushroomA_L"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Noise Texture */
     /* Com Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;



    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 50.f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("S_HpFlower");
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this);


    ///* Com_InterAction_UI */
    m_pItemUi = CItem_UI_ButtonE::Create(m_pDevice, m_pContext);    
    m_pItemUi->Initialize(nullptr); 




    return S_OK;
}

HRESULT CSM_MushroomA_L::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))  
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveAmount, sizeof(_float))))    
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTimeDeltaDissove, sizeof(_float)))) 
        return E_FAIL;  


    return S_OK;
}

void CSM_MushroomA_L::IsTriggerOn(_wstring _EventName)
{

}

void CSM_MushroomA_L::OnCollisionEnter(CGameObject* _pOther)
{

}

void CSM_MushroomA_L::OnCollision(CGameObject* _pOther)
{
    m_isCollision = true; 

}

void CSM_MushroomA_L::OnCollisionExit(CGameObject* _pOther)
{
    m_isCollision = false; 
}

CSM_MushroomA_L* CSM_MushroomA_L::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSM_MushroomA_L* pInstance = new  CSM_MushroomA_L(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CSM_MushroomA_L");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSM_MushroomA_L::Clone(void* pArg)
{
    CSM_MushroomA_L* pInstance = new  CSM_MushroomA_L(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CSM_MushroomA_L");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CSM_MushroomA_L::Free()
{
    __super::Free();


    
    //m_pGameInstance->Sub_GameObject_To_Layer(LEVEL_GAMEPLAY,TEXT("Layer_Particle"),)

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pItemUi);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
