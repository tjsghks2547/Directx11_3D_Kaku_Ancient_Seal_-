#include "pch.h"
#include "Boss.h"
#include "GameInstance.h"
#include "Animation.h"
#include "BossHpUi.h"
#include "MonsterStunBar.h"
#include "Boss_Left_Arm.h"
#include "Boss_Right_Arm.h"
#include "FireBreath.h"
#include <random>	




CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss::CBoss(const CBoss& Prototype)	
	:CMonster(Prototype)
{
}

HRESULT CBoss::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
	/*if (m_szName == nullptr)
		strcpy_s(m_szName, "Prototype_GameObject_M_Guardian_Model_D");

	_char* NoName = " ";*/

	if (pArg != nullptr)
	{
		CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
		{
			//if (strcmp(pObjectDesc->szName, NoName))	
			strcpy_s(m_szName, pObjectDesc->szName);
		}

	}

	m_iHp = 100;	
		
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_Speed(10.f);
	m_pTransformCom->Set_RotationSpeed(10.f);
	
		
	m_iPatternIndex = 0;	

	return S_OK;
}

void CBoss::Priority_Update(_float fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;	
	m_fTimeDelta = fTimeDelta;	


	BOSS_STATE partterns[] =
	{
		//BOSS_STATE::STATE_IDLE,
		BOSS_STATE::STATE_ATTACK_PARTTERN_1,
		BOSS_STATE::STATE_ATTACK_PARTTERN_2,	
		BOSS_STATE::STATE_ATTACK_PARTTERN_3,	
		BOSS_STATE::STATE_ATTACK_PARTTERN_4,
		//BOSS_STATE::STATE_DEAD,				
	};

	std::random_device rd; 
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);		

	_vector pPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);	
	_vector MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);	

	float distance = XMVectorGetX(XMVector3Length(pPlayerPos - MonsterPos));


	if (m_iHp <= 0 && !m_bDead)
	{
		m_bDead = true;	
		m_pModelCom->SetUp_Animation(7, false);		
		m_iState = STATE_DEAD;	
		m_fAnimationSpeed = 5.f;	
		m_bBossStart = false; 

	}

	else if (!m_bDead)
	{

		if (distance > 250.f)
		{
			m_iState = 0;
			m_iState = STATE_IDLE;		
			m_pModelCom->SetUp_Animation(8, true);	
			m_fAnimationSpeed = 1.f;	

			
		}

		else
		{
			m_bBossStart = true;	

			if (m_dwtime + 12000 < GetTickCount64())	
			{
				int randomIndex = dis(gen);

				m_iState = partterns[1];	

				if (m_iPatternIndex >= 3)
					m_iPatternIndex = -1;

				m_iPatternIndex++;
					
				m_dwtime = GetTickCount64();
			}
		}

	}



	/* 애니메이션 관련 설정 */


	/* 패턴 1*/
	if (m_iState == STATE_ATTACK_PARTTERN_1)	
	{	
		m_pModelCom->SetUp_Animation(1, false);		
		// 여기가 STAT_ATTACK_AL이 될거임 
		if (m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish())
			m_iState = STATE_ATTACK_AR;		
	}

	if (m_iState == STATE_ATTACK_AR)	
	{
		m_pModelCom->SetUp_Animation(2, false);			
		if (m_pModelCom->Get_VecAnimation().at(2)->isAniMationFinish())		
			m_iState = STATE_ATTACK_DOUBLE_PUNCH;	
	}

	if (m_iState == STATE_ATTACK_DOUBLE_PUNCH)	
	{
		m_pModelCom->SetUp_Animation(0, false);		
		if (m_pModelCom->Get_VecAnimation().at(0)->isAniMationFinish())
		{
			m_iState = STATE_IDLE;
			m_pModelCom->SetUp_Animation(8, true);
		}
	}

	/* 할퀴기 공격 (패턴 2)  */
	if (m_iState == STATE_ATTACK_PARTTERN_2)		
	{	
		m_pModelCom->SetUp_Animation(3, false);		
		if (m_pModelCom->Get_VecAnimation().at(3)->isAniMationFinish())		
			m_iState = STATE_ATTACK_BR;	
	}

	if (m_iState == STATE_ATTACK_BR)		
	{
		m_pModelCom->SetUp_Animation(4, false);	
		if (m_pModelCom->Get_VecAnimation().at(4)->isAniMationFinish())
		{
			m_iState = STATE_IDLE;
			m_pModelCom->SetUp_Animation(8, true);
		}
	}


	if (m_iState == STATE_ATTACK_PARTTERN_3)
	{
		CFireBreath::PARTICLE_FIREBREATH_DESC EffectDesc{};

		
		m_pModelCom->SetUp_Animation(6, false);
		if (m_pModelCom->Get_VecAnimation().at(6)->isAniMationFinish())
		{
			m_iState = STATE_IDLE;	
			m_pModelCom->SetUp_Animation(8, true);	
		}
	}

	/* 브레스 패턴 */
	if (m_iState == STATE_ATTACK_PARTTERN_4)
	{
		m_fPatternTimeAcc += fTimeDelta;	 
		

		m_pModelCom->SetUp_Animation(5, false);
		if (m_pModelCom->Get_VecAnimation().at(5)->isAniMationFinish())
		{
			m_iState = STATE_IDLE;
			m_pModelCom->SetUp_Animation(8, true);
			m_fPatternTimeAcc = 0.f; 
		}
	}
	
	if (m_iState != STATE_IDLE)
		m_bBossAttackOnOff = true;
	else
		m_bBossAttackOnOff = false;		

