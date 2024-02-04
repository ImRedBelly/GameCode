#pragma once

#include "CoreMinimal.h"
#include "GameCodeBasePawn.h"
#include "SpiderPawn.generated.h"

UCLASS()
class GAMECODE_API ASpiderPawn : public AGameCodeBasePawn
{
	GENERATED_BODY()

public:
	ASpiderPawn();
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, BlueprintPure)

	FORCEINLINE float GetIKRightFrontFootOffset() const { return IKRightFrontFootOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetIKRightRearFootOffset() const { return IKRightRearFootOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetIKLeftFrontFootOffset() const { return IKLeftFrontFootOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetIKLeftRearFootOffset() const { return IKLeftRearFootOffset; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Spider bot")
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Spider bot|IK Settings")
	FName RightFrontFootSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Spider bot|IK Settings")
	FName RightRearFootSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Spider bot|IK Settings")
	FName LeftFrontFootSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Spider bot|IK Settings")
	FName LeftRearFootSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Spider bot|IK Settings", meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float IKTraceExtendDistance = 50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Spider bot|IK Settings", meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float IKInterpSpeed = 20.0f;

private:
	float GetIKOffsetForASocket(const FName& SocketName);

	float IKRightFrontFootOffset = 0.0f;
	float IKRightRearFootOffset = 0.0f;
	float IKLeftFrontFootOffset = 0.0f;
	float IKLeftRearFootOffset = 0.0f;

	float IKTraceDistance = 0.0f;
	float ISScale = 0.0f;
};
