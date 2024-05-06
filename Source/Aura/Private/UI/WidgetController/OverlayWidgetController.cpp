


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(100.f);

	/**
  	 * CastChecked: 더 효율적인 캐스트 방식. 단, 안전성이 떨어지므로 Cast가 성공하는것으로 확신하면 사용 (브로드캐스트 초기값을 호출하면 절대 NULL값이 나오지 않음)
	 *   == ( ptrA = Cast< type >( ptrB ); check( ptrA != nullptr ); )
	 */
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}
