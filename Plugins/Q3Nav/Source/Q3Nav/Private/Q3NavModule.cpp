
#include "Q3NavPrivatePCH.h"
#include "Q3NavGridVolume.h"
#include "IQ3Nav.h"

class Q3NavModule : public IQ3Nav
{
public:
	virtual void StartupModule() override;
	virtual TArray<FVector> FindPath(UWorld* World, const FVector& StartLocation, const FVector& EndLocation) const override;
};

IMPLEMENT_MODULE(Q3NavModule, Q3Nav);
DEFINE_LOG_CATEGORY(Q3Nav);



static void OnBuildNavigationEvent(UWorld* World)
{
	for (TActorIterator<AQ3NavGridVolume> It(World); It; ++It)
	{
		AQ3NavGridVolume* Volume = *It;
		Volume->BuildGrid();
	}
}


void Q3NavModule::StartupModule()
{
#if WITH_EDITORONLY_DATA
	FWorldDelegates::OnBuildNavigation.AddStatic(&OnBuildNavigationEvent);
#endif
}


TArray<FVector> Q3NavModule::FindPath(UWorld* World, const FVector& StartLocation, const FVector& EndLocation) const
{
	TArray<FVector> Path;

	for (TActorIterator<AQ3NavGridVolume> It(World); It; ++It)
	{
		Path = (*It)->FindPath(StartLocation, EndLocation);

		if (Path.Num() > 0)
		{
			break;
		}
	}

	for (const auto& Location : Path)
	{
		//DrawDebugSolidBox(World, Location, FVector(8, 8, 8), FColor(0, 0, 255));
	}

	return Path;
}
