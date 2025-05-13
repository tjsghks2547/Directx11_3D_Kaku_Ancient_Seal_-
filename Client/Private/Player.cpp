#include "pch.h" 
#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "Particle_Explosion.h"	
#include "Weapon.h"
#include "SlashEffect.h"
#include "SlashEffect2.h"
#include "HitParticle.h"
#include "SlashEffectRB.h"


CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice,pContext)
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CContainerObject(Prototype)
	,m_iState(Prototype.m_iState)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	strcpy_s(m_szName, "PLAYER");

    CGameObject::GAMEOBJECT_DESC        Desc{};
 
    Desc.fSpeedPerSec = 45.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);


    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL; 

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

	///* 루트 모션 애니메션 코드 */
	//m_pRootMatrix = m_pModel->Get_RootMotionMatrix("kaku");		
	

	// 시작 지점의 플레이어 위치 1_23일 
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 232.f,62.f,60.f,1.f });		
	


	//디버깅용 시작지점 플레이어 위치 stage 3 올라가는 발판 
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -1022.f,-141.f,904.f,1.f });

	//// 디버깅용 시작지점 플레이어 위치 stage 4 몬스터와 싸움  
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -562.f,40.f,839.f,1.f });

	//// 디버깅용 시작지점 플레이어 위치 stage 5 돌 이벤트   
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -562.f,39.f,197.f,1.f });		

	//// 디버깅용 시작지점 플레이어 위치 stage 6 맵 이동 이벤트
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -559.f,312.f,-688.f,1.f });	

	//// 디버깅용 시작지점 플레이어 위치 보스
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 238.f,88.f,1251.f,1.f });		


	m_pTransformCom->Scaling(_float3{ 0.045f, 0.045f, 0.045f });

	m_iHp = 10;		
	m_fStamina = 100.f;		

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{	
	// 여기 실험중 ㅋ
	/*if (m_pGameInstance->isKeyEnter(DIK_O))
		m_iHp--;	

	if (m_pGameInstance->isKeyEnter(DIK_P))
		m_iStamina--;
	*/
	//

	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);	
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft  = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	/*if (m_pGameInstance->isKeyPressed(DIK_LCONTROL))
		m_iState = STATE_RUN;*/

