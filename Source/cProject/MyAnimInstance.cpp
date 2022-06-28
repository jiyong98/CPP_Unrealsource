// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Greystone_Skeleton_Montage.Greystone_Skeleton_Montage'"));
	//만약 이걸찾았다
	if (AM.Succeeded())
	{
		//어택몽타주를 셋팅
		AttackMontage = AM.Object;
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	//나의 폰주인님을 겟하겠다.
	//하지만 트라이가붙은건 백프로 있을거란보장이없음
	//예외처리하기
	if (IsValid(Pawn))
	{
		//원래속도에다가 실제속도의 크기받아오기
		//매프레임마다 속도가갱신됨
		Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);

		if (Character)
		{
			isFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	//플레이하기
	//중복되서 플래이하지않게 예외처리
	//if (!Montage_IsPlaying(AttackMontage))
	//{
		//Montage_Play(AttackMontage, 1.f);
	//}
	//델레게이트를 만들었기때문에 위처럼 할필요없음
	Montage_Play(AttackMontage, 1.f);
}
