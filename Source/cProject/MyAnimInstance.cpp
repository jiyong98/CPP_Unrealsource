// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Greystone_Skeleton_Montage.Greystone_Skeleton_Montage'"));
	//���� �̰�ã�Ҵ�
	if (AM.Succeeded())
	{
		//���ø�Ÿ�ָ� ����
		AttackMontage = AM.Object;
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	//���� �����δ��� ���ϰڴ�.
	//������ Ʈ���̰������� ������ �����Ŷ������̾���
	//����ó���ϱ�
	if (IsValid(Pawn))
	{
		//�����ӵ����ٰ� �����ӵ��� ũ��޾ƿ���
		//�������Ӹ��� �ӵ������ŵ�
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
	//�÷����ϱ�
	//�ߺ��Ǽ� �÷��������ʰ� ����ó��
	//if (!Montage_IsPlaying(AttackMontage))
	//{
		//Montage_Play(AttackMontage, 1.f);
	//}
	//��������Ʈ�� ������⶧���� ��ó�� ���ʿ����
	Montage_Play(AttackMontage, 1.f);
}