#pragma region Mouse_Input
	/* 마우스 입력 */
	if (m_pGameInstance->isMouseEnter(DIM_LB))			
	{
		_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

		// vLook과 캐릭터의 look 의 내적구하기 
		_vector PlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}	

		if (m_iState ^ STATE_ATTACK)
		{
			// m_iState가 STATE_ATTACK이 아닐 때 실행할 로직
			

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			vCurPosition.m128_f32[1] += m_pNavigationCom->Compute_Height(vCurPosition);	
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);

			
			m_iState = STATE_ATTACK;

			CSlashEffect2* pSlashEffect2 = CSlashEffect2::Create(m_pDevice, m_pContext);
			if (pSlashEffect2 == nullptr)
				return;

			//_float4 pos = { vCurPosition.m128_f32[0],vCurPosition.m128_f32[1] - 10.f,vCurPosition.m128_f32[2],1.f };
			_float4 pos = { vCurPosition.m128_f32[0],m_pNavigationCom->Compute_Height(vCurPosition),vCurPosition.m128_f32[2],1.f };
			pSlashEffect2->Set_Poisition(pos);
			pSlashEffect2->Set_WorldMatrix(*m_pTransformCom->Get_WorldMatrix_Ptr());	
			m_pGameInstance->Add_Effect(pSlashEffect2);	

			
		

			
		}
		else if (m_iState & STATE_ATTACK)// m_iState == STATE_ATTACK	 
		{
			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			vCurPosition.m128_f32[1] += m_pNavigationCom->Compute_Height(vCurPosition);	
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);	
	
			m_iState = STATE_ATTACK2;


			/// 
			CSlashEffect* pSlashEffect = CSlashEffect::Create(m_pDevice, m_pContext);
			if (pSlashEffect == nullptr)
				return;

			//_float4 pos = { vCurPosition.m128_f32[0],vCurPosition.m128_f32[1] - 10.f,vCurPosition.m128_f32[2],1.f };
			_float4 pos = { vCurPosition.m128_f32[0],m_pNavigationCom->Compute_Height(vCurPosition),vCurPosition.m128_f32[2],1.f };	
			pSlashEffect->Set_Poisition(pos);
			pSlashEffect->Set_WorldMatrix(*m_pTransformCom->Get_WorldMatrix_Ptr());
			m_pGameInstance->Add_Effect(pSlashEffect);
			////
		}

		


	}

	else
	{
		if ((m_iState & STATE_ATTACK) && m_pModel->GetAniFinish())	
		{
			m_iState ^= STATE_ATTACK;	
			m_iState |= STATE_IDLE;	
		}

		else if((m_iState & STATE_ATTACK2) && m_pModel->GetAniFinish())
		{
			m_iState ^= STATE_ATTACK2;	
			m_iState |= STATE_IDLE;	
		}

	}



	if (m_pGameInstance->isMouseEnter(DIM_RB))
	{

		_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

		// vLook과 캐릭터의 look 의 내적구하기 
		_vector PlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		if (m_iState ^ STATE_COMBOATTACK)
		{
			// m_iState가 STATE_ATTACK이 아닐 때 실행할 로직


			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			vCurPosition.m128_f32[1] += m_pNavigationCom->Compute_Height(vCurPosition);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);


			m_iState = STATE_COMBOATTACK;

		}


		/* RB 이펙트 */

		GAMEOBJECT_DESC Desc{};

		_float4 pos = { vCurPosition.m128_f32[0],m_pNavigationCom->Compute_Height(vCurPosition),vCurPosition.m128_f32[2],1.f };
	

		Desc._ParentMatrix = *m_pTransformCom->Get_WorldMatrix_Ptr();
		Desc.fPosition = pos;	
		
		CSlashEffectRB* pSlashRBEffect = CSlashEffectRB::Create(m_pDevice, m_pContext, &Desc);			
		if (pSlashRBEffect == nullptr)
			return;

		m_pGameInstance->Add_Effect(pSlashRBEffect);		
		

	
	}

	else
	{
		if ((m_iState & STATE_COMBOATTACK) && m_pModel->GetAniFinish())
		{
			m_iState ^= STATE_COMBOATTACK;
			m_iState |= STATE_IDLE;
		}
	}


#pragma endregion 

#pragma region KeyBoard Input

#pragma region PULL and PUSH
	if (GetKeyState('F') & 0x8000 && GetKeyState('W') & 0x8000) // 회전 없이 밀 때	
	{

		m_iState = 0;
		m_iState = STATE_PUSH;
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		m_pTransformCom->Set_Speed(10.f);
	}

	else 
	{
		if (m_iState == STATE_PUSH )
		{
			m_iState = 0; 
			m_iState = STATE_IDLE;
		}
	}

	if (GetKeyState('F') & 0x8000 && GetKeyState('S') & 0x8000) // 회전 없이 밀 때 
	{
		m_iState = 0;
		m_iState = STATE_PULL;
		//if(m_pGameInstance->isKeyEnter(DIK_S))
		//	m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,1.f }, 180.f);
		m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, m_pNavigationCom);
		m_pTransformCom->Set_Speed(10.f);
	}

	else
	{
		if (m_iState == STATE_PULL)		
		{
			m_iState = 0;	
			m_iState = STATE_IDLE;	
		}
	}	

	if (GetKeyState('G') & 0x8000  && GetKeyState('W') & 0x8000) // 이건 회전하면서 밀때		
	{

		m_iState = 0;	
		m_iState = STATE_PUSH;	
		//m_pTransformCom->Turn(_fvector{ 0.f,1.f,0.f,0.f },fTimeDelta);
		//m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, fTimeDelta);
		//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		m_pTransformCom->Set_Speed(10.f);		

	}
		
