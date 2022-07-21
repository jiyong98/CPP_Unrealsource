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

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void AttackCheck();

	
	void Inventory();


	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	
	//��������Ʈ������ �´��Լ����·θ����
	UFUNCTION()
		//��Ÿ�ְ� �������� ������  ���Լ��� ȣ���
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	//�κ��丮
	//����
	//�������Ѱ�
	void Inventory_Input();
	///////
private:
	UPROPERTY(VisibleAnywhere);
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere);
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Pawn);
	bool IsAttacking = false;


	// �⺻�Ҹ���, int�̷��Ծƴ϶� ���۳�Ʈ�� �ۿ��������Ǵ� �ֵ���
	// UPROPERTY();����� �˾Ƽ� �޸𸮰�������
	UPROPERTY();
	//���� ����ҰŸ� �̷��� ���漱���ؼ� �ɹ������θ����
	class UMyAnimInstance* AnimInstance;

	//UPROPERTY();
	//class UMyItem* pItem;

	UPROPERTY();
	int32 Attackindex = 0;
public:
	UPROPERTY();
	float UpDownValue = 0.f;

	UPROPERTY();
	float LeftRightValue = 0.f;

	UPROPERTY(VisibleAnywhere);
	UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere);
	class UMyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere);
	class UWidgetComponent* HpBar;

	UPROPERTY(VisibleAnywhere);
	class UWidgetComponent* InventoryWidget;

	//UPROPERTY(VisibleAnywhere);
	//UStaticMeshComponent* Item;


	UPROPERTY();
	bool IsItemColl = false;

	UPROPERTY();
	bool IsWidgetShowing = false;

	//UPROPERTY();
	//bool IsDeleteItem = false;


	



	



};
