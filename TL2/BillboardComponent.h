#pragma once
#include "MeshComponent.h"
class UStaticMesh;
class URenderer;
class UBillboardComponent : public UMeshComponent
{
public:
	DECLARE_CLASS(UBillboardComponent, UMeshComponent)

	UBillboardComponent();
	
	virtual void InitializeComponent() override;
	virtual void Render(URenderer* Renderer, const FMatrix& View, const FMatrix& Proj) override;
	void SetTexture(const FString& InFilePath);
	UTexture* GetTexture() const;
	UStaticMesh* GetStaticMesh() const { return QuadMesh; }

protected:
	virtual ~UBillboardComponent() = default;
	
private:
	UStaticMesh* QuadMesh = nullptr;
	// U, V, UL, VL
	// OpacityMaskRefVal
};