#pragma region 왼팔 부분
	m_pBoss_Left_Arm->Priority_Update(fTimeDelta);
#pragma endregion 

#pragma region 오른팔 부분
	m_pBoss_Right_Arm->Priority_Update(fTimeDelta);	
#pragma endregion 


	if (m_fPatternTimeAcc > 2.3f && m_fPatternTimeAcc < 5.f)
	{
		m_fBreathTimeAcc += fTimeDelta; 

		if (m_fBreathTimeAcc > 0.1f)
		{

			CFireBreath::PARTICLE_FIREBREATH_DESC EffectDesc{};

			_float4  startpos = {};
			XMStoreFloat4(&startpos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			//XMStoreFloat3(&pPlayerPos, m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));  
			EffectDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
			EffectDesc.pStartPos = startpos;
			EffectDesc.ParentSocketMarix = m_pModelCom->Get_BoneMatrix("Bip001-Head");



			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Fire"), LEVEL_GAMEPLAY, TEXT("Layer_Particle"), &EffectDesc)))
			{
				MSG_BOX("Failed to Effect : ItemGain");
			}

			m_fBreathTimeAcc = 0.f;
		}
	}

}

void CBoss::Update(_float fTimeDelta)	
{
	//if (true == m_pModelCom->Play_Animation(fTimeDelta * m_fAnimationSpeed))
	//	int a = 10;

	_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();




	_vector pPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector Dir = pPlayerPos - MonsterPos;


	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		test = { 0.f,0.f,0.f,1.f };
	/* 루트 모션 애니메션 코드 */
	m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("Dummy001");	


	if (m_iState == STATE_IDLE)
	{
		//외적 구해야겟네	
		_vector vLook = XMVector2Normalize(_fvector{ m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[0],m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[2] });
		_vector vDir = XMVector2Normalize(_fvector{ XMVectorGetX(Dir),XMVectorGetZ(Dir) });

		float dotResult = XMVectorGetX(XMVector2Dot(vLook, vDir));

		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector2Cross(vDir, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		if (fabsf(Radian) > XMConvertToRadians(0.1f))
		{
			m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, Radian);	
			m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));	
		}
	}
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));



#pragma region 왼팔 부분
	m_pBoss_Left_Arm->Update(fTimeDelta);
#pragma endregion 

#pragma region 오른팔 부분
	m_pBoss_Right_Arm->Update(fTimeDelta);	
#pragma endregion 
}

