// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class CPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	
	//델리게이트문법에 맞는함수형태로만들기
	UFUNCTION()
		//몽타주가 끝나면이 끝나고  이함수가 호출됨
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(VisibleAnywhere);
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere);
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Pawn);
	bool IsAttacking = false;


	// 기본불리언, int이런게아니라 컴퍼넌트나 밖에서관리되는 애들은
	// UPROPERTY();해줘야 알아서 메모리관리가됨
	UPROPERTY();
	//많이 사용할거면 이렇게 전방선언해서 맴버변수로만들기
	class UMyAnimInstance* AnimInstance;



};
