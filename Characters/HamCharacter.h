#pragma once

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "HamCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHamPowerUp; //Powerup

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateKilllog, FString, Victim, FString, Killer);

UCLASS(Abstract)
class AHamCharacter : public APawn
{
	GENERATED_BODY()
public:
	AHamCharacter();
	UFUNCTION(BlueprintCallable)
	void MoveInDirection(const float DeltaTime, const FVector MoveDirection, const float Yaw);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName="OnTakeDamge"))
	void ReceiveOnTakeDamage();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName="OnDeath"))
	void ReceiveOnDeath();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName="OnRespawn"))
	void ReceiveOnRespawn();
	UFUNCTION(BlueprintCallable)
	void Respawn();
	UFUNCTION(BlueprintCallable)
	void Die();
	UFUNCTION(NetMulticast, Reliable)
	void UpdateKillLogMulticast();

	//PowerUp
	UFUNCTION(BlueprintCallable)
	void HandleActivatePowerUp(AHamCharacter* Me);
	
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Destroyed() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void AddAngularVelocity(const float DeltaTime, FVector MoveDirection, const float Yaw) const;
	void AddInAirVelocity(const float DeltaTime, FVector MoveDirection, const float Yaw) const;
	void ReduceVelocityToTurn(FVector& CurrentVelocity, const FVector MoveDirection, const float MaxSpeedSquared, const float DeltaSpeed) const;
	void SetOnGround();
	void SetAirShotGround();
	void CheckOutOfBounds();

private:
	UFUNCTION(Server, Reliable)
	void GetServerStartTransform();
	UFUNCTION(Server, Reliable)
	void UpdateDeath();
	UFUNCTION(Server, Reliable)
	void UpdateKills();
	
public:
	UPROPERTY(BlueprintAssignable)
	FUpdateKilllog FUpdateKilllog;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* Collider;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, Replicated)
	float HealthPoints = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealthPoints = 3;
	UPROPERTY(BlueprintReadWrite, Replicated)
	FString CharacterName = "DefaultName";
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bHitPlayer = false;
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bKilledPlayer = false;

	// Score counting
	UPROPERTY(BlueprintReadOnly, Replicated)
	AHamCharacter* Killer = this;
	UPROPERTY(BlueprintReadWrite)
	bool bShowKillFeed = false;
	UPROPERTY(BlueprintReadOnly, Replicated)
	int Deaths = 0;
	UPROPERTY(BlueprintReadOnly, Replicated)
	int Kills = 0;

	//Powerup
	UPROPERTY(EditAnywhere)
	UHamPowerUp* PowerUp;
	bool bPowerUpActivated = false;
	UPROPERTY(EditAnywhere)
	bool bPickUpPowerup = true;

	void EquipPowerUp(UHamPowerUp* NewPowerUp);
	UFUNCTION(BlueprintPure)
	bool HasEquippedPowerUp();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AirShotDistance = 250.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SecondsToMaxAngularSpeed = .1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxAngularSpeed = 1500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SecondsToMaxInAirSpeed = .5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxInAirSpeed = 2500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RespawnTime = 3.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DistanceToGround = 3.f;
	UPROPERTY(Replicated)
	FTransform PlayerStart;

	UPROPERTY(BlueprintReadWrite)
	bool bCanShoot;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bOnGround;
	UPROPERTY(BlueprintReadOnly, Replicated)
	bool AirShotGround;
	
	bool bWaitingOnRespawn;
	
protected:
	bool bStopAngularVelocity;
	inline static TArray<TWeakObjectPtr<AHamCharacter>> AllCharacters = {};

	
private:
	UPROPERTY(Replicated)
	bool bPowerupEquiped;
	
	float AngularAcceleration;
	float MaxAngularSpeedSquared;
	float InAirAcceleration;
	float MaxInAirSpeedSquared;
	float RespawnTimer;
	FVector CurrentAddedInAirVelocity;
	FHitResult GroundHit;
	FCollisionShape ColShape;
	FCollisionShape OverlapShape;
	TArray<FOverlapResult> OutOfBoundsResults;
};