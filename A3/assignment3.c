#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Defining constants
#define MAX_CART_ITEMS 100
#define MAX_BAG_WT 5.0
#define MAX_BAG_ITEMS 25

//Defning structures
typedef struct{
  char *name;
  float cost;
  float weight;
  int perishable;
} GroceryItem;

typedef struct{
  GroceryItem *itemsInBag[MAX_BAG_ITEMS];
  int itemCount;
  float bagWeight;
} Bag;

typedef struct{
  GroceryItem *looseItems[MAX_CART_ITEMS];
  Bag **bags;
  int itemCount;
  int bagCount;
} Cart;

//Function Prototypes
int addItemToCart(GroceryItem*, Cart*);
int addItemToBag(GroceryItem*, Bag*);
int removeItemFromBag(GroceryItem*, Bag*);
void displayGroceryItem(GroceryItem*);
void displayBag(Bag*, int);
void displayCart(Cart*);
void packCartItems(Cart*);
void removePerishable(Cart*);

int main(){
  srand(time(NULL));

  //Sample data
  GroceryItem testItems[12];
  char *sampleItemNames[] = {"Smart-Ones Frozen Entrees", "SnackPack Pudding", "Breyers Chocolate Icecream", "Nabob Coffee", "Gold Seal Salmon", "Ocean Spray Cranberry Cocktail", "Heinz Beans Original", "Lean Ground Beef", "5-Alive Frozen Juice", "Coca-Cola 12-pack", "Toilet Paper - 48 pack", "Crate of milk"};
  float sampleItemPrices[] = {1.99, 0.99, 2.99, 3.99, 1.99, 2.99, 0.79, 4.94, 0.75,
                              3.49, 40.96, 12.99};
  float sampleItemWeights[] = {0.311, 0.396, 2.27, 0.326, 0.213, 2.26, 0.477, 0.75,
                              0.426, 5.112, 10.89f, 6.18f};
  float sampleItemPerish[] = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1};

  for(int i = 0; i < 12; i++){
    testItems[i].name = sampleItemNames[i];
    testItems[i].cost = sampleItemPrices[i];
    testItems[i].weight = sampleItemWeights[i];
    testItems[i].perishable = sampleItemPerish[i];
  }

  //Creating a cart variable and populating it with the addItemToCart function
  Cart shoppingCart;
  shoppingCart.itemCount = 0;
  shoppingCart.bagCount = 0;
  for(int i = 0; i < 50; i++){
    addItemToCart(&testItems[(int)(rand()/(double)RAND_MAX*12)], &shoppingCart);
  }

  printf("Before packing!\n");
  displayCart(&shoppingCart);
  packCartItems(&shoppingCart);
  printf("After packing!\n");
  displayCart(&shoppingCart);
  removePerishable(&shoppingCart);
  printf("After removing perishables!\n");
  displayCart(&shoppingCart);

  for(int i = 0; i < shoppingCart.bagCount; i++){
    free(shoppingCart.bags[i]);
  }
  free(shoppingCart.bags);
}

//Takes pointers to GroceryItem and Cart, and returns 1 if adding the item to cart was successful, 0 otherwise
int addItemToCart(GroceryItem *item, Cart *cart){
  if(cart->itemCount < 100){
      cart->looseItems[cart->itemCount] = item;
      cart->itemCount++;
      return 1;
    }
  //}
  else
    return 0;
}

//Takes pointers to GroceryItem and Bag, and returns 1 if adding the item to bag was successful, 0 otherwise
int addItemToBag(GroceryItem *item, Bag *bag){
  if(bag->itemCount < MAX_BAG_ITEMS && (item->weight + bag->bagWeight) <= MAX_BAG_WT){
    bag->itemsInBag[bag->itemCount++] = item;
      bag->bagWeight += item->weight;
      return 1;
  }
  else
    return 0;
}

//Takes pointers to GroceryItem and Bag, and returns 1 if removing the item from bag was successful, 0 otherwise
int removeItemFromBag(GroceryItem *item, Bag *bag){
  if(bag->itemCount > 0){
    for(int i = 0; i < bag->itemCount; i++){
      if(strcmp(bag->itemsInBag[i]->name, item->name) == 0){
        bag->bagWeight -= item->weight;
        for(int j = i; j < bag->itemCount - 1; j++){
          bag->itemsInBag[j] = bag->itemsInBag[j + 1];
        }
        bag->itemsInBag[bag->itemCount - 1] = NULL;
        bag->itemCount--;
        return 1;
      }
    }
  }
  return 0;
}

//Takes a pointer to a GroceryItem and prints the item info in the specified format
void displayGroceryItem(GroceryItem *item){
  if(item == NULL){
    printf("NULL!\n");
    return;
  }
  printf("%s weighing %.3f kg with price $%.2f\n", item->name, item->weight, item->cost);
}

