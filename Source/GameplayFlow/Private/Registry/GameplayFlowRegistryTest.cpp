#include "Registry/GameplayFlowRegistry.h"
#include "Registry/GameplayFlowRegistrySubsystem.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

BEGIN_DEFINE_SPEC(FGameplayFlowRegistryTest, "GameplayFlow.Registry.GameplayFlowRegistryTest", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
UGameplayFlowRegistrySubsystem* RegistrySubsystem;
END_DEFINE_SPEC(FGameplayFlowRegistryTest)

void FGameplayFlowRegistryTest::Define() {
	BeforeEach([this]()
	{		
		const FString MapName = TEXT("AutomationTest");
		if (!TestTrue("Open automation map", AutomationOpenMap(MapName, true)))
		{
			return false;
		}
		UWorld* World = nullptr;
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
			{
				World = Context.World();
				break;
			}
		}
		if (!TestNotNull("World valid", World))
		{
			return false;
		}
		if (!TestNotNull("Game Instance valid", World->GetGameInstance()))
		{
			return false;
		}
		RegistrySubsystem = World->GetGameInstance()->GetSubsystem<UGameplayFlowRegistrySubsystem>();
		TestNotNull("Registry valid", RegistrySubsystem);
		if (RegistrySubsystem->GetRegistry())
		{
			RegistrySubsystem->DeinitializeRegistry();
		}
		return true;
	});
	It("Class Initialize", [this](){
		UClass* NullClass = nullptr;
		AddExpectedMessage(TEXT("Attempted to setup registry null registry class!"));
		const bool bNullClassTest = RegistrySubsystem->InitializeRegistry(NullClass);
		TestFalse("Null class fail", bNullClassTest);

		const bool bFlowRegistryInit = RegistrySubsystem->InitializeRegistry(UGameplayFlowRegistry::StaticClass());
		TestTrue("Class initialize registry", bFlowRegistryInit);

		TestNotNull("Registry valid", RegistrySubsystem->GetRegistry().GetObject());

		TestTrue("Registry deinitialised", RegistrySubsystem->DeinitializeRegistry());
	}); 
	It("Object Initialize", [this](){		
		UObject* NullObject = nullptr;
		AddExpectedMessage(TEXT("Attempted to setup registry null registry!"));
		const bool bNullObjectTest = RegistrySubsystem->InitializeRegistry(NullObject);
		TestFalse("Null object fail", bNullObjectTest);

		UGameplayFlowRegistry* NewRegistry = NewObject<UGameplayFlowRegistry>(RegistrySubsystem);
		const bool bFlowRegistryInit = RegistrySubsystem->InitializeRegistry(NewRegistry);
		TestTrue("Object initialize registry", bFlowRegistryInit);
		
		TestNotNull("Registry valid", RegistrySubsystem->GetRegistry().GetObject());

		TestTrue("Registry deinitialised", RegistrySubsystem->DeinitializeRegistry());
	});
	AfterEach([this]()
	{		
		if (RegistrySubsystem)
		{
			RegistrySubsystem->DeinitializeRegistry();
		}
	});
}