#include "pch.h"
#include "PrimitiveComponent.h"
#include "SceneLoader.h"
#include "SceneComponent.h"
#include "SceneRotationUtils.h"

void UPrimitiveComponent::SetMaterial(const FString& FilePath, EVertexLayoutType layoutType)
{
    Material = UResourceManager::GetInstance().Load<UMaterial>(FilePath, layoutType);
}

void UPrimitiveComponent::Serialize(bool bIsLoading, FPrimitiveData& InOut)
{
    if (bIsLoading)
    {
        // FPrimitiveData -> 컴포넌트 월드 트랜스폼
        SetRelativeLocation(InOut.Location);
        SetRelativeRotation(FQuat::MakeFromEuler(InOut.Rotation));
        SetRelativeScale(InOut.Scale);
    }
    else
    {
        // 컴포넌트 월드 트랜스폼 -> FPrimitiveData
        InOut.Location = GetRelativeLocation();
        InOut.Rotation = GetRelativeRotation().ToEuler();
        InOut.Scale = GetRelativeScale();
    }
}
void UPrimitiveComponent::RenderDetail()
{
    USceneComponent::RenderDetail();
}