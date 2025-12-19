#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct User {
    char username[20];
    char password[20];
};

struct Product {
    int id;
    char name[30];
    float price;
    int qty;
};

struct Product products[50];
int productCount = 0;

struct Product cart[20];
int cartCount = 0;


/* ===========================
   LOAD & SAVE PRODUCTS
   =========================== */
void loadProducts() {
    FILE *fp = fopen("products.dat", "rb");
    if(fp == NULL) {
        printf("No product file found. Starting empty.\n");
        return;
    }
    fread(&productCount, sizeof(int), 1, fp);
    fread(products, sizeof(struct Product), productCount, fp);
    fclose(fp);
}

void saveProducts() {
    FILE *fp = fopen("products.dat", "wb");
    fwrite(&productCount, sizeof(int), 1, fp);
    fwrite(products, sizeof(struct Product), productCount, fp);
    fclose(fp);
}


/* ===========================
   USER REGISTER
   =========================== */
void registerUser() {
    FILE *fp;
    struct User u;

    printf("Enter username: ");
    scanf("%s", u.username);

    printf("Enter password: ");
    scanf("%s", u.password);

    fp = fopen("users.dat", "ab");
    fwrite(&u, sizeof(struct User), 1, fp);
    fclose(fp);

    printf("User Registered.\n");
}

/* ===========================
   LOGIN
   =========================== */
int loginUser() {
    FILE *fp;
    struct User u;
    char un[20], pw[20];
    int found = 0;

    printf("Username: ");
    scanf("%s", un);

    printf("Password: ");
    scanf("%s", pw);

    fp = fopen("users.dat", "rb");
    if(fp == NULL) {
        printf("No users found!\n");
        return 0;
    }

    while(fread(&u, sizeof(struct User), 1, fp)) {
        if(strcmp(un, u.username)==0 && strcmp(pw, u.password)==0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if(found) {
        printf("Login Success!\n");
        return 1;
    } else {
        printf("Wrong username or password.\n");
        return 0;
    }
}


/* ===========================
   SHOW PRODUCTS
   =========================== */
void showProducts() {
    int i;
    printf("\n---- Product List ----\n");
    for(i=0;i<productCount;i++) {
        printf("%d. %s - Rs %.2f (Qty: %d)\n",
            products[i].id,
            products[i].name,
            products[i].price,
            products[i].qty
        );
    }
}

/* ===========================
   ADD TO CART
   =========================== */
void addToCart() {
    int id, i;
    printf("Enter Product ID to add: ");
    scanf("%d", &id);

    for(i=0;i<productCount;i++) {
        if(products[i].id == id) {
            cart[cartCount] = products[i];
            cartCount++;
            printf("Added to cart.\n");
            return;
        }
    }
    printf("Product not found.\n");
}


/* ===========================
   CHECKOUT
   =========================== */
void checkout() {
    FILE *fp;
    float total = 0;
    int i;

    printf("\n---- BILL ----\n");
    for(i=0;i<cartCount;i++) {
        printf("%s - %.2f\n", cart[i].name, cart[i].price);
        total += cart[i].price;
    }
    printf("Total Amount = %.2f\n", total);

    fp = fopen("log.txt", "a");
    fprintf(fp, "Order placed. Total = %.2f\n", total);
    fclose(fp);

    printf("Order saved to log.txt\n");
}


/* ===========================
   ADMIN ADD PRODUCT
   =========================== */
void adminAddProduct() {
    struct Product p;

    printf("Enter product id: ");
    scanf("%d", &p.id);

    printf("Enter name: ");
    scanf("%s", p.name);

    printf("Enter price: ");
    scanf("%f", &p.price);

    printf("Enter qty: ");
    scanf("%d", &p.qty);

    products[productCount] = p;
    productCount++;

    saveProducts();
    printf("Product added.\n");
}


/* ===========================
   MAIN MENU
   =========================== */
int main() {

    int ch;
    loadProducts();

    while(1) {
        printf("\n1.Register\n2.Login\n3.Admin Add Product\n4.Exit\nChoice: ");
        scanf("%d", &ch);

        if(ch == 1) registerUser();
        else if(ch == 2) {
            if(loginUser()) {
                int op;
                while(1) {
                    printf("\n1.Show Products\n2.Add to Cart\n3.Checkout\n4.Logout\nChoice: ");
                    scanf("%d", &op);

                    if(op == 1) showProducts();
                    else if(op == 2) addToCart();
                    else if(op == 3) checkout();
                    else break;
                }
            }
        }
        else if(ch == 3) adminAddProduct();
        else break;
    }

    return 0;
}