#pragma endregion		
		
#pragma region 지면상의 WASD 이동 

	if (!(m_iState & STATE_JUMP) && !(m_iState & STATE_JUMP2) &&!(m_iState & STATE_ROLL) && !(m_iState & STATE_HURT) && !(m_iState & STATE_PUSH) && !(m_iState & STATE_PULL))//*&& !(m_iState & STATE_JUMPLAND)*/)
	{
		if ((GetKeyState('W') & 0x8000))
		{
			_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

			// vLook과 캐릭터의 look 의 내적구하기 
			_vector PlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			//내적의 각도 구하기




			if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
				m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			else
			{
				if (m_pGameInstance->isKeyEnter(DIK_A))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네

				if (GetKeyState('A') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);

				if (m_pGameInstance->isKeyEnter(DIK_D))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));

				if (GetKeyState('D') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
			}

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

			if (m_pGameInstance->isKeyPressed(DIK_LCONTROL) && m_fStamina > 0.f)
			{
				m_pTransformCom->Set_Speed(80.f);
				m_iState = STATE_RUN;

				if(m_fStamina >1.f)
					m_fStamina -= fTimeDelta * 10.f;		
			}
			else
			{
				m_pTransformCom->Set_Speed(45.f);
				m_iState = STATE_WALK;
			}

			/* 사운드 관련 */
			
		}

		if ((GetKeyState('S') & 0x8000))
		{
			_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

			// vLook과 캐릭터의 look 의 내적구하기 
			_vector PlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			//내적의 각도 구하기

			if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
				m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);

			else
			{
				if (m_pGameInstance->isKeyEnter(DIK_A))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네

				if (GetKeyState('A') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);

				if (m_pGameInstance->isKeyEnter(DIK_D))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));

				if (GetKeyState('D') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
			}

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

			if (m_pGameInstance->isKeyPressed(DIK_LCONTROL) && m_fStamina > 0.f)
			{
				m_pTransformCom->Set_Speed(80.f);	
				m_iState = STATE_RUN;
				m_fStamina -= fTimeDelta * 10.f;	
			}
			else
			{	
				m_pTransformCom->Set_Speed(45.f);	
				m_iState = STATE_WALK;	
			}

			
		}



		if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000) && GetKeyState('A') & 0x8000)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	
			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, vLook));	
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamLeft, vLook);	
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), -Radian);

			m_iState = STATE_WALK;

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

			
		}

		if (m_pGameInstance->isKeyPressed(DIK_A) && m_pGameInstance->isKeyEnter(DIK_W))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, vLook));	
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamLeft, vLook);	
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네	

			m_iState = STATE_WALK;
			
		}

		if (m_pGameInstance->isKeyPressed(DIK_A) && m_pGameInstance->isKeyEnter(DIK_S) && (!(GetKeyState('D') & 0x8000)))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			

			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, vLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamLeft, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네	

			m_iState = STATE_WALK;
			
		}



		if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000) && GetKeyState('D') & 0x8000)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			

			/* 음과 양수를 판별*/


			float dotResult = XMVectorGetX(XMVector3Dot(CamRight, vLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(vLook, CamRight);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);	

			m_iState = STATE_WALK;

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);	
			
		}

		if (m_pGameInstance->isKeyPressed(DIK_D) && m_pGameInstance->isKeyEnter(DIK_W))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			


			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(vLook, CamRight));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamRight, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네	

			m_iState = STATE_WALK;
			
		}


		if (m_pGameInstance->isKeyPressed(DIK_D) && m_pGameInstance->isKeyEnter(DIK_S))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			


			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(vLook, CamRight));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamRight, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네	

			m_iState = STATE_WALK;
			
		}

		if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('A') & 0x8000) && !(GetKeyState('S') & 0x8000) && (!(GetKeyState('D') & 0x8000) &&
			(m_iState & STATE_WALK || m_iState & STATE_RUN)))
		{
			m_iState ^= STATE_WALK;
			m_iState |= STATE_IDLE;
		}

		if (GetKeyState(VK_LSHIFT) & 0x8000)
		{

			m_iState = 0;
			m_iState = STATE_ROLL;

		}

	}


	//else if (m_iState == STATE_ROLL && )
	//{
	//	m_iState ^= STATE_ROLL;		
	//	m_iState |= STATE_IDLE;
	//}

