#pragma once
#include "Vector.h"
#include "ActorComponent.h"



// 부착 시 로컬을 유지할지, 월드를 유지할지
enum class EAttachmentRule
{
    KeepRelative,
    KeepWorld
};

class URenderer;
class USceneComponent : public UActorComponent
{
public:
    DECLARE_CLASS(USceneComponent, UActorComponent)
    USceneComponent();

protected:
    ~USceneComponent() override;


public:
    // ──────────────────────────────
    // Relative Transform API
    // ──────────────────────────────
    void SetRelativeLocation(const FVector& NewLocation);
    FVector GetRelativeLocation() const;

    void SetRelativeRotation(const FQuat& NewRotation);
    FQuat GetRelativeRotation() const;

    void SetRelativeScale(const FVector& NewScale);
    FVector GetRelativeScale() const;

    void SetWorldLocation(const FVector& WorldLocation);
    void SetRelativeTransform(const FTransform& InRelativeTransform);

    //void AddRelativeLocation(const FVector& DeltaLocation);
    //void AddRelativeRotation(const FQuat& DeltaRotation);
    //void AddRelativeScale3D(const FVector& DeltaScale);

    // ──────────────────────────────
    // World Transform API
    // ──────────────────────────────
    //const FTransform& GetWorldTransform() const;
    //void SetWorldTransform(const FTransform& W);

   /* void SetWorldLocation(const FVector& L);
    const FVector& GetWorldLocation() const;

    void SetWorldRotation(const FQuat& R);
    const FQuat& GetWorldRotation() const;

    void SetWorldScale(const FVector& S);
    const FVector& GetWorldScale() const;*/

 /*   void AddWorldOffset(const FVector& Delta);
    void AddWorldRotation(const FQuat& DeltaRot);
    void SetWorldLocationAndRotation(const FVector& L, const FQuat& R);

    void AddLocalOffset(const FVector& Delta);
    void AddLocalRotation(const FQuat& DeltaRot);
    void SetLocalLocationAndRotation(const FVector& L, const FQuat& R);*/

    const FMatrix& GetWorldMatrix(); // ToMatrixWithScale

    // ──────────────────────────────
    // Attach/Detach
    // ──────────────────────────────
    void SetupAttachment(USceneComponent* InParent);
    void DetachFromParent(bool bKeepWorld = true);

    // ──────────────────────────────
    // Hierarchy Access
    // ──────────────────────────────
    USceneComponent* GetAttachParent() const { return AttachParent; }
    const TArray<USceneComponent*>& GetAttachChildren() const { return AttachChildren; }
    UWorld* GetWorld() { return AttachParent->GetWorld(); }



protected:
    virtual void RenderDetail() override;
protected:
    
    FTransform RelativeTransform;

    // Hierarchy
    USceneComponent* AttachParent = nullptr;
    TArray<USceneComponent*> AttachChildren;

    FMatrix WorldMatrix;

private:
    void TransformDirty();
private:
    bool bUniformScale = true;
    bool bTransformDirty = true;
};
