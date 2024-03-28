


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	/**
	* Minimal - 게임 플레이효과가 복제되지 않음, 게임에 영향을 주는 신호와 태그만 모든 클라이언트에 복제됨 (Use Mixed instead). [멀티플레이어, AI컨트롤]
	* Mixed - 게임 플레이효과가 소유 중인 클라이언트에만 복제됨, 게임 플레이 신호와 태그는 모든 클라이언트에 복제됨 [멀티플레이어, 플레이어컨트롤]
	* Full - 게임 플레이 정보를 모두 복제함 [싱글플레이어]
	*
	* => 복제모드 설정 방법: UAbilitySystemComponent::SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	*/
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	/**
	 * NetUpdateFrequency: 서버가 클라이언트 업데이트를 시도하는 빈도
	 * - 플레이어 상태 변경이 발생하면 업데이트를 외부로 보냄 => 클라이언트와 서버 버전 동기화
	 * - 일반적인 업데이트 빈도는 0.5 정도이며, Lyra 등 프로젝트는 더 빠른 동기화를 위해 100으로 설정함
	 */
	NetUpdateFrequency = 100.f;
	
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