#pragma endregion 지면 상 WASD 이동 

#pragma region 점프 중 WASD 이동 
	else if (m_iState & STATE_JUMP || m_iState & STATE_JUMP2)
	{

 
		if (GetKeyState('W') & 0x8000)
		{
			_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

			// vLook과 캐릭터의 look 의 내적구하기 
			_vector PlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}


			if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
				m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);		

			else
			{
				if (m_pGameInstance->isKeyEnter(DIK_A))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네

				if (GetKeyState('A') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);

				if (m_pGameInstance->isKeyEnter(DIK_D))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));

				if (GetKeyState('D') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
			}

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}


		if ((GetKeyState('S') & 0x8000))
		{
			_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

			// vLook과 캐릭터의 look 의 내적구하기 
			_vector PlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(PlayerLook, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			//내적의 각도 구하기

			if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
				m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);

			else
			{
				if (m_pGameInstance->isKeyEnter(DIK_A))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네

				if (GetKeyState('A') & 0x8000)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);

				if (m_pGameInstance->isKeyEnter(DIK_D))
					m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));

				if (GetKeyState('D') & 0x8000 )
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
			}

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}



		if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000) && GetKeyState('A') & 0x8000)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, vLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamLeft, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}
			
			if(m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
				m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), -Radian);

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}

		if (m_pGameInstance->isKeyPressed(DIK_A) && m_pGameInstance->isKeyEnter(DIK_W))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, vLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamLeft, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			if (m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
				m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));
		}

		if (m_pGameInstance->isKeyPressed(DIK_A) && m_pGameInstance->isKeyEnter(DIK_S) && (!(GetKeyState('D') & 0x8000)))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, vLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamLeft, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));
		}


		if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000) && GetKeyState('D') & 0x8000)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			float dotResult = XMVectorGetX(XMVector3Dot(CamRight, vLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector3Cross(vLook, CamRight);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}

		if (m_pGameInstance->isKeyPressed(DIK_D) && m_pGameInstance->isKeyEnter(DIK_W))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(vLook, CamRight));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamRight, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));
		}


		if (m_pGameInstance->isKeyPressed(DIK_D) && m_pGameInstance->isKeyEnter(DIK_S))
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));



			// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
			float dotResult = XMVectorGetX(XMVector3Dot(vLook, CamRight));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);


			_vector crossResult = XMVector3Cross(CamRight, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}

			m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네	

		}
	}

#pragma endregion 점프 중 WASD 이동 

	if (m_pGameInstance->isKeyEnter(DIK_SPACE))
	{

		m_fJumpTime = 0.f;

		if (m_iState != STATE_JUMP)
		{
			m_fJumpPower = { 2.5f };
			m_iState = STATE_JUMP;
		}

		else if (m_iState & STATE_JUMP)
		{
			m_fJumpPower = { 2.5f };
			m_iState = STATE_JUMP2;
		}
	}

#pragma endregion

	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector test = { 0.f,0.f,0.f,1.f };
	/* 루트 모션 애니메션 코드 */
	m_pRootMatrix = m_pModel->Get_RootMotionMatrix("kaku");	

	if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModel->Get_LerpFinished())
	{
		if (m_pNavigationCom->isMove(vCurPosition) && m_iState != STATE::STATE_FALLING)
			m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);	
	}

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);	

