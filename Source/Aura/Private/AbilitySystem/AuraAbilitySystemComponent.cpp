


#include "AbilitySystem/AuraAbilitySystemComponent.h"

// 해당 함수가 호출되면 AbilityActorInfo가 설정되었음을 알 수 있음
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/** OnGameplayEffectAppliedDelegateToSelf
	 *  - 동적델리게이트가 아니므로 동적 추가 불가능. 객체를 추가해야 함
	 */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveffectHandle)
{
	// GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));
	
	/** [ASC 정보를 HUD에 띄우는 방법]
	 * - ASC를 HUD에 띄우려면 종속성을 고려해야 함: AuraASC는 위젯 컨트롤러를 모르기 때문
	 * - 델리게이트가 무엇을 브로드캐스트해야 HUD가 알아들을까?
	 *   => 어떠한 효과에 게임플레이 태그가 포함된 경우 위젯 컨트롤러에 브로드캐스트할 수 있음
	 *
	 * - 효과를 가진 게임 플레이 태그를 어떻게 ASC가 얻을 수 있을까? => EffectSpec의 GetAllAssetTags로 해당 애셋에 대한 모든 태그를 불러옴 => 우리가 필요한 효과를 가진 태그를 가져올 수 있다!
	 *   - GetAllAssetTags은 태그를 배열에 저장하지 않고 FGameplayTagContainer에 저장함
	 *   
	 * - 위젯 컨트롤러에 있는 TagContainer의 모든 단일 태그에 대해 브로드캐스트
	 *   - 게임플레이 효과에 태그를 추가할 때 해당 태그를 가져와 브로드캐스트 => 화면에 메시지 효시
	 *   - Asset Tag를 브로드캐스트하는 델리게이트가 필요함
	 */
	
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	/** TagContainer 브로드캐스트 => EffectAssetTags를 바인딩하는 모든 클래스는 AssetTags가 들어 있는 TagContainer를 받게 됨 */
	EffectAssetTags.Broadcast(TagContainer);
	
	
}
