// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceSphere.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "StaticMeshResources.h"



#define RED 2
#define GREEN 1
#define BLUE 0
#define ALPHA 3

#define ALPHA_CHECK 200

void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}
// Sets default values
ASpaceSphere::ASpaceSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Sphere");
	static ConstructorHelpers::FObjectFinder<UTexture2D> BlackText(TEXT("/Game/Black"));
	textureToReadFrom = BlackText.Object;
}

// Called when the game starts or when spawned
void ASpaceSphere::BeginPlay()
{
	Super::BeginPlay();
	FSpaceSphereAlg::JoyInit(mDynamicColors,mDataSize,h,w,StarsDensity,StarsBrightness);
	
}

// Called every frame
void ASpaceSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMaterial();
	if (FSpaceSphereAlg::IsThreadFinished() && bC == false)
	{
		bC = true;

		FSpaceSphereAlg::Shutdown();
	}
		
}

void ASpaceSphere::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (textureToReadFrom)
	{
		w = textureToReadFrom->GetSizeX();
		h = textureToReadFrom->GetSizeY();
		//Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
		mDynamicMaterials.Add(StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0));
		//Create a dynamic texture with the default compression (B8G8R8A8)
		mDynamicTexture = UTexture2D::CreateTransient(w, h);
		//Make sure it won't be compressed
		mDynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		//Turn off Gamma-correction
		mDynamicTexture->SRGB = 0;
		//Guarantee no garbage collection by adding it as a root reference
		mDynamicTexture->AddToRoot();
		//Update the texture with new variable values.
		mDynamicTexture->UpdateResource();
		//Grab the colorvalues from our existing texture (the one we created at '''Texture Setup''') and copy it into a uint8* mTextureColors variable.

		FTexture2DMipMap& readMip = textureToReadFrom->PlatformData->Mips[0];
		mDataSize = w * h * 4; // * 4 because we're working with uint8's - which are 4 bytes large
		mDataSqrtSize = w * 4; // * 4 because we're working with uint8's - which are 4 bytes large
		readMip.BulkData.GetCopy((void**)&mTextureColors);
		// Initalize our dynamic pixel array with data size
		mDynamicColors = new uint8[mDataSize];
		// Copy our current texture's colors into our dynamic colors
		FMemory::Memcpy(mDynamicColors, mTextureColors, mDataSize);
		// Create a new texture region with the width and height of our dynamic texture
		mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);
		// Set the Paramater in our material to our texture
		mDynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", mDynamicTexture);
		UpdateMaterial();
	}

	
}

void ASpaceSphere::UpdateMaterial()
{
	UpdateTextureRegions(mDynamicTexture, 0, 1, mUpdateTextureRegion, mDataSqrtSize, (uint32)4, mDynamicColors, false);
	mDynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", mDynamicTexture);
}

void ASpaceSphere::Test()
{
	if (mDynamicColors)
	{
		mDynamicColors[0] = 255; // Set the Blue channel in pixel nr. 0 to 255
		mDynamicColors[1] = 120; // Set the Green channel in pixel nr. 0 to 120
		mDynamicColors[2] = 120; // Set the Red channel in pixel nr. 0 to 120
		mDynamicColors[3] = 120; // Set the Alpha channel in pixel nr. 0 to 120
		mDynamicColors[4] = 120; // Set the Blue channel in pixel nr. 1 to 120
								 // etc etc
								 // Array example:
		int pixelAmount = mDataSize / 4;
		for (int i = 0; i < pixelAmount; ++i)
		{
			int blue = i * 4 + 0;
			int green = i * 4 + 1;
			int red = i * 4 + 2;
			int alpha = i * 4 + 3;
			mDynamicColors[red] = 255; // Set pixel's red value to 120
		}
		UpdateMaterial();
	}
}

