

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

/**
 * 클래스 매크로에 Abstract를 지정해 인스턴스화 방지
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief
	 * UPROPERTY() => 모든 멤버 포인터는 UPROPERTY로 만듦
	 * TObjectPtr: 포인터와 동일하게 동작 + 접근성과 관련된 기능 추가
	 * - Access traking: 포인터가 얼마나 자주 액세스되거나 참조 해제되는지 추적 가능
	 * - lazy load: 실제로 사용될 때까지 로드하지 않음
	 * - 별도의 포인터 표시(*) 없음!
	 */
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
};
