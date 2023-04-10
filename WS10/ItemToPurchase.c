#include <stdio.h>
#include "ItemToPurchase.h"
#include <string.h>


// Make the inputted blank as in the name is "none", price = 0, and quantity = 0
void MakeItemBlank(ItemToPurchase *item) {
    strcpy(item->itemName, "none");
    item->itemPrice = 0;
    item->itemQuantity = 0;
}

// Display item info in the format of <item name> <quantity> @ $<price> <total>
void PrintItemCost(ItemToPurchase *item) {
    printf("%s %d @ $%d = $%d\n", item->itemName, item->itemQuantity,item->itemPrice, item->itemPrice * item->itemQuantity);
}
