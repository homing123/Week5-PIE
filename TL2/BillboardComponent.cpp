#include "pch.h"
#include "BillboardComponent.h"
#include "ResourceManager.h"
#include "CameraActor.h"
#include "Renderer.h"

UBillboardComponent::UBillboardComponent()
{
	QuadMesh = UResourceManager::GetInstance().GetQuadMesh();

	Material = NewObject<UMaterial>();

	UShader* BillboardShader = UResourceManager::GetInstance().Get<UShader>("StaticMeshShader.hlsl");
	if (BillboardShader)
	{
		Material->SetShader(BillboardShader);
	}

	InitializeComponent();
}

// Set Default Sprite
void UBillboardComponent::InitializeComponent()
{
	// jft
	UTexture* NewTexture = UResourceManager::GetInstance().Load<UTexture>("Pawn_64x.png");
	if (NewTexture)
	{
		Material->SetTexture(NewTexture);
	}
}

void UBillboardComponent::Render(URenderer* Renderer, const FMatrix& View, const FMatrix& Proj)
{
	if (!QuadMesh || !Material || !Material->GetShader() || !Material->GetTexture())
	{
		return;
	}

	ACameraActor* CameraActor = GetOwner()->GetWorld()->GetCameraActor();
	if (!CameraActor) return;

	FVector CamRight = CameraActor->GetActorRight();
	FVector CamUp = CameraActor->GetActorUp();

	 //FVector CameraPosition = CameraActor->GetActorLocation();
	// Renderer->UpdateBillboardConstantBuffers(Owner->GetActorLocation() + FVector(0.f, 0.f, 1.f) * Owner->GetActorScale().Z, View, Proj, CamRight, CamUp);

	// FVector WorldLocation = GetWorldLocation();
	// Renderer->UpdateBillboardConstantBuffers(WorldLocation, View, Proj, CamRight, CamUp);
	
	Renderer->PrepareShader(Material->GetShader());
	
	Renderer->OMSetBlendState(true);
	Renderer->RSSetState(EViewModeIndex::VMI_Unlit);

	Renderer->DrawIndexedPrimitiveComponent(this, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Renderer->OMSetBlendState(false);
}

void UBillboardComponent::SetTexture(const FString& InFilePath)
{
	if (!Material) return;
	
	// 리소스 매니저를 통해 텍스처를 로드하고 재질에 설정합니다.
	UTexture* NewTexture = UResourceManager::GetInstance().Load<UTexture>(InFilePath);
	if (NewTexture)
	{
		Material->SetTexture(NewTexture);
	}
}

UTexture* UBillboardComponent::GetTexture() const
{
	if (Material)
	{
		return Material->GetTexture();
	}
	return nullptr;
}