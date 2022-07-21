// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "Engine/World.h"

// Sets default values
AMyItem::AMyItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM"));
	TriggerItem = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'"));
	if (SW.Succeeded())
	{
		Item->SetStaticMesh(SW.Object);
	}
	Item->SetupAttachment(RootComponent); 
	TriggerItem->SetupAttachment(Item);


	Item->SetCollisionProfileName(TEXT("MyItemCollectible"));
	TriggerItem->SetCollisionProfileName(TEXT("MyItemCollectible"));
	TriggerItem->SetBoxExtent(FVector(100.f, 100.f, 100.f));

}

void AMyItem::Delete()
{
	Destroy();
}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();
	

}



void AMyItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TriggerItem->OnComponentBeginOverlap.AddDynamic(this, &AMyItem::OnComponentBeginOverlap_Trigger);
	TriggerItem->OnComponentEndOverlap.AddDynamic(this, &AMyItem::OnComponentEndOverlap_Trigger);
	//TriggerItem->OnComponentBeginOverlap.AddDynamic(this, &AMyItem::OnCharacterOverlap);
}


void AMyItem::OnComponentBeginOverlap_Trigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	
	if (MyCharacter)
	{
		MyCharacter->IsItemColl = true;
		/*
		if (bCollCheck)
		{
			Destroy();
			UE_LOG(LogTemp, Log, TEXT("destroy"));
		}
		

		UE_LOG(LogTemp, Log, TEXT("test bool %d"), MyCharacter->IsDeleteItem);
		*/
		
	}
	
	UE_LOG(LogTemp, Log, TEXT("BeginOverlapped"));
	
}

void AMyItem::OnComponentEndOverlap_Trigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->IsItemColl = false;
	}
	UE_LOG(LogTemp, Log, TEXT("EndOverlapped"));

}



