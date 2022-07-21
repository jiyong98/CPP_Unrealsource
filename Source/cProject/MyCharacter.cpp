// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyItem.h"
#include "MyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"

// Sets default values

AMyCharacter::AMyCharacter()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	//�˸��� ��ġ�� ����������
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	/*
	FName WeaponSocket(TEXT("hand_lSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));
		if (SW.Succeeded())
		{
			Weapon->SetStaticMesh(SW.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	*/
	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));
	
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	
	InventoryWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("INVENTORYWIDGET"));
	InventoryWidget->SetupAttachment(GetMesh());
	InventoryWidget->SetRelativeLocation(FVector(-300.f, 0.f, 0.f));
	InventoryWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UW2(TEXT("WidgetBlueprint'/Game/UI/WBP_Slot.WBP_Slot_C'"));
	//�κ��丮
	if (UW2.Succeeded())
	{
		InventoryWidget->SetWidgetClass(UW2.Class);
		InventoryWidget->SetDrawSize(FVector2D(600.f, 600.f));
	}
	

	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	FName WeaponSocket(TEXT("hand_lSocket"));
	//���峻�� ����뿡 ��ġ
	auto CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector(200.f, 200.f, 0.f), FRotator::ZeroRotator);
	//auto CurrentItem = GetWorld()->SpawnActor<AMyItem>(FVector(0.f, 0.f, 0.f ), FRotator::ZeroRotator);


	//������ ���� �޼ռ��Ͽ��ٿ��ֱ�
	if (CurrentWeapon)
	{
		
		//CurrentWeapon->AttachToComponent(GetMesh(),
		//	FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		//	WeaponSocket
		//);
		
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//�� �ؿ���(�����Լ�)ó�� �����ϱ� �ȴ�
	//��������Ʈ��� �𸮾󿡼������� ��ü����
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance) //�ִ��ν��Ͻ��� �Ⱥ����� ��ũ���ð� ��������
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		//��Ÿ�ְ� �������� ȣ��Ǵ°��� ��OnAttackMontageEnded�Լ����ض�
		//���� - �������Ӹ��� �������ŷ�� ���������ƴ��� üũ���ʿ����
		// �˾Ƽ� �ִϸ��̼��� ������ �̸�Ÿ�ְ� �����ԵǸ� ���Լ��� ȣ���

		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
		//��ε�ĳ��Ʈ�Ҷ� ����üũ�� �Ҹ� 
	}

	HpBar->InitWidget();
	InventoryWidget->InitWidget();

	//�ʱ�ȭ�� ���ε����ֱ�
	auto HpWidget = Cast<UMyCharacterWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
		HpWidget->BindHp(Stat);

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�׼�
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	
	//E�Է½� �κ��丮�� ��
	PlayerInputComponent->BindAction(TEXT("ItemInventory"), EInputEvent::IE_Pressed, this, &AMyCharacter::Inventory);
	//I�Է½� �κ��丮�� ��µ� -�ذ����
	PlayerInputComponent->BindAction(TEXT("InventoryInput"), EInputEvent::IE_Pressed, this, &AMyCharacter::Inventory_Input);

	//Axis(��) - ���̽�ƽ
	//Action(�׼�) - ��ư
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
	//���Ŀ� ������Ʈ ���ÿ��� ����-�Է�-> �� ���ο� UpDown,LeftRight �����
	//������Ʈ���� ������ ���� �ؿ� Value�� �Ѿ��

}

void AMyCharacter::Attack()
{
	//1�������� ȿ�����̰� üũ
	if (IsAttacking)
		return;
	//�ִϸ��̼ǿ����õȰ� ���̷���޽��� �׻�پ�����
	/*
	auto AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
	}
	*/
	//�ִ��ν��Ͻ��� ��������� �߱⶧���� ��ó�����ʿ����
	//�ؿ�ó�� ����ȭ
	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToSection(Attackindex);

	Attackindex = (Attackindex + 1) % 3;

	IsAttacking = true;

	//��������Ʈ - 
	//��������Ʈ�� ��������� ���ɻ��������ִ�
	// ��������Ʈ - �ݹ�, ����Ƽ����
	//���� - ���ʿ��ϰ� PlayAttackMontage��ȣ���ؼ�
	//���Լ��ȿ� ����ó���� �����ʿ���⶧����
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2, //�̰� ������ �ִ���ġ
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	//�ǰ������� HitResult�ι޾ƿ�
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		Rotation,DrawColor, false, 2.f
	);
	if (bResult && HitResult.Actor.IsValid()) //�浹������
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
	
}

void AMyCharacter::Inventory()
{
	IsDeleteItem = true;
	if (IsItemColl)
	{
		UE_LOG(LogTemp, Log, TEXT("Inventory Item Input"));
	
	}

}





void AMyCharacter::UpDown(float Value)
{
	
	UpDownValue = Value; //����̿��ؼ� �ǳ��ٰ���
	//UE_LOG(LogTemp, Warning, TEXT("UpDown %f"), Value);
	AddMovementInput(GetActorForwardVector(), Value);

}

void AMyCharacter::LeftRight(float Value)
{
	

	LeftRightValue = Value;//����̿��ؼ� �ǳ��ٰ���
	//UE_LOG(LogTemp, Warning, TEXT("LeftRight %f"), Value);
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::Yaw(float Value)
{
	
	AddControllerYawInput(Value);
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}


void AMyCharacter::Inventory_Input()
{
	IsWidgetShowing = true;
	UE_LOG(LogTemp, Log, TEXT("Inventory Open"));
}

