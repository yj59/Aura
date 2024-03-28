

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 클래스 매크로에 Abstract를 지정해 인스턴스화 방지
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	/**
	 * @brief PlayerState 클래스에서 AbilitySystem을 상속받을 수 있도록 인터페이스 역할을 함 (Getter)
	 * @return AbilitySystemComponent
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
protected:
	virtual void BeginPlay() override;

	/**
	 * UPROPERTY() => 모든 멤버 포인터는 UPROPERTY로 만듦
	 * TObjectPtr: 포인터와 동일하게 동작 + 접근성과 관련된 기능 추가
	 * - Access traking: 포인터가 얼마나 자주 액세스되거나 참조 해제되는지 추적 가능
	 * - lazy load: 실제로 사용될 때까지 로드하지 않음
	 * - 별도의 포인터 표시(*) 없음!
	 */
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;


	/**
	 * AbilitySystemComponent, AttributeSet => AuraEnemy 클래스 생성자에서 초기화
	 * 단, 플레이어 캐릭터 클래스에선 초기화하고 싶지 않음 (PlayerState에서 초기화하고 싶을 경우)
	 *   - Enemy는 Owner Actor와 Avatar Actor가 같은 클래스에서 동작하므로, Enemy 클래스에서 ASC와 AS 포인터가 유효값을 가짐
	 *   - 플레이어 캐릭터는 Owner Actor와 Avatar Actor가 다른 클래스로 구분됨 (각각 PlayerState, Character 클래스)
	 *     - 플레이어 캐릭터 클래스에서 해당 포인터에 대한 유효값을 가지지 못해 오류 발생
	 *     => Owner와 Avatar를 설정하는 함수를 호출해 AbilitySystemComponent를 직접 초기화해야 함
	 *     (UAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor))
	 *     
	 * 언제 초기화 함수를 호출하는게 좋을까?
	 *   - 플레이어 컨트롤러가 Pawn에 설정된 이후 수행되어야 함
	 *     1) ASC가 Pawn 자체에 있는 경우(eg. Enemy) => 서버의 PossesedBy, 클라이언트 AcknowledgePossession 함수에 호출
	 *       - PossesedBy: Pawn이 빙의되면 서버에서 호출됨
	 *       - AcknowledgePossession: Pawn 빙의 후, 플레이어 컨트롤러(클라이언트)에서 로컬 Pawn에 대한 기본 설정을 위해 호출됨
	 *     2) Owner와 Avatar가 구분되어 있을 경우 => OnRep_PlayerState 함수에서 ASC 초기화
	 *       - ASC가 PlayerState에 있으므로 AcknowledgePossession를 사용하지 않음
	 *       - 컨트롤러 설정 및 현재 시점에서 PlayerState가 유효한지 확인해야 함
	 *         - 컨트롤러와 플레이어가 모두 유효할 경우, 플레이어 상태가 서버에 설정된 후 복제
	 *       - 플레이어 상태가 폰에 대해 설정되자마자 복제 -> 복제에 대한 응답으로 알림이 트리거됨(RepNotify)
	 *         - 해당 시점에 대한 함수: OnRep_PlayerState
	 *         - OnRep_PlayerState에서 OwnerActor를 PlayerState 클래스로, AvatarActor를 Character 클래스로 설정
	 *
	 *   - AI 캐릭터의 경우
	 *     - 초기 정보만 호출한 후 BeginPlay 함수 호출
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
