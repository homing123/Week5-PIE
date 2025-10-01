#include "pch.h"
#include "SceneComponent.h"
#include <algorithm>
#include "ObjectFactory.h"
#include "ImGui/imgui.h"

USceneComponent::USceneComponent()
{

}

USceneComponent::~USceneComponent()
{
    // 자식 메모리 해제
    // 복사본을 만들어 부모 리스트 무효화 문제를 피함
    TArray<USceneComponent*> ChildrenCopy = AttachChildren;
    AttachChildren.clear();
    for (USceneComponent* Child : ChildrenCopy)
    {
        if (Child)
        {
            ObjectFactory::DeleteObject(Child);
        }
    }

    // 부모에서 자신 제거
    if (AttachParent)
    {
        TArray<USceneComponent*>& ParentChildren = AttachParent->AttachChildren;
        ParentChildren.Remove(this);
        AttachParent = nullptr;
    }
}

// ──────────────────────────────
// Relative API
// ──────────────────────────────
void USceneComponent::SetRelativeLocation(const FVector& NewLocation){
    RelativeTransform.Translation = NewLocation;
    TransformDirty();
}
FVector USceneComponent::GetRelativeLocation() const { return  RelativeTransform.Translation; }

void USceneComponent::SetRelativeRotation(const FQuat& NewRotation)
{
    RelativeTransform.Rotation = NewRotation;
    TransformDirty();
}
FQuat USceneComponent::GetRelativeRotation() const { return  RelativeTransform.Rotation; }

void USceneComponent::SetRelativeScale(const FVector& NewScale)
{
    RelativeTransform.Scale3D = NewScale;
    TransformDirty();
}
FVector USceneComponent::GetRelativeScale() const { return  RelativeTransform.Scale3D; }


void USceneComponent::SetWorldLocation(const FVector& WorldLocation)
{
    TransformDirty();
    if (AttachParent != nullptr) 
    {
        FMatrix InverseParentWorld = AttachParent->GetWorldMatrix().InverseAffine();
        RelativeTransform.Translation = WorldLocation * InverseParentWorld;
    }
    else 
    {
        RelativeTransform.Translation = WorldLocation;
    }
}
FVector USceneComponent::GetWorldLocation()
{
    if (AttachParent != nullptr)
    {
        const FMatrix& ParentWorldMatrix = AttachParent->GetWorldMatrix();
        FVector temp = RelativeTransform.Translation * ParentWorldMatrix;
        return RelativeTransform.Translation * ParentWorldMatrix;
    }
    return RelativeTransform.Translation;
}

void USceneComponent::SetRelativeTransform(const FTransform& InRelativeTransform)
{
    RelativeTransform = InRelativeTransform;
    TransformDirty();
}
void USceneComponent::TransformDirty()
{
    bTransformDirty = true;
    for (USceneComponent* Child : AttachChildren)
    {
        Child->TransformDirty();
    }
}
FVector USceneComponent::GetForward()
{
    if (AttachParent != nullptr)
    {
        const FMatrix& ParentWorldMatrix = AttachParent->GetWorldMatrix();
        FMatrix CurRotMatrix = RelativeTransform.Rotation.ToMatrix() * ParentWorldMatrix;
        return CurRotMatrix.GetForward();        
    }
    return RelativeTransform.Rotation.GetForward();
}
FVector USceneComponent::GetRight()
{
    if (AttachParent != nullptr)
    {
        const FMatrix& ParentWorldMatrix = AttachParent->GetWorldMatrix();
        FMatrix CurRotMatrix = RelativeTransform.Rotation.ToMatrix() * ParentWorldMatrix;
        return CurRotMatrix.GetRight();
    }
    return RelativeTransform.Rotation.GetRight();
}
FVector USceneComponent::GetUp()
{
    if (AttachParent != nullptr)
    {
        const FMatrix& ParentWorldMatrix = AttachParent->GetWorldMatrix();
        FMatrix CurRotMatrix = RelativeTransform.Rotation.ToMatrix() * ParentWorldMatrix;
        return CurRotMatrix.GetUp();
    }
    return RelativeTransform.Rotation.GetUp();
}

// ──────────────────────────────
// World API
// ──────────────────────────────
//const FTransform& USceneComponent::GetWorldTransform() const
//{
//    if (bTransformDirty)
//    {
//        if (AttachParent != nullptr)
//        {
//            const FTransform& ParentTransform = AttachParent->GetWorldTransform();
//            WorldTransform = RelativeTransform * ParentTransform;
//        }
//    }
//    
//    return RelativeTransform;
//}
//
//void USceneComponent::SetWorldTransform(const FTransform& W)
//{
//    if (AttachParent)
//    {
//        const FTransform ParentWorld = AttachParent->GetWorldTransform();
//        RelativeTransform = ParentWorld.Inverse() * W;
//    }
//    else
//    {
//        RelativeTransform = W;
//    }
//
//    RelativeLocation = RelativeTransform.Translation;
//    RelativeRotation = RelativeTransform.Rotation;
//    RelativeScale = RelativeTransform.Scale3D;
//}
 