#pragma region Falling 
	if(m_iState & STATE_FALLING)
	{

		if (m_pNavigationCom->Compute_Height(vPosition) >= vPosition.m128_f32[1])	
			m_iState = STATE_IDLE;	

		else
		{
			vPosition.m128_f32[1] -= fTimeDelta * 50.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}



#pragma region Jump 이동량 
	if (m_iState & STATE_JUMP || m_iState & STATE_JUMP2)
	{
		_float height = (m_fJumpTime * m_fJumpTime * (-1.f * m_fGravity) / 2) + (m_fJumpTime * m_fJumpPower);	

		m_bGround = true;

		if (height >= 0)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, vPosition.m128_f32[1] + height));
			m_fJumpTime += fTimeDelta;	
		}

		else
		{
			if (m_pNavigationCom->Compute_Height(vPosition) >= vPosition.m128_f32[1])
				m_iState = STATE_IDLE;			
	
			else 
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, vPosition.m128_f32[1] + height));
				m_fJumpTime += fTimeDelta;
			}
		
		}

	}
	
	else if (m_iState != STATE::STATE_FALLING && m_bGround)		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));
	
#pragma endregion
	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));


	if ((m_iState & STATE_WALK || m_iState & STATE_IDLE) && m_fStamina < 100.f)
		m_fStamina += fTimeDelta * 2.f;

 	__super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	/* 잠시 네비게이션 정확히 타는지 확인할려고 넣는 코드 */
#ifdef _DEBUG
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
	m_fTimeDelta = fTimeDelta;

	if (m_pNavigationCom->Get_CurCellIndex() == 215)
		m_bMonsterActive = true;	
	
	__super::Late_Update(fTimeDelta);	


	if (m_iState == STATE_WALK)	
	{
		m_fFootStepTime += fTimeDelta; 
		// 원래 지면하고 발하고의 Collision 을 넣어야하네	
		if (!m_pGameInstance->CheckSoundPlaying(SOUND_SURROUNDING))
		{
			if (m_fFootStepTime > 0.3f)
			{
				if (m_bFootStep1Foot_1_time)
				{
					m_pGameInstance->Play_Sound(L"foot1.ogg", SOUND_SURROUNDING, 0.02f);
					m_bFootStep1Foot_1_time = false; 
				}

				else
				{
					m_pGameInstance->Play_Sound(L"foot2.ogg", SOUND_SURROUNDING, 0.02f);
					m_bFootStep1Foot_1_time = true;
				}
				
				m_fFootStepTime = 0.f; 
			}
		}
	}


	else if (m_iState == STATE_RUN)
	{
		m_fFootStepTime += fTimeDelta;
		// 원래 지면하고 발하고의 Collision 을 넣어야하네	
		if (!m_pGameInstance->CheckSoundPlaying(SOUND_SURROUNDING))
		{
			if (m_fFootStepTime > 0.15f)
			{
				if (m_bFootStep1Foot_1_time)
				{
					m_pGameInstance->Play_Sound(L"sand_left_run_6.ogg", SOUND_SURROUNDING, 0.02f);
					m_bFootStep1Foot_1_time = false;
				}

				else
				{
					m_pGameInstance->Play_Sound(L"sand_right_run_6.ogg", SOUND_SURROUNDING, 0.02f);
					m_bFootStep1Foot_1_time = true;
				}

				m_fFootStepTime = 0.f;
			}
		}
	}


	if(m_iState == STATE_JUMP &&  m_iPreState  != STATE_JUMP)
		m_pGameInstance->Play_Sound(L"Kaku_Atk_LL_Jump.ogg", SOUND_SURROUNDING, 0.1f);

	if (m_iState == STATE_JUMP2 && m_iPreState != STATE_JUMP2)
		m_pGameInstance->Play_Sound(L"H_Kaku_Atk_ChargeX_Ready_Jump2.ogg", SOUND_SURROUNDING, 0.1f);

	if (m_iState == STATE_ROLL && m_iPreState != STATE_ROLL)
		m_pGameInstance->Play_Sound(L"H_Kaku_Atk_Flewit_Roll.ogg", SOUND_SURROUNDING, 0.1f);


	/* 이전 상태 저장하기 */
	m_iPreState = m_iState;	

}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	//m_pNavigationCom->Render();	
	//m_pColliderCom->Render();