//Takes a pointer to a Bag and prints the bag info in the specified format
void displayBag(Bag *bag, int bagNumber){
  printf("BAG %d (Total Weight = %.3f kg)\n", bagNumber, bag->bagWeight);
  float heaviest = 0.0;
  for(int i = 0; i < bag->itemCount; i++){
    if(heaviest < bag->itemsInBag[i]->weight){
      heaviest = bag->itemsInBag[i]->weight;
    }
  }
  for(int i = 0; i < bag->itemCount; i++){
    if(bag->itemsInBag[i]->weight == heaviest)
      printf("\t*");
    else
      printf("\t ");
    displayGroceryItem(bag->itemsInBag[i]);
  }
}

//Takes a pointer to a Cart and prints the cart info in the specified format
void displayCart(Cart *cart){
  for(int i = 0; i < cart->itemCount; i++){
    displayGroceryItem(cart->looseItems[i]);
  }
  printf("\n");

  for(int i = 0; i < cart->bagCount; i++){
    displayBag(cart->bags[i], i + 1);
    printf("\n");
  }
}

//Takes a pointer to a Cart and packs the items in the looseItems array into the dynamically allocated bags array
void packCartItems(Cart *cart){
  //Calculating number of bags required
  int bagsRequired = 0;
  float tempBagWeight = 0.0;
  int tempBagItemCount = 0;
  int tempCartItemCount = cart->itemCount;
  while(tempCartItemCount > 0){
    if(bagsRequired == 0)
      bagsRequired++;
    if((tempBagWeight + cart->looseItems[tempCartItemCount - 1]->weight) <= MAX_BAG_WT && tempBagItemCount < MAX_BAG_ITEMS){
      tempBagWeight += cart->looseItems[tempCartItemCount - 1]->weight;
      tempBagItemCount++;
      tempCartItemCount--;
    }
    else if(cart->looseItems[tempCartItemCount - 1]->weight <= MAX_BAG_WT){
        bagsRequired++;
        tempBagWeight = 0.0;
        tempBagItemCount = 0;
        tempBagWeight += cart->looseItems[tempCartItemCount - 1]->weight;
        tempBagItemCount++;
        tempCartItemCount--;
    }
    else
      tempCartItemCount--;
  }

    cart->bagCount = bagsRequired;
    //Allocating memory to the bags array
    cart->bags = (Bag **) malloc(bagsRequired * sizeof(Bag *));
    if(cart->bags == NULL){
      printf("Malloc of packCartItems failed!\n");
      return;
    }

    //Packing the bags
    int startItemCount = tempCartItemCount = cart->itemCount;
    int i = 0;
    cart->bags[0] = (Bag *) malloc(sizeof(Bag));
    if(cart->bags[0] == NULL){
      printf("malloc of bag failed!\n");
      return;
    }
    cart->bags[0]->itemCount = 0;
    cart->bags[0]->bagWeight = 0.0;
    while(tempCartItemCount > 0){
      int result = addItemToBag(cart->looseItems[tempCartItemCount - 1], cart->bags[i]);
      if(result == 0){
        if(cart->looseItems[tempCartItemCount - 1]->weight <= MAX_BAG_WT){
          i++;
          cart->bags[i] = (Bag *) malloc(sizeof(Bag));
          if(cart->bags[i] == NULL){
            printf("malloc of bag failed!\n");
            return;
          }
          cart->bags[i]->itemCount = 0;
          cart->bags[i]->bagWeight = 0.0;
          addItemToBag(cart->looseItems[tempCartItemCount - 1], cart->bags[i]);

          cart->looseItems[tempCartItemCount - 1] = NULL;
          tempCartItemCount--;
          cart->itemCount--;
        }
        else{
          tempCartItemCount--;
        }
      }
      else{
        cart->looseItems[tempCartItemCount - 1] = NULL;
        tempCartItemCount--;
        cart->itemCount--;
      }
  }
  i = 0;
  GroceryItem *tempLooseItems[startItemCount];
  for(int j = 0; j < startItemCount; j++){
    if(cart->looseItems[j] != NULL){
      tempLooseItems[i++] = cart->looseItems[j];
    }
  }
  for(int j = 0; j < cart->itemCount; j++){
    cart->looseItems[j] = tempLooseItems[j];
  }
}

//Takes a pointer to a Cart and removes the items from looseItems and the Bags array which are perishable
void removePerishable(Cart *cart){
  for(int i = 0; i < cart->bagCount; i++){
    for(int j = 0; j < cart->bags[i]->itemCount; j++){
      if(cart->bags[i]->itemsInBag[j]->perishable){
        removeItemFromBag(cart->bags[i]->itemsInBag[j], cart->bags[i]);
        j--;
      }
    }
    if(cart->bags[i]->itemCount == 0){
      free(cart->bags[i]);
      for(int k = i; k < cart->bagCount - 1; k++){
        cart->bags[k] = cart->bags[k + 1];
      }
      cart->bagCount--;
      i--;
    }
  }

  for(int i = 0; i < cart->itemCount; i++){
    if(cart->looseItems[i]->perishable){
      for(int j = i; j < cart->itemCount - 1; j++){
        cart->looseItems[j] = cart->looseItems[j + 1];
      }
      cart->itemCount--;
      i--;
    }
  }
}