//void USceneComponent::SetWorldLocation(const FVector& L)
//{
//    FTransform W = GetWorldTransform();
//    W.Translation = L;
//    SetWorldTransform(W);
//}
//FVector USceneComponent::GetWorldLocation() const
//{
//    return GetWorldTransform().Translation;
//}
//
//void USceneComponent::SetWorldRotation(const FQuat& R)
//{
//    FTransform W = GetWorldTransform();
//    W.Rotation = R;
//    SetWorldTransform(W);
//}
//FQuat USceneComponent::GetWorldRotation() const
//{
//    return GetWorldTransform().Rotation;
//}
//
//void USceneComponent::SetWorldScale(const FVector& S)
//{
//    FTransform W = GetWorldTransform();
//    W.Scale3D = S;
//    SetWorldTransform(W);
//}
//FVector USceneComponent::GetWorldScale() const
//{
//    return GetWorldTransform().Scale3D;
//}

//void USceneComponent::AddWorldOffset(const FVector& Delta)
//{
//    FTransform W = GetWorldTransform();
//    W.Translation = W.Translation + Delta;
//    SetWorldTransform(W);
//}
//
//void USceneComponent::AddWorldRotation(const FQuat& DeltaRot)
//{
//    FTransform W = GetWorldTransform();
//    W.Rotation = DeltaRot * W.Rotation;
//    SetWorldTransform(W);
//}
//
//
//void USceneComponent::SetWorldLocationAndRotation(const FVector& L, const FQuat& R)
//{
//    FTransform W = GetWorldTransform();
//    W.Translation = L;
//    W.Rotation = R;
//    SetWorldTransform(W);
//}
//
//void USceneComponent::AddLocalOffset(const FVector& Delta)
//{
//    const FVector parentDelta = RelativeRotation.RotateVector(Delta);
//    RelativeLocation = RelativeLocation + parentDelta;
//    UpdateRelativeTransform();
//}
//
//void USceneComponent::AddLocalRotation(const FQuat& DeltaRot)
//{
//    RelativeRotation = (RelativeRotation * DeltaRot).GetNormalized(); // 로컬: 우측곱
//    UpdateRelativeTransform();
//}
//
//void USceneComponent::SetLocalLocationAndRotation(const FVector& L, const FQuat& R)
//{
//    RelativeLocation = L;
//    RelativeRotation = R.GetNormalized();
//    UpdateRelativeTransform();
//}


const FMatrix& USceneComponent::GetWorldMatrix()
{
    if (bTransformDirty)
    {
        bTransformDirty = false;
        if (AttachParent != nullptr)
        {
            WorldMatrix = RelativeTransform.GetWorldMatrix() * AttachParent->GetWorldMatrix();
        }
        else
        {
            WorldMatrix = RelativeTransform.GetWorldMatrix();
        }
    }
    return WorldMatrix;
}

// ──────────────────────────────
// Attach / Detach
// ──────────────────────────────
void USceneComponent::SetupAttachment(USceneComponent* InParent)
{
    if (AttachParent == InParent) return;

    // 기존 부모에서 제거
    if (AttachParent)
    {
        auto& Siblings = AttachParent->AttachChildren;
        Siblings.erase(std::remove(Siblings.begin(), Siblings.end(), this), Siblings.end());
    }

    // 새 부모 설정
    AttachParent = InParent;
    if (AttachParent)
        AttachParent->AttachChildren.push_back(this);

    // 규칙 적용

    TransformDirty();
}

void USceneComponent::DetachFromParent(bool bKeepWorld)
{
    if (AttachParent)
    {
        auto& Siblings = AttachParent->AttachChildren;
        Siblings.erase(std::remove(Siblings.begin(), Siblings.end(), this), Siblings.end());
        AttachParent = nullptr;
    }

    TransformDirty();

}

// ──────────────────────────────
// 내부 유틸
// ──────────────────────────────




void USceneComponent::RenderDetail()
{
    UActorComponent::RenderDetail();

    if (ImGui::TreeNode("Transform"))
    {
        // Location 편집
        if (ImGui::DragFloat3("Location", &RelativeTransform.Translation.X, 0.1f))
        {
            SetRelativeLocation(RelativeTransform.Translation);
        }

        // Rotation 편집 (Euler angles)
        FVector Euler = RelativeTransform.Rotation.ToEulerDegree();
        if (ImGui::DragFloat3("Rotation", &Euler.X, 0.5f))
        {
            SetRelativeRotation(FQuat::MakeFromEuler(Euler));
        }

        // Scale 편집
        ImGui::Checkbox("Uniform Scale", &bUniformScale);

        if (bUniformScale)
        {
            float UniformScale = RelativeTransform.Scale3D.X;
            if (ImGui::DragFloat("Scale", &UniformScale, 0.01f, 0.01f, 10.0f))
            {
                SetRelativeScale(UniformScale);
            }
        }
        else
        {
            if (ImGui::DragFloat3("Scale", &RelativeTransform.Scale3D.X, 0.01f, 0.01f, 10.0f))
            {
                SetRelativeScale(RelativeTransform.Scale3D);
            }
        }
        ImGui::TreePop();
    }
}