#endif 

	return S_OK;	
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC   Desc{};

	/* 초기 디버깅 플레이어가 서있는 셀의 인덱스 */
	//Desc.iCurrentCellIndex = 0;			


	/* 스테이지3 디버깅용 플레이어가 서있는 셀의 인덱스 */
	//Desc.iCurrentCellIndex = 175;

	/* 스테이지4 디버깅용 플레이어가 서있는 셀의 인덱스 */
	//Desc.iCurrentCellIndex = 215;
	
	/* 스테이지5 디버깅용 플레이어가 서있는 셀의 인덱스 */
	//Desc.iCurrentCellIndex = 238;

	/* 스테이지6 디버깅용 플레이어가 서있는 셀의 인덱스 */
	//Desc.iCurrentCellIndex = 262;ㅇ

	/* 보스 디버깅용 플레이어가 서있는 셀의 인덱스 */
	Desc.iCurrentCellIndex = 295;		

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),	
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom),&Desc)))	
		return E_FAIL;


	/* Com_Collider */
	/*CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};

	Desc.iCurrentCellIndex = 0;

	ColliderDesc.vExtents = _float3(180.f, 600.f, 180.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);	


	if(FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;*/
		

	/* Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};	


	SphereDesc.fRadius = 125.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius+60.f, 0.f);	


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),	
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;	

	
	m_pColliderCom->Set_Collider_Name("Player");		
	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::PLAYER, this);	


	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	//BodyDesc.fSpeedPerSec = 1.f;
	BodyDesc.fRotationPerSec = 0.f; 

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;	

	m_pModel = dynamic_cast<CModel*>(Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));	

	/*CModel*	pBodyModelCom = dynamic_cast<CModel*>
		(m_PartObjects[TEXT("Part_Body")]->Find_Component(TEXT("Com_Model")));*/
	/* 위의 코드는 만약 Part_Body가 없으면 맵컨테이너에서 추가해서 집어넣기 때문에 디버깅이 안됨 따라서 안좋음 */

		/* 무기를 만든다ㅏ. */
	CWeapon::WEAPON_DESC		WeaponDesc{};

	CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));
	if (nullptr == pBodyModelCom)
		return E_FAIL;

	WeaponDesc.pParentState = &m_iState;	
	WeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("DM_R_Hand"); /* 캐릭터 모델마다 다름 */
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	WeaponDesc.fSpeedPerSec = 0.f;
	WeaponDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"), &WeaponDesc)))
		return E_FAIL;

	///* 이펙트를 추가하나다. */
	//CHitParticle::PARTICLE_HITPARTICLE_DESC EffectDesc{};	
	//
	//EffectDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();			
	//
	//if (FAILED(__super::Add_PartObject(TEXT("Part_Effect"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Hit"), &EffectDesc)))	
	//	return E_FAIL;
	/* 유아이르르 추가하한다. */
	return S_OK;
}


void CPlayer::OnCollisionEnter(CGameObject* _pOther)
{
	if (!strcmp(_pOther->Get_Name(), "M_Guardian_Model_C_ARM") && m_iState != STATE_ROLL)
	{
		m_iHp--;
		m_iState = 0;	
		m_iState = STATE_HURT;			
	}

	if (!strcmp(_pOther->Get_Name(), "M_Guardian_Model_D_ARM") && m_iState != STATE_ROLL )
	{
		m_iHp--;	
		m_iState = 0;	
		m_iState = STATE_HURT;	
	}

	

	if (!strcmp(_pOther->Get_Name(), "Boss_Left_Arm") && m_iState != STATE_ROLL)		
	{
		m_iHp--;	
		m_iState = 0;	
		m_iState = STATE_HURT;	
	}

	if (!strcmp(_pOther->Get_Name(), "Boss_Right_Arm") && m_iState != STATE_ROLL)
	{
		m_iHp--;
		m_iState = 0;
		m_iState = STATE_HURT;
	}

	if(!strcmp(_pOther->Get_Name(), "LIFT_TABLE"))
	{
		m_iState = 0;
		m_iState = STATE_FALLING; 
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet)
	}
	// 이 때 이제 캐릭터 모션을 추가해줘야할거같음 피격당할시 


