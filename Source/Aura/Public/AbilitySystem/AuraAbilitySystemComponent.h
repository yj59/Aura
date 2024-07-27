

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */)

/**
 * [멀티플레이에서의 GAS 개요]
 * 
 *  - Dedicated Server
 *    - 플레이어(사람)가 존재하지 않음(게임 시뮬레이션을 실행하는 컴퓨터)
 *    - 실제로 화면 렌더링이 발생하지 않음
 *  - Listen Server
 *    - 플레이어가 캐릭터를 제어하는 게임 버전을 실행하는 컴퓨터
 *      - (플레이어가 게임을 호스팅함)
 *    - 호스트 플레이어는 지연이 발생하지 않음
 *      - 호스트가 네트워크를 통해 데이터를 보낼 필요가 없음 => 데이터가 이동하는 시간이 없다!
 *
 *  - 지연이 발생하면 컴퓨터마다 캐릭터 위치 버전이 달라짐
 *    => 어떤 기기가 올바른 버전인지 결정해야 한다!
 *    => 일반적으로 서버가 게임의 올바른 버전임 (Server is Authority)
 *
 *  - 서버와 클라이언트의 구분
 *    - 게임 모드는 서버에만 존재함 (플레이어 생성, 규칙과 같은 작업을 담당하기 때문)
 *    - 클라이언트 중 하나에서 게임 모드에 액세스하려고 하면 nullptr 표시됨
 *    - 플레이어 컨트롤러는 서버와 클라이언트에 모두 존재
 *      - 권한있는 서버 버전(Authoritative server version)과 클라이언트의 로컬 버전
 *      - 클라이언트0에는 클라이언트1의 컨트롤러가 없다! 오직 해당 플레이어의 컨트롤러만 있음 (서버는 모두 o)
 *        - cf) 단, 클라이언트는 다른 플레이어 상태 버전과 pawn들을 모두 가지고 있음 (멀티에서 다른 플레이어를 감지해야 하므로! 당연함)
 *        - cf) 자기자신의 상태 정보를 나타내는 hud도 해당 클라이언트에만 존재함
 *
 *  - 복제란? 데이터가 서버에서 클라이언트로 전송될 때 서버가 이를 관리할 수 있도록 하는 것
 *    - eg) 플레이어 컨트롤러 클래스가 복제되도록 설정 
 *      - 업데이트 빈도수: 100 => 서버가 변경 사항을 알 수 있도록 클라이언트에 데이터를 보냄(초당 100회)
 *    - Pawn 클래스에 클라이언트 폰 버전 변수, 서버 폰 버전 변수가 내장
 *      - 변수가 복제 변수로 지정되면 해당 변수의 값이 변경됨
 *      - 서버에서 변경 사항이 포함된 네트워크 업데이트가 클라이언트로 전송 => 클라이언트가 해당 변수의 버전을 새 값으로 업데이트
 *    => 복제 변수에 해당하는 데이터를 클라이언트로 전송하는 행위를 복제라고 한다!
 *    
 *   - 복제된 변수가 클라이언트에서 변경된다면 서버로 전송될까? => X, 복제는 양방향이 아니다.
 *     - 복제는 서버->클라이언트의 단방향으로만 작동
 *     - 플레이어의 입력 등 서버로 전달되어야 할 경우 RPC나 원격 프로시저 호출이라고 하는 복제된 함수의 형태로 전송됨
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	 GENERATED_BODY()
public:
	void AbilityActorInfoSet();

protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveffectHandle);

	
public:
	FEffectAssetTags EffectAssetTags;
 
};
