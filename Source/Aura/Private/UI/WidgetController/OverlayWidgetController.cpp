


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	/**
  	 * CastChecked: 더 효율적인 캐스트 방식. 단, 안전성이 떨어지므로 Cast가 성공하는것으로 확신하면 사용 (브로드캐스트 초기값을 호출하면 절대 NULL값이 나오지 않음)
	 *   == ( ptrA = Cast< type >( ptrB ); check( ptrA != nullptr ); )
	 */
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

// 콜백 바인딩 함수, Health와 MaxHealth와 같은 속성이 변경될 때마다 브로드캐스트 -> 콜백
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	/** 게임플레이 속성 값 변경 델리게이트
	 * GetHealthAttribute에 대한 델리게이트를 반환하면 .AddUObject()를 사용해 바인딩
	 * - 바인딩할 오브젝트 (this)
	 * - 호출할 함수 주소 (HealthChanged)
	 */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		//AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
		/* 델리게이트 콜백 대신 람다 함수 사용 => 모든 속성에 대해 동일한 델리게이트를 사용하지 못하므로 콜백 대신 내부에서 바로 바인딩하는 것이 더 효율적임 + 간단함 */
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	/** 콜백 바인딩 함수에 ASC에서 브로드캐스트한 EffectAssetTags를 가져와 람다함수 실행
	 * - 단, 클래스 내부의 멤버함수를 호출하려면 추가 작업이 필요함
	 *   - 람다함수는 익명 함수이므로 클래스에 대해 모르기 때문! 멤버함수를 불러올 수 없다.
	 *     GetDataTableRowByTag<>() 와 같이 멤버함수를 호출하고 싶다면 대괄호를 활용하자.
	 *     
	 *   - []: introducer
	 *     - 대괄호 내부에 외부 변수를 써놓으면 이를 캡처해 해당 변수를 람다 내부에서 사용할 수 있도록 함.
	 *     - eg) GetDataTableRowByTag는 람다함수의 외부이나 OverlayWidgetController 클래스의 내부 함수이므로, this 키워드를 사용해 캡처할 수 있다.
	 */
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			/** 왜 const &? 태그를 복사하지 않도록 하기 위함, 일종의 싱글톤. 참조& 를 사용하려면 const참조가 되어야 함 */
			for(const FGameplayTag& Tag : AssetTags)
			{
				// const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				// GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
				
				/** "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False */
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if(Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}

/* 람다 함수로 브로드캐스트하기 이전에 구현한 콜백함수
void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
*/