


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/**
	 * Sphere와 겹치는 액터에 접근해 해당 액터가 GAS를 포함하고 있는지 확인
	 * - GetAbilitySystemComponent 함수를 활용해 IAbilitySystemInterface를 어떤 액터가 구현하는지 확인할 수 있다! => 해당 액터에 속한 능력 시스템 구성 요소를 얻음
	 *   - 속성 클래스를 갖고 있는 액터가 어떤 유형인지 신경쓸 필요 x
	 */
	if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		/** Interface 클래스가 NULL이 아니므로 AuraAttributeSet에 설정된 속성을 찾을 수 있다.
		 * 1. ASCInterface의 GetAbilitySystemComponent포인터로 접근
		 * 2. GetAbilitySystemComponent에서 GetAttributeSet 함수 호출
		 *    - GetAttributeSet는 파라미터로 AttributeSet 클래스의 하위 클래스를 받음
		 * 3. AuraAttributeSet을 반환하므로 캐스팅 필요
		 * 4. 접근한 클래스를 변수처럼 다루기 위해 포인터 변수 지정
		 * 5. 반환값이 StaticClass이므로 const 붙임
		 */
		//TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		
		// const 포인터를 Set하므로 오류가 발생 => 직접 상태를 설정할 수 X. 학습을 위해 const를 일반 변수로 형변환
		UAuraAttributeSet* MutableAuraAttributeSet= const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
		Destroy();
	}
	
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}
