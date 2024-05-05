


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SAdvancedRotationInputBox.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	/**
	 * 왜 bReplicates를 true로 해야함? (왜 플레이어 컨트롤러를 복제?)
	 * - 서버가 클라이언트와 연관된 액터의 정보를 모두 수집
	 * - 서버는 리플리케이트된 데이터의 값이 업데이트될 때마다 클라이언트로 전송
	 * - 클라이언트는 데이터를 받아 로컬에 적용함
	 * - bReplicates 플래그가 true로 설정되면 해당 액터에 대해 서버에 접속된 모든 클라이언트에 동기화
	 * - 일반적으로 생성자에 플래그를 둔다!
	 */
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	/**
	 * EnhancedInputLocalPlayerSubsystem에 매핑 컨텍스트를 액세스해 연동
	 * - 액세스하지 않을 경우 충돌 발생
	 * - 로컬 플레이어 클래스의 함수를 통해 접근 가능
	 * - GetSubsystem을 통해 정적 함수 액세스
	 * - assert를 활용해 Subsystem이 올바르게 접근되었는지 확인(null일 경우 액세스 x)
	 *	 AddMappingContext 함수를 통해 AuraContext가 우선순위가 0이 되도록 지정
	 * - Subsystem은 싱글톤으로 동작함(입력 기간 동안 한 개만 존재)
	 */
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	

	// 기본 마우스 커서 노출
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	/**
	 * FInputModeGameAndUI: UI와 인게임 간 상호작용을 도움
	 * EMouseLockMode::DoNotLock: 마우스 커서가 뷰포트에 고정되지 않음
	 * SetInputMode(): 커서 설정 적용
	 */
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);	// 마우스 숨김
	SetInputMode(InputModeData);
}

// 입력 매핑과 함수 바인딩
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	/**
	 * InputComponent는 컴포넌트의 주소를 저장하고 있어 포인터 타입으로 불러올 수 있음
	 * CastChecked가 실패하면 프로그램이 중단될 수 있음(어설션을 동시헤 실행하는 것과 같다.)
	 */
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	/**
	 * ForwardDirection: X가 정방향 축 제공 => 회전 벡터에 해당하는 전방 벡터 제공
	 * - 회전이 pitch와 roll을 0으로 만들면 순방향이 벡터가 됨
	 * - 카메라에서 캐릭터를 가리키는 방향이 수평 => 전방 방향
	 * RightDirection: Forward와 반대로 Y축에서 FRotationMatrix를 구함
	 */
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/**
	 * Line trace from cursor. There are several senarios;
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is Valid
	 *		- Highlight ThisActor
	 *	C. LastActor is Valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	C. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	D. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A (Do nothing)
		}
	}
	else
	{
		// LastActor != nullptr
		if(ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else
		{
			if(LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E (Do nothing)
			}
		}
	}
}
