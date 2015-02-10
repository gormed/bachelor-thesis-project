

#include "Caede.h"
#include "Item.h"

#include "Character/InventoryComponent.h"
#include "Character/Slots/ItemSlot.h"

AItem::AItem(const class FObjectInitializer& PCIP)
: Super(PCIP)
{
	Value = 0;
	Weight = 1;
	EmptyWeight = Weight;
	FullWeight = Weight;
	InternalItemName = "Unnamed Item";
	LocalizedItemName = FText::FromString(InternalItemName);
	IdenticationColor = FColor::Magenta;

	DefaultSceneRoot = PCIP.CreateDefaultSubobject<class USceneComponent>(this, "DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	ItemMesh = PCIP.CreateDefaultSubobject<class UStaticMeshComponent>(this, TEXT("Item Mesh"));
	ItemMesh->AttachTo(DefaultSceneRoot);

	InfoAnchor = PCIP.CreateDefaultSubobject<class USceneComponent>(this, "InfoAnchor");
	InfoAnchor->AttachTo(ItemMesh);

	CanChangeName = false;
	CanChangeWeight = false;
	CanChangeValue = false;
	CanChangeDropping = false;
	CanChangeEquipping = false;
	CanChangeEquipping = false;
	CanChangeUsing = false;
	CanChangeStore = false;

	CanBePickedUp = true;
	CanBeDropped = true;
	CanBeStored = true;
	CanBeUsed = false;
	CanBeEquipped = false;

	CurrentInventory = NULL;
	OriginalItemOwner = NULL;

	UsesHands = EHandSlot::RightHand;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

//////////////////////////////////////////////////////////////////////////

float AItem::PercentageAmount_Implementation()
{
	return PercentageAmount_Internal();
}

float AItem::ActualWeight_Implementation()
{
	return ActualWeight_Internal();
}

bool AItem::IsEmpty_Implementation()
{
	return IsEmpty_Internal();
}

float AItem::PercentageAmount_Internal()
{
	return 1;
}

float AItem::ActualWeight_Internal()
{
	return Weight;
}

bool AItem::IsEmpty_Internal()
{
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool AItem::ChangeName(FText newName)
{
	if (CanChangeName)
	{
		LocalizedItemName = newName;
		return true;
	}
	return false;
}

bool AItem::ChangeWeight(float newItemWeight)
{
	if (CanChangeWeight)
	{
		float oldItemWeight = Weight;
		Weight = newItemWeight;
		if (CurrentInventory)
		{
			CurrentInventory->CurrentInventoryWeight -= oldItemWeight;
			CurrentInventory->CurrentInventoryWeight += newItemWeight;
		}
		return true;
	}
	return false;
}

bool AItem::ChangeValue(float newItemValue)
{
	if (CanChangeValue)
	{
		Value = newItemValue;
		return true;
	}
	return false;
}

bool AItem::ChangeCanBeDropped(bool newCanBeDropped)
{
	if (CanChangeDropping)
	{
		CanBeDropped = newCanBeDropped;
		return true;
	}
	return false;
}

bool AItem::ChangeCanBeEquipped(bool newCanBeEqipped)
{
	if (CanChangeEquipping)
	{
		CanBeEquipped = newCanBeEqipped;
		return true;
	}
	return false;
}

bool AItem::ChangeCanBePickedUp(bool newCanBePickedUp)
{
	if (CanChangePickup)
	{
		CanBePickedUp = newCanBePickedUp;
		return true;
	}
	return false;
}

bool AItem::ChangeCanBeUsed(bool newCanBeUsed)
{
	if (CanChangeUsing)
	{
		CanBeUsed = newCanBeUsed;
		return true;
	}
	return false;
}

bool AItem::ChangeCanBeStored(bool newCanBeStored)
{
	if (CanChangeStore)
	{
		CanBeStored = newCanBeStored;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void AItem::TogglePhyiscsAndCollision(bool enabled)
{
	SetActorEnableCollision(enabled);
	if (ItemMesh != NULL)
	{
		ItemMesh->SetSimulatePhysics(enabled);
		ItemMesh->SetCollisionEnabled(enabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

//////////////////////////////////////////////////////////////////////////

void AItem::SetItemInventory(UInventoryComponent* itemOwner)
{
	if (OriginalItemOwner == NULL)
	{
		OriginalItemOwner = itemOwner->OwningItem->CurrentItemOwner;
	}
	CurrentItemOwner = itemOwner->OwningItem->CurrentItemOwner;
	CurrentInventory = itemOwner;
	CurrentItemSlot = NULL;
	//UE_LOG(LogCaede, Display, TEXT("Successfully added condition '%s = %s'"), *subCondition[0], *subCondition[1]);
}

void AItem::SetItemSlot(AItemSlot* itemSlot)
{
	if (OriginalItemOwner == NULL)
	{
		OriginalItemOwner = itemSlot->OwningComponent->OwningCharacter;
	}
	CurrentItemOwner = itemSlot->OwningComponent->OwningCharacter;
	CurrentItemSlot = itemSlot;
	CurrentInventory = NULL;
}

void AItem::SetItemOwner(class ACaedeCharacter* itemOwner)
{
	if (OriginalItemOwner == NULL)
	{
		OriginalItemOwner = itemOwner;
	}
	CurrentItemOwner = itemOwner;
}


void AItem::SetItemFree(bool freeSlot, bool freeInventory, bool freeCharacter)
{
	if (freeInventory)
	{
		CurrentInventory = NULL;
	}
	if (freeSlot)
	{
		CurrentItemSlot = NULL;
	}
	if (freeCharacter)
	{
		CurrentItemOwner = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

void AItem::ReceiveDestroyed()
{
	if (ItemMesh != NULL)
	{
		ItemMesh->DestroyComponent();
	}
	AActor::ReceiveDestroyed();
}

bool AItem::AttachItem_Implementation(AActor* parentActor, USceneComponent* parentMesh, FName socketName)
{
	if (parentActor == NULL)
	{
		UE_LOG(LogCaedeItem, Error, TEXT("Actor to attach item to is invalid!"));
		return false;
	}
	if (parentMesh == NULL)
	{
		UE_LOG(LogCaedeItem, Error, TEXT("Parent component to attach item mesh to is invalid!"));
		return false;
	}
	if (socketName.IsValid() && socketName.IsNone())
	{
		UE_LOG(LogCaedeItem, Error, TEXT("Socket name to attach item to is invalid or none!"));
		return false;
	}
	USkeletalMeshComponent* skeletalMesh = Cast<USkeletalMeshComponent>(parentMesh);
	UStaticMeshComponent* staticMesh = Cast<UStaticMeshComponent>(parentMesh);

	AItemSlot* parentItemSlot = Cast<AItemSlot>(parentActor);
	ACaedeCharacter* parentCharacter = Cast<ACaedeCharacter>(parentActor);

	if (parentItemSlot != NULL)
	{
		AttachRootComponentToActor(parentItemSlot, NAME_None, EAttachLocation::KeepWorldPosition, false);
	}
	else if (parentCharacter != NULL)
	{
		AttachRootComponentToActor(parentCharacter, NAME_None, EAttachLocation::KeepWorldPosition, false);
	}
	else
	{
		UE_LOG(LogCaedeItem, Error, TEXT("The parent actor needs to be either a 'AItemSlot' or a 'ACaedeCharacter'!"));
		return false;
	}

	if (skeletalMesh != NULL && skeletalMesh->CanAttachAsChild(ItemMesh, socketName))
	{
		ItemMesh->AttachTo(skeletalMesh, socketName, EAttachLocation::SnapToTarget, true);

	}
	else if (staticMesh != NULL && staticMesh->CanAttachAsChild(ItemMesh, socketName))
	{
		ItemMesh->AttachTo(staticMesh, socketName, EAttachLocation::SnapToTarget, true);
	}
	else
	{
		UE_LOG(LogCaedeItem, Error, TEXT("The parent mesh is no 'USkeletalMeshComponent' or 'UStaticMeshComponent'!"));
		return false;
	}

	UE_LOG(LogCaedeItem, Log, TEXT("Attached item %s to %s on socket %s..."), *InternalItemName, *parentActor->GetName(), *socketName.ToString());
	return true;
}

bool AItem::DetachItem_Implementation()
{
	UE_LOG(LogCaedeItem, Log, TEXT("Detaching item %s from %s..."), *InternalItemName, *GetOuter()->GetName());
	ItemMesh->DetachFromParent(true);
	DetachRootComponentFromParent(true);
	return true;
}
