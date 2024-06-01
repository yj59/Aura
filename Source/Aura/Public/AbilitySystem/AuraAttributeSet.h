

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * @brief 매크로 기능을 통해 초기화 함수를 만들지 않아도 일련의 기능 사용
 * @param 해당 속성에 대한 클래스명
 * @param 속성명
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}
	
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;
	
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;
	
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
	
};

/**
 * [4강] boilerplate: 서버에 알릴 속성을 추가하는 과정
 * 1. 변수 선언
 * 2. 선언한 변수 복제
 * 3. 담당자 함수 생성(매개변수: 이전 값)
 * 4. 담당자 정의
 * 5. GAS 담당자 알림 매크로 추가
 * 6. 매크로에 복제 변수 추가 => 조건 없음 및 담당자 알림
 * 일반적으로 GAS에서 속성이 필요한 모든 작업은 위 과정을 따른다!
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	// 담당자 알림 매크로
	/** 복제를 위한 변수 등록
	 * - 복제를 위해 Life Time 속성을 보유하는 배열을 가져와 재정의해야함
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	/** 속성이 변경되기 전 호출 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/** 속성이 변경된 이후 호출 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	// 담당자 함수 생성
	/** 새 값과 이전 값 비교 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	
public:
	// 복제할 변수 선언
	/** 다른 모든 클라이언트가 해당 변경 사항을 알아야 하므로 복제 지정자(Replicated) 사용 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;

	// 속성 접근자 매크로
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

};
