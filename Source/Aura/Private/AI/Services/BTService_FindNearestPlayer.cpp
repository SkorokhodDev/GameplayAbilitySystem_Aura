// Copyrights Skorokhod.


#include "AI/Services/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* OwningPawn = AIOwner->GetPawn();

	// Determines target tag. Target Tag will be an Enemy if this service owner is a player.
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float closestDistance = TNumericLimits<float>::Max();
	AActor* closestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Orange, *Actor->GetName());
	
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float distance = OwningPawn->GetDistanceTo(Actor);
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestActor = Actor;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, closestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, closestDistance);
}
