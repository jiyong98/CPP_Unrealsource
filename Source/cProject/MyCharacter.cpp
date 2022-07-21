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

	//알맞은 위치로 강제조정함
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
	//인벤토리
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
	//월드내에 가운대에 배치
	auto CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector(200.f, 200.f, 0.f), FRotator::ZeroRotator);
	//auto CurrentItem = GetWorld()->SpawnActor<AMyItem>(FVector(0.f, 0.f, 0.f ), FRotator::ZeroRotator);


	//스폰한 액터 왼손소켓에붙여주기
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
	//난 밑에서(어택함수)처럼 낭비하기 싫다
	//델레게이트사용 언리얼에서구현한 자체문법
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance) //애님인스턴스가 안붙으면 널크래시가 날수있음
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		//몽타주가 끝났을때 호출되는것을 위OnAttackMontageEnded함수로해라
		//장점 - 매프래임마다 이즈어태킹이 끝났는지아닌지 체크할필요없이
		// 알아서 애니메이션이 끝나서 이몽타주가 끝나게되면 위함수가 호출됨

		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
		//브로드캐스트할때 어택체크가 불림 
	}

	HpBar->InitWidget();
	InventoryWidget->InitWidget();

	//초기화후 바인딩해주기
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

	//액션
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	
	//E입력시 인벤토리에 들어감
	PlayerInputComponent->BindAction(TEXT("ItemInventory"), EInputEvent::IE_Pressed, this, &AMyCharacter::Inventory);
	//I입력시 인벤토리가 출력됨 -해결못함
	PlayerInputComponent->BindAction(TEXT("InventoryInput"), EInputEvent::IE_Pressed, this, &AMyCharacter::Inventory_Input);

	//Axis(축) - 조이스틱
	//Action(액션) - 버튼
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
	//이후에 프로젝트 세팅에서 엔진-입력-> 축 매핑에 UpDown,LeftRight 만들기
	//프로젝트에서 세팅한 값이 밑에 Value로 넘어옴

}

void AMyCharacter::Attack()
{
	//1차적으로 효율적이게 체크
	if (IsAttacking)
		return;
	//애니메이션에관련된건 스켈레톤메쉬에 항상붙어있음
	/*
	auto AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
	}
	*/
	//애님인스턴스를 멤버변수로 했기때문에 위처럼할필요없이
	//밑에처럼 최적화
	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToSection(Attackindex);

	Attackindex = (Attackindex + 1) % 3;

	IsAttacking = true;

	//델레게이트 - 
	//델레게이트로 만들었으면 성능상이점이있다
	// 델레게이트 - 콜백, 유니티문법
	//이유 - 불필요하게 PlayAttackMontage를호출해서
	//그함수안에 예외처리를 또할필요없기때문에
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
		ECollisionChannel::ECC_EngineTraceChannel2, //이게 어택이 있는위치
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	//피격판정을 HitResult로받아옴
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
	if (bResult && HitResult.Actor.IsValid()) //충돌했을때
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
	
	UpDownValue = Value; //얘넬이용해서 건내줄것임
	//UE_LOG(LogTemp, Warning, TEXT("UpDown %f"), Value);
	AddMovementInput(GetActorForwardVector(), Value);

}

void AMyCharacter::LeftRight(float Value)
{
	

	LeftRightValue = Value;//얘넬이용해서 건내줄것임
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

