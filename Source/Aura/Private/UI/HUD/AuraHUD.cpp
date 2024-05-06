


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	// 오버레이 위젯 컨트롤러를 싱글톤으로 관리
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 뮈젯 및 위젯 컨트롤러 생성 
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class ininitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller uninitialized, please fill out BP_AuraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	// 위젯 컨트롤러 매개변수를 전달하는 GetOverlayWidgetController 함수 호출
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// 위젯에 위젯 컨트롤러 설정
	OverlayWidget->SetWidgetController(WidgetController);
	
	// 위젯 컨트롤러가 바인딩 된 이후에 브로드캐스트를 수행해야 함!
	WidgetController->BroadcastInitialValues();

	
	Widget->AddToViewport();

	
}
