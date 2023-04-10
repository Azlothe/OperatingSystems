// Flexible array struct
typedef struct ItemToPurchase {
    int itemPrice;
    int itemQuantity;
    char itemName[];
} ItemToPurchase;

void MakeItemBlank(ItemToPurchase *item);
void PrintItemCost(ItemToPurchase *item);