#pragma region Water_Step
	if (!strcmp(_pOther->Get_Name(), "Water_Step_01"))		
	{
		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);	

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];	
	

		//if (vCurPos.m128_f32[1] < fHeight)	
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight+0.5f));	


	}

	if (!strcmp(_pOther->Get_Name(), "Water_Step_02"))
	{
		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	}

	if (!strcmp(_pOther->Get_Name(), "Water_Step_03"))
	{
		
		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	}

	if (!strcmp(_pOther->Get_Name(), "Water_Step_04"))
	{

		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	}

#pragma endregion


#pragma region  MovingFloor	

	//if (!strcmp(_pOther->Get_Name(), "Moving_Floor_01"))		
	//{

	//	m_iState = 0;	
	//	m_iState = STATE_IDLE;	
	//	m_bGround = false;
	//	_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//	_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


	//	//if (vCurPos.m128_f32[1] < fHeight)
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	//}


#pragma endregion 
}


void CPlayer::OnCollision(CGameObject* _pOther)
{
	if (!strcmp(_pOther->Get_Name(), "M_Guardian_Model_C_ARM") && m_iState != STATE_ROLL)
	{
	
		m_iState = 0;
		m_iState = STATE_HURT;
	}

	if (!strcmp(_pOther->Get_Name(), "M_Guardian_Model_D_ARM") && m_iState != STATE_ROLL)
	{
	
		m_iState = 0;
		m_iState = STATE_HURT;
	}
#pragma region Water_Step
	if (!strcmp(_pOther->Get_Name(), "Water_Step_01"))
	{
		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	
	}

	if (!strcmp(_pOther->Get_Name(), "Water_Step_02"))
	{
		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	}

	if (!strcmp(_pOther->Get_Name(), "Water_Step_03"))
	{

		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	}

	if (!strcmp(_pOther->Get_Name(), "Water_Step_04"))
	{

		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 0.5f));

	}

#pragma endregion 


#pragma region Moving Floor		
	if (!strcmp(_pOther->Get_Name(), "Moving_Floor_01"))	
	{

		m_iState = 0;
		m_iState = STATE_IDLE;
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight + 11.f));

	}
#pragma endregion 


#pragma region SC_StepBox_01		
	if (!strcmp(_pOther->Get_Name(), "SC_StepBox_01"))
	{

		m_iState = 0;
		m_iState = STATE_IDLE;	
		m_bGround = false;
		_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float  fHeight = _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1];


		//if (vCurPos.m128_f32[1] < fHeight)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, fHeight+3.f));
	

	}
#pragma endregion 

}

void CPlayer::OnCollisionExit(CGameObject* _pOther)
{

#pragma region  MovingFloor	

	if (!strcmp(_pOther->Get_Name(), "Moving_Floor_01"))
		m_bGround = true;	



#pragma endregion 
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);	

	if (FAILED(pInstance->Initialize_Prototype()))	
	{
		MSG_BOX("Failed To Created : CPlayer");	
		Safe_Release(pInstance);	
	}

	return pInstance;	
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);	

	if (FAILED(pInstance->Initialize(pArg)))	
	{
		MSG_BOX("Failed To Cloned : CPlayer");	
		Safe_Release(pInstance);	
	}

	return pInstance;	
}

void CPlayer::Free()
{
	__super::Free();	

	Safe_Release(m_pColliderCom);	
	Safe_Release(m_pNavigationCom);

}
