#include <stdio.h>
#include "ItemToPurchase.h"
#include <stdlib.h>
#include <string.h>

#define ITEMS 2

// Initialize a single Item onto the heap and return it
ItemToPurchase *promptItem(int count) {

    printf("Item %d\n"
    "Enter the item name:\n"
    , count);

    char *itemName;
    size_t length = 0;

    length = getline(&itemName, &length, stdin);

    itemName[--length] = '\0';

    ItemToPurchase *newItem = (ItemToPurchase *) malloc(sizeof(ItemToPurchase) + length); // sizeof for flexible array struct does not take into account size of the array, so we add on the length

    strcpy(newItem->itemName, itemName);

    printf("Enter the item price:\n");
    scanf("%d", &(newItem->itemPrice));

    printf("Enter the item quantity:\n");
    scanf("%d", &(newItem->itemQuantity));

    printf("\n");

    free(itemName);
    return newItem;
}

// Print a receipt of all Items and the total cost
void printTotalCost(ItemToPurchase **items) {
    printf("TOTAL COST\n");

    int total = 0;

    for (int i = 0; i < ITEMS; i++) {
        PrintItemCost(items[i]);
        total += (items[i]->itemPrice * items[i]->itemQuantity);
    }

    printf("\nTotal: $%d\n", total);
}

int main() {

    ItemToPurchase **items = malloc(ITEMS * sizeof(ItemToPurchase*));

    for (int i = 0; i < ITEMS; i++) {

        items[i] = promptItem(i + 1);

        char c;
        do {
            c = getchar();
        } while (c != '\n' && c != EOF); // remove nl chars left behind by scanf

    }

    printTotalCost(items);

    for (int i = 0; i < ITEMS; i++)
        free(items[i]);

    free(items);
}
