


#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	/**
	 * 이름 지정에 TEXT() 매크로를 사용하지 않는 이유: FName을 사용하기 때문임!!
	 * - FName은 FString을 사용하는데, 일반 문자열 리터럴을 작성해도 암시적으로 변환해줌
	 *
	 * 일반적으로 무기에는 Collision을 사용하지 않는다.
	 */
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