void CBoss::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	
	m_pBoss_Left_Arm->Late_Update(fTimeDelta);
	m_pBoss_Right_Arm->Late_Update(fTimeDelta);

}

HRESULT CBoss::Render()
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



		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);


	}

#ifdef _DEBUG
	//m_pColliderCom->Render();
#endif 
	//m_pMonsterHpBar->Render();

	return S_OK;
}

HRESULT CBoss::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC   Desc{};



	/* Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


	SphereDesc.fRadius = 300.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pColliderCom->Set_Collider_Name("Boss");
	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);



	///* Boss Parts*/
	CBoss_Left_Arm::BODY_DESC Body_Desc{}; 
	Body_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("Bip001-L-Hand");

	m_pBoss_Left_Arm = CBoss_Left_Arm::Create(m_pDevice, m_pContext,&Body_Desc);			
	m_pBoss_Left_Arm->SetParentWorldMatrix(m_pTransformCom->Get_WorldMatrix_Ptr());			
	m_pBoss_Left_Arm->Set_Parent_State_Ptr(&m_iState);	
	m_pBoss_Left_Arm->Set_Attack_State_Ptr(&m_bBossAttackOnOff);	


	Body_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("Bip001-R-Hand");

	m_pBoss_Right_Arm = CBoss_Right_Arm::Create(m_pDevice, m_pContext, &Body_Desc);	
	m_pBoss_Right_Arm->SetParentWorldMatrix(m_pTransformCom->Get_WorldMatrix_Ptr());		
	m_pBoss_Right_Arm->Set_Parent_State_Ptr(&m_iState);		
	m_pBoss_Right_Arm->Set_Attack_State_Ptr(&m_bBossAttackOnOff);	
	//m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, m_pMonsterArm	);		


	///* Noise Texture */
	///* Com Texture */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}


void CBoss::OnCollisionEnter(CGameObject* _pOther)
{
	_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();

	if (!strcmp(_pOther->Get_Name(), "PLAYER_WEAPON"))
	{
		if (m_iHp > 0)
		{
			m_iHp--; // 애니메이션이 한번 진행될때 한번만 데미지가 입혀지도록 해야함.    
		}
	}

	//if (!strcmp(_pOther->Get_Name(), "PLAYER"))
	//{


	//	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	//	float pushStrength = 10.0f; // 밀리는 힘 크기	
	//	_vector pushDirection = vLook * pushStrength * m_fTimeDelta;

	//	_vector playerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	//	playerPos += pushDirection;

	//	// 새로운 위치 적용
	//	m_pPlayer->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, playerPos);

	//}


}

void CBoss::OnCollision(CGameObject* _pOther)
{
	_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();

	if (!strcmp(_pOther->Get_Name(), "PLAYER"))
	{


		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));	
		float pushStrength = 10.0f; // 밀리는 힘 크기	
		_vector pushDirection = vLook * pushStrength * m_fTimeDelta * -1.f;	

		_vector playerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);	
		playerPos += pushDirection;	

		// 새로운 위치 적용
		m_pPlayer->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, playerPos);	

		if (m_iHp <= 0)
		{
			//// 여기서 게임 오브젝트에서 삭제를 다뤄야겠네 
			//if (AnimationFinish && m_iState == STATE_DEAD && m_bDead)
			//{
			//	m_pGameInstance->Sub_ObjCollider(GROUP_TYPE::MONSTER, this);
			//	m_pGameInstance->Sub_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), this);
			//}
		}

	}




}


void CBoss::OnCollisionExit(CGameObject* _pOther)
{
	int a = 4;
}



HRESULT CBoss::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveAmount, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTimeDeltaDissove, sizeof(_float))))
	//	return E_FAIL;


	return S_OK;
}

CBoss* CBoss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss* pInstance = new CBoss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss::Clone(void* pArg)
{
	CBoss* pInstance = new CBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss::Free()
{
	__super::Free();

	Safe_Release(m_pBoss_Left_Arm);
	Safe_Release(m_pBoss_Right_Arm);

}
