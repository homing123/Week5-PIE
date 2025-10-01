#include "pch.h"
#include "Actor.h"
#include "SceneComponent.h"
#include "ObjectFactory.h"
#include "ShapeComponent.h"
#include "AABoundingBoxComponent.h"   
#include "MeshComponent.h"
#include "TextRenderComponent.h"
AActor::AActor()
{
    Name = "DefaultActor";
    RootComponent= CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
    //CollisionComponent = CreateDefaultSubobject<UAABoundingBoxComponent>(FName("CollisionBox"));
    //UTextRenderComponent* TextComp = NewObject<UTextRenderComponent>();
    //TextComp->SetOwner(this);
   // AddComponent(TextComp);    
}

AActor::~AActor()
{
    //// 1) Delete root: cascades to attached children
    //if (RootComponent)
    //{
    //    ObjectFactory::DeleteObject(RootComponent);
    //    RootComponent = nullptr;
    //}
    // 2) Delete any remaining components not under the root tree (safe: DeleteObject checks GUObjectArray)
    for (UActorComponent*& Comp : Components)
    {
        if (Comp)
        {
            ObjectFactory::DeleteObject(Comp);
            Comp = nullptr;
        }
    }
    Components.Empty();
    //TextComp->SetupAttachment(GetRootComponent());
}

void AActor::BeginPlay()
{
}

void AActor::Tick(float DeltaSeconds)
{
}

void AActor::Destroy()
{
    if (!bCanEverTick) return;
    // Prefer world-managed destruction to remove from world actor list
    if (World)
    {
        // Avoid using 'this' after the call
        World->DestroyActor(this);
        return;
    }
    // Fallback: directly delete the actor via factory
    ObjectFactory::DeleteObject(this);
}

void AActor::SetActorLocation(const FVector& NewLocation)
{
    if (RootComponent)
    {
        RootComponent->SetRelativeLocation(NewLocation);
    }
}

FVector AActor::GetActorLocation() const
{
    return RootComponent ? RootComponent->GetRelativeLocation() : FVector();
}

void AActor::SetActorRotation(const FVector& EulerDegree)
{
    if (RootComponent)
    {
        RootComponent->SetRelativeRotation(FQuat::MakeFromEuler(EulerDegree));
    }
}

void AActor::SetActorRotation(const FQuat& InQuat)
{
    if (RootComponent)
    {
        RootComponent->SetRelativeRotation(InQuat);
    }
}

FQuat AActor::GetActorRotation() const
{
    return RootComponent ? RootComponent->GetRelativeRotation() : FQuat();
}

void AActor::SetActorScale(const FVector& NewScale)
{
    if (RootComponent)
    {
        RootComponent->SetRelativeScale(NewScale);
    }
}

FVector AActor::GetActorScale() const
{
    return RootComponent ? RootComponent->GetRelativeScale() : FVector(1, 1, 1);
}

FMatrix AActor::GetWorldMatrix() const
{
    return RootComponent ? RootComponent->GetWorldMatrix() : FMatrix::Identity();
}


void AActor::SetActorTransform(const FTransform& Transform)
{
    RootComponent->SetRelativeTransform(Transform);
}

const TArray<UActorComponent*>& AActor::GetComponents() const
{
    return Components;
}

void AActor::AddComponent(UActorComponent* Component)
{
    if (!Component)
    {
        return;
    }
    USceneComponent* SceneComp = nullptr;
    if (SceneComp = Cast<USceneComponent>(Component))
    {
        if (!RootComponent)
        {
            RootComponent = SceneComp;
        }
        else
        {
            SceneComp->SetupAttachment(RootComponent);
        }
    }
    Components.push_back(Component);
    Component->SetOwner(this);
}
