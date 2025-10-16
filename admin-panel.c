// Admin Panel for E-Commerce System
// File: admin-panel.c
// Dedicated admin interface for shop owners and system management

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// Structure for admin/shop owner details
struct AdminDetails {
    char shop_name[100];
    char owner_name[50];
    char email[100];
    char password[100];
    char phone[15];
    char registration_date[20];
    int shop_id;
    int status; // 1=active, 0=inactive
};

// Structure for product management
struct Product {
    int product_id;
    int shop_id;
    char shop_name[100];
    char product_name[50];
    int price;
    int stock;
    int sold_quantity;
    float total_revenue;
    char date_added[20];
};

// Structure for sales analytics
struct SalesReport {
    char shop_name[100];
    int total_products;
    int total_sold;
    float total_revenue;
    char last_sale_date[20];
};

// Global variables
struct AdminDetails current_admin;
int logged_in = 0;

// Function prototypes
void display_admin_banner();
void clear_screen();
void admin_login();
void admin_registration();
void admin_dashboard();
void product_management();
void add_new_product();
void view_my_products();
void edit_product();
void delete_product();
void sales_analytics();
void view_all_orders();
void revenue_report();
void sync_with_main_system();
void loading_animation(char* message);
int validate_admin_input();
void hash_password(char* password, char* hashed);
int get_next_shop_id();
void save_admin_to_file();
int verify_admin_login();
void update_products_dat();

// Main function
int main() {
    display_admin_banner();
    
    while(1) {
        if(!logged_in) {
            clear_screen();
            printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
            printf("\t║                   🏢 E-COMMERCE ADMIN PANEL 🏢                       ║\n");
            printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
            printf("\t║                                                                      ║\n");
            printf("\t║          Welcome to Shop Owner & Admin Management System             ║\n");
            printf("\t║                                                                      ║\n");
            printf("\t║     [1] 🔐 Admin Login     - Access your shop dashboard              ║\n");
            printf("\t║     [2] 📝 Register Shop   - Create new shop account                 ║\n");
            printf("\t║     [3] 🚪 Exit System     - Close admin panel                       ║\n");
            printf("\t║                                                                      ║\n");
            printf("\t╚══════════════════════════════════════════════════════════════════════╝\n");
            
            printf("\n\t🎯 Choose your option (1-3): ");
            int choice;
            scanf("%d", &choice);
            while(getchar()!='\n'); // clear input buffer
            
            switch(choice) {
                case 1:
                    admin_login();
                    break;
                case 2:
                    admin_registration();
                    break;
                case 3:
                    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
                    printf("\t║                        👋 GOODBYE ADMIN!                             ║\n");
                    printf("\t║                   Thanks for managing the system! 🛍️                  ║\n");
                    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
                    return 0;
                default:
                    printf("\n\t❌ Invalid choice! Please try again.\n");
                    printf("\t⏳ Press Enter to continue...");
                    getchar();
                    break;
            }
        } else {
            admin_dashboard();
        }
    }
    
    return 0;
}

// Utility functions
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void display_admin_banner() {
    clear_screen();
    printf("\n\n");
    printf("\t╔════════════════════════════════════════════════════════════╗\n");
    printf("\t║    🏢🏢🏢  E-COMMERCE ADMIN MANAGEMENT SYSTEM  🏢🏢🏢      ║\n");
    printf("\t║                                                            ║\n");
    printf("\t║            ✨ Empowering Shop Owners Since 2025 ✨         ║\n");
    printf("\t║                                                            ║\n");
    printf("\t║        📊 Manage • 📈 Analyze • 💰 Profit • 🚀 Grow        ║\n");
    printf("\t╚════════════════════════════════════════════════════════════╝\n");
    printf("\n\t⏳ Initializing admin panel... Please wait...\n");
    
    for(int i = 0; i < 25; i++) {
        printf("█");
        fflush(stdout);
        usleep(80000);
    }
    printf(" ✅ Ready!\n\n");
    printf("\t🎉 Welcome to the Admin Control Center! 🎉\n");
    printf("\t⏳ Press Enter to continue...");
    getchar();
}

void loading_animation(char* message) {
    printf("\n\t🔄 %s", message);
    for(int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        usleep(400000);
    }
    printf(" ✅ Done!\n");
}

// Simple password hashing - converts to hex for storage
void hash_password(char* password, char* hashed) {
    char key[] = "admin_secret_key_2025";
    int key_len = strlen(key);
    int pass_len = strlen(password);
    char temp_hash[100];

    // Generate binary hash
    for (int i = 0; i < pass_len; i++) {
        temp_hash[i] = password[i] ^ key[i % key_len];
    }
    temp_hash[pass_len] = '\0';
    
    // Convert to hex representation for storage
    strcpy(hashed, "");
    for (int i = 0; i < pass_len; i++) {
        char hex[3];
        sprintf(hex, "%02x", (unsigned char)temp_hash[i]);
        strcat(hashed, hex);
    }
}

// Get next available shop ID
int get_next_shop_id() {
    FILE* fp = fopen("admin_accounts.dat", "r");
    int max_id = 0;
    
    if(fp != NULL) {
        char line[200];
        while(fgets(line, sizeof(line), fp)) {
            if(strncmp(line, "shop_id:", 8) == 0) {
                int id = atoi(line + 9);
                if(id > max_id) max_id = id;
            }
        }
        fclose(fp);
    }
    
    return max_id + 1;
}

// Admin registration function
void admin_registration() {
    clear_screen();
    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                       📝 SHOP REGISTRATION                           ║\n");
    printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
    printf("\t║           Register your shop and start selling online! 🏪            ║\n");
    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");

    struct AdminDetails new_admin;
    char password[100], confirm_password[100];
    
   // while(getchar() != '\n');
    
    printf("\t🏪 Enter Shop Name: ");
    fgets(new_admin.shop_name, sizeof(new_admin.shop_name), stdin);
    new_admin.shop_name[strcspn(new_admin.shop_name, "\n")] = 0;
    
    printf("\t👤 Enter Owner Name: ");
    fgets(new_admin.owner_name, sizeof(new_admin.owner_name), stdin);
    new_admin.owner_name[strcspn(new_admin.owner_name, "\n")] = 0;
    
    printf("\t📧 Enter Email: ");
    scanf("%s", new_admin.email);
    
    printf("\t🔒 Enter Password (min 6 characters): ");
    scanf("%s", password);
    
    printf("\t🔒 Confirm Password: ");
    scanf("%s", confirm_password);
    
    printf("\t📱 Enter Phone Number: ");
    scanf("%s", new_admin.phone);
    
    // Validation
    if(strcmp(password, confirm_password) != 0) {
        printf("\n\t❌ Passwords don't match!\n");
        printf("\t⏳ Press Enter to try again...");
        getchar();
        return;
    }
    
    if(strlen(password) < 6) {
        printf("\n\t❌ Password must be at least 6 characters!\n");
        printf("\t⏳ Press Enter to try again...");
        getchar();
        return;
    }
    
    // Set additional fields
    new_admin.shop_id = get_next_shop_id();
    new_admin.status = 1; // Active
    hash_password(password, new_admin.password);
    
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(new_admin.registration_date, "%02d-%02d-%04d", 
            tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    
    // Save to file
    FILE* fp = fopen("admin_accounts.dat", "a");
    if(fp == NULL) {
        printf("\n\t❌ Error creating account!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    fprintf(fp, "shop_id: %d\n", new_admin.shop_id);
    fprintf(fp, "shop_name: %s\n", new_admin.shop_name);
    fprintf(fp, "owner_name: %s\n", new_admin.owner_name);
    fprintf(fp, "email: %s\n", new_admin.email);
    fprintf(fp, "password: %s\n", new_admin.password);
    fprintf(fp, "phone: %s\n", new_admin.phone);
    fprintf(fp, "registration_date: %s\n", new_admin.registration_date);
    fprintf(fp, "status: %d\n", new_admin.status);
    fprintf(fp, "---\n");
    fclose(fp);
    
    clear_screen();
    printf("\n\n\t🎉 SHOP REGISTRATION SUCCESSFUL! 🎉\n\n");
    printf("\t╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                        🏪 SHOP DETAILS                               ║\n");
    printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
    
    // Truncate long names to fit in the box
    char display_shop[45], display_owner[45], display_email[45];
    
    if(strlen(new_admin.shop_name) > 42) {
        strncpy(display_shop, new_admin.shop_name, 39);
        display_shop[39] = '\0';
        strcat(display_shop, "...");
    } else {
        strcpy(display_shop, new_admin.shop_name);
    }
    
    if(strlen(new_admin.owner_name) > 42) {
        strncpy(display_owner, new_admin.owner_name, 39);
        display_owner[39] = '\0';
        strcat(display_owner, "...");
    } else {
        strcpy(display_owner, new_admin.owner_name);
    }
    
    if(strlen(new_admin.email) > 42) {
        strncpy(display_email, new_admin.email, 39);
        display_email[39] = '\0';
        strcat(display_email, "...");
    } else {
        strcpy(display_email, new_admin.email);
    }
    
    printf("\t║  Shop ID: %-58d ║\n", new_admin.shop_id);
    printf("\t║  Shop Name: %-54s   ║\n", display_shop);
    printf("\t║  Owner: %-58s   ║\n", display_owner);
    printf("\t║  Email: %-58s   ║\n", display_email);
    printf("\t║  Registration Date: %-48s ║\n", new_admin.registration_date);
    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n");
    printf("\n\t✅ Your shop is now ready to start selling!\n");
    printf("\t🚀 You are now automatically logged in and ready to manage your shop!\n");
    
    // Auto login after successful registration
    current_admin = new_admin;
    logged_in = 1;
    
    printf("\n\t⏳ Press Enter to continue...");
    getchar();
}

// Admin login function
void admin_login() {
    clear_screen();
    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                      🔐 ADMIN LOGIN                                  ║\n");
    printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
    printf("\t║                Welcome back! Please sign in 👋                       ║\n");
    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
    
    char email[100], password[100];
    
    printf("\t📧 Enter Email: ");
    scanf("%s", email);
    
    // Clear input buffer
    while(getchar() != '\n');
    
    printf("\t🔒 Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline
    
    printf("\n\t🔄 Verifying credentials...");
    fflush(stdout);
    
    FILE* fp = fopen("admin_accounts.dat", "r");
    if(fp == NULL) {
        printf("\n\t❌ No admin accounts found! Please register first.\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    char line[200];
    char file_email[100], file_password[100];
    int found = 0;
    char hashed_input[100];
    
    hash_password(password, hashed_input);
    
    while(fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "shop_id:") == 0) {
            current_admin.shop_id = atoi(value);
        } else if(strcmp(key, "shop_name:") == 0) {
            strcpy(current_admin.shop_name, value);
        } else if(strcmp(key, "owner_name:") == 0) {
            strcpy(current_admin.owner_name, value);
        } else if(strcmp(key, "email:") == 0) {
            strcpy(file_email, value);
        } else if(strcmp(key, "password:") == 0) {
            strcpy(file_password, value);
        } else if(strcmp(key, "phone:") == 0) {
            strcpy(current_admin.phone, value);
        } else if(strcmp(key, "registration_date:") == 0) {
            strcpy(current_admin.registration_date, value);
        } else if(strcmp(key, "status:") == 0) {
            current_admin.status = atoi(value);
        } else if(strcmp(key, "---") == 0) {
            if(strcmp(file_email, email) == 0 && strcmp(file_password, hashed_input) == 0) {
                if(current_admin.status == 1) {
                    printf(" ✅ Login successful!\n");
                    strcpy(current_admin.email, email);
                    strcpy(current_admin.password, file_password);
                    found = 1;
                    logged_in = 1;
                    break;
                } else {
                    printf(" ❌ Account inactive!\n");
                    printf("\n\t❌ Account is inactive! Contact system administrator.\n");
                    printf("\t⏳ Press Enter to continue...");
                    getchar();
                    fclose(fp);
                    return;
                }
            }
        }
    }
    fclose(fp);
    
    if(found) {
        clear_screen();
        printf("\n\n\t🎉 LOGIN SUCCESSFUL! 🎉\n");
        printf("\t✨ Welcome back, %s! ✨\n\n", current_admin.owner_name);
        printf("\t🏪 Shop: %s\n", current_admin.shop_name);
        printf("\t🆔 Shop ID: %d\n", current_admin.shop_id);
        
        loading_animation("Loading admin dashboard");
        printf("\n\t⏳ Press Enter to continue...");
        getchar();
    } else {
        printf(" ❌ Invalid credentials!\n");
        printf("\n\t❌ Invalid email or password!\n");
        printf("\t⏳ Press Enter to try again...");
        getchar();
    }
}

// Admin dashboard
void admin_dashboard() {
    while(logged_in) {
        clear_screen();
        
        // Calculate padding for shop name to center it in the header
        char header_title[100];
        sprintf(header_title, "🏪 ADMIN DASHBOARD");
        int header_len = strlen(header_title);
        int header_padding = (70 - header_len) / 2; // 70 is the box width minus borders
        
        printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
        printf("\t║ %*s%s%*s ║\n", header_padding, "", header_title, 70 - header_len - header_padding, "");
        printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
        
        // Truncate long shop names to fit in the box
        char display_shop[45], display_owner[44];
        if(strlen(current_admin.shop_name) > 42) {
            strncpy(display_shop, current_admin.shop_name, 39);
            display_shop[39] = '\0';
            strcat(display_shop, "...");
        } else {
            strcpy(display_shop, current_admin.shop_name);
        }
        
        if(strlen(current_admin.owner_name) > 41) {
            strncpy(display_owner, current_admin.owner_name, 38);
            display_owner[38] = '\0';
            strcat(display_owner, "...");
        } else {
            strcpy(display_owner, current_admin.owner_name);
        }
        
        printf("\t║  Shop    : %-45s             ║\n", display_shop);
        printf("\t║  Owner   : %-44s              ║\n", display_owner);
        printf("\t║  Shop ID : %-39d                   ║\n", current_admin.shop_id);
        printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
        
        printf("\t╔══════════════════════════════════════════════════════════════════════╗\n");
        printf("\t║                           📋 ADMIN MENU                              ║\n");
        printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
        printf("\t║                                                                      ║\n");
        printf("\t║  [1] 📦 Product Management  - Add/Edit/Delete products               ║\n");
        printf("\t║  [2] 📊 Sales Analytics     - View sales reports & revenue           ║\n");
        printf("\t║  [3] 🛒 Order Management    - View customer orders                   ║\n");
        printf("\t║  [4] 💰 Revenue Report      - Detailed financial analysis            ║\n");
        printf("\t║  [5] 🔄 Sync with System    - Update main e-shop database            ║\n");
        printf("\t║  [6] 🚪 Logout              - Exit admin panel                       ║\n");
        printf("\t║                                                                      ║\n");
        printf("\t╚══════════════════════════════════════════════════════════════════════╝\n");
        
        printf("\n\t🎯 Choose your option (1-6): ");
        int choice;
        scanf("%d", &choice);
        while(getchar()!='\n'); // clear input buffer
        
        switch(choice) {
            case 1:
                product_management();
                break;
            case 2:
                sales_analytics();
                break;
            case 3:
                view_all_orders();
                break;
            case 4:
                revenue_report();
                break;
            case 5:
                sync_with_main_system();
                break;
            case 6:
                logged_in = 0;
                printf("\n\t👋 Logged out successfully!\n");
                printf("\t⏳ Press Enter to continue...");
                getchar();
                break;
            default:
                printf("\n\t❌ Invalid choice! Please try again.\n");
                printf("\t⏳ Press Enter to continue...");
                getchar();
                break;
        }
    }
}

// Product management menu
void product_management() {
    while(1) {
        clear_screen();
        
        // Truncate shop name if too long for the header
        char display_shop[50];
        if(strlen(current_admin.shop_name) > 45) {
            strncpy(display_shop, current_admin.shop_name, 42);
            display_shop[42] = '\0';
            strcat(display_shop, "...");
        } else {
            strcpy(display_shop, current_admin.shop_name);
        }
        
        printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
        printf("\t║                       📦 PRODUCT MANAGEMENT                          ║\n");
        printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
        printf("\t║  Shop: %-61s ║\n", display_shop);
        printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
        
        printf("\t╔══════════════════════════════════════════════════════════════════════╗\n");
        printf("\t║                         📋 PRODUCT MENU                              ║\n");
        printf("\t╠══════════════════════════════════════════════════════════════════════╣\n");
        printf("\t║                                                                      ║\n");
        printf("\t║  [1] ➕ Add New Product     - Add products to your shop              ║\n");
        printf("\t║  [2] 👁️  View My Products    - See all your products                  ║\n");
        printf("\t║  [3] ✏️  Edit Product        - Modify existing products               ║\n");
        printf("\t║  [4] 🗑️  Delete Product      - Remove products from shop              ║\n");
        printf("\t║  [5] 🔙 Back to Dashboard   - Return to main menu                    ║\n");
        printf("\t║                                                                      ║\n");
        printf("\t╚══════════════════════════════════════════════════════════════════════╝\n");
        
        printf("\n\t🎯 Choose your option (1-5): ");
        int choice;
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                add_new_product();
                break;
            case 2:
                view_my_products();
                break;
            case 3:
                edit_product();
                break;
            case 4:
                delete_product();
                break;
            case 5:
                return;
            default:
                printf("\n\t❌ Invalid choice! Please try again.\n");
                printf("\t⏳ Press Enter to continue...");
                while(getchar()!='\n'); // clear input buffer
                getchar();
                break;
        }
    }
}

// Add new product function
void add_new_product() {
    clear_screen();
    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                      ➕ ADD NEW PRODUCT                              ║\n");
    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
    
    struct Product new_product;
    
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(new_product.date_added, "%02d-%02d-%04d", 
            tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    
    new_product.shop_id = current_admin.shop_id;
    strcpy(new_product.shop_name, current_admin.shop_name);
    new_product.sold_quantity = 0;
    new_product.total_revenue = 0.0;
    
    while(getchar() != '\n');
    
    printf("\t📦 Enter Product Name: ");
    fgets(new_product.product_name, sizeof(new_product.product_name), stdin);
    new_product.product_name[strcspn(new_product.product_name, "\n")] = 0;
    
    printf("\t💰 Enter Price (Taka): ");
    scanf("%d", &new_product.price);
    
    printf("\t📊 Enter Initial Stock Quantity: ");
    scanf("%d", &new_product.stock);
    
    if(new_product.price <= 0 || new_product.stock < 0) {
        printf("\n\t❌ Invalid price or stock quantity!\n");
        printf("\t⏳ Press Enter to try again...");
        getchar();
        return;
    }
    
    // Generate product ID
    new_product.product_id = current_admin.shop_id * 1000 + (rand() % 1000);
    
    // Save product to file
    FILE* fp = fopen("admin_products.dat", "a");
    if(fp == NULL) {
        printf("\n\t❌ Error saving product!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    fprintf(fp, "product_id: %d\n", new_product.product_id);
    fprintf(fp, "shop_id: %d\n", new_product.shop_id);
    fprintf(fp, "shop_name: %s\n", new_product.shop_name);
    fprintf(fp, "product_name: %s\n", new_product.product_name);
    fprintf(fp, "price: %d\n", new_product.price);
    fprintf(fp, "stock: %d\n", new_product.stock);
    fprintf(fp, "sold_quantity: %d\n", new_product.sold_quantity);
    fprintf(fp, "total_revenue: %.2f\n", new_product.total_revenue);
    fprintf(fp, "date_added: %s\n", new_product.date_added);
    fprintf(fp, "---\n");
    fclose(fp);
    
    printf("\n\t✅ Product added successfully!\n");
    printf("\t🆔 Product ID: %d\n", new_product.product_id);
    printf("\t📦 Product: %s\n", new_product.product_name);
    printf("\t💰 Price: %d Taka\n", new_product.price);
    printf("\t📊 Stock: %d units\n", new_product.stock);
    
    printf("\n\t💡 Don't forget to sync with main system to make it visible to customers!\n");
    printf("\t⏳ Press Enter to continue...");
    getchar();
}

// View products function
void view_my_products() {
    clear_screen();
    printf("\n\n\t╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                             👁️  MY PRODUCTS                                 ║\n");
    printf("\t╚════════════════════════════════════════════════════════════════════════════╝\n\n");
    
    FILE* fp = fopen("admin_products.dat", "r");
    if(fp == NULL) {
        printf("\t😔 No products found! Add some products first.\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    printf("\t📦 Products for Shop: %s\n", current_admin.shop_name);
    printf("\t───────────────────────────────────────────────────────────────────────────────\n");
    printf("\t%-6s %-32s %-12s %-8s %-8s %-12s\n", "ID", "Product Name", "Price", "Stock", "Sold", "Revenue");
    printf("\t───────────────────────────────────────────────────────────────────────────────\n");
    
    char line[200];
    struct Product product;
    int found = 0;
    
    while(fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "product_id:") == 0) {
            product.product_id = atoi(value);
        } else if(strcmp(key, "shop_id:") == 0) {
            product.shop_id = atoi(value);
        } else if(strcmp(key, "product_name:") == 0) {
            strcpy(product.product_name, value);
        } else if(strcmp(key, "price:") == 0) {
            product.price = atoi(value);
        } else if(strcmp(key, "stock:") == 0) {
            product.stock = atoi(value);
        } else if(strcmp(key, "sold_quantity:") == 0) {
            product.sold_quantity = atoi(value);
        } else if(strcmp(key, "total_revenue:") == 0) {
            product.total_revenue = atof(value);
        } else if(strcmp(key, "---") == 0) {
            if(product.shop_id == current_admin.shop_id) {
                printf("\t%-6d %-32s %-12d %-8d %-8d %-12.2f\n", 
                       product.product_id, product.product_name, 
                       product.price, product.stock, 
                       product.sold_quantity, product.total_revenue);
                found = 1;
            }
        }
    }
    
    if(!found) {
        printf("\t😔 No products found for your shop.\n");
    }
    
    fclose(fp);
    printf("\t───────────────────────────────────────────────────────────────────────────────\n");
    printf("\n\t⏳ Press Enter to continue...");
    while(getchar()!='\n'); // clear input buffer
    getchar();
}

// Edit product function
void edit_product() {
    clear_screen();
    printf("\n\n\t╔═════════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                            ✏️  EDIT PRODUCT                              ║\n");
    printf("\t╚═════════════════════════════════════════════════════════════════════════╝\n\n");
    
    // First show current products
    printf("\t📦 Your Current Products:\n");
    printf("\t──────────────────────────────────────────────────────────────────────────\n");
    
    FILE* fp = fopen("admin_products.dat", "r");
    if(fp == NULL) {
        printf("\t😔 No products found! Add some products first.\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    struct Product products[100];
    int product_count = 0;
    char line[200];
    struct Product temp_product;
    
    // Load products for this shop
    while(fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "product_id:") == 0) {
            temp_product.product_id = atoi(value);
        } else if(strcmp(key, "shop_id:") == 0) {
            temp_product.shop_id = atoi(value);
        } else if(strcmp(key, "product_name:") == 0) {
            strcpy(temp_product.product_name, value);
        } else if(strcmp(key, "price:") == 0) {
            temp_product.price = atoi(value);
        } else if(strcmp(key, "stock:") == 0) {
            temp_product.stock = atoi(value);
        } else if(strcmp(key, "---") == 0) {
            if(temp_product.shop_id == current_admin.shop_id) {
                products[product_count] = temp_product;
                printf("\t[%d] %-32s - Price: %12d Tk,   Stock: %d\n", 
                       product_count + 1, temp_product.product_name, temp_product.price, temp_product.stock);
                product_count++;
            }
        }
    }
    fclose(fp);
    
    if(product_count == 0) {
        printf("\t😔 No products found for your shop.\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    printf("\t──────────────────────────────────────────────────────────────────────────\n");
    printf("\t🎯 Choose an option:\n");
    printf("\t   [1-%d] Select product number to edit\n", product_count);
    printf("\t   [0] ⬅️  Go back to main menu\n");
    printf("\t📝 Enter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    if(choice == 0) {
        printf("\n\t↩️  Returning to main menu...\n");
        sleep(1);
        return;
    }
    
    if(choice < 1 || choice > product_count) {
        printf("\n\t❌ Invalid product number!\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    struct Product *selected = &products[choice - 1];
    
    printf("\n\t📝 Editing: %s\n", selected->product_name);
    printf("\t──────────────────────────────────────────────────────────────────────\n");
    
    char new_name[50];
    int new_price, new_stock;
    
    while(getchar() != '\n');
    
    printf("\t📦 Current Name: %s\n", selected->product_name);
    printf("\t📦 Enter New Name (or press Enter to keep current): ");
    fgets(new_name, sizeof(new_name), stdin);
    new_name[strcspn(new_name, "\n")] = 0;
    
    if(strlen(new_name) > 0) {
        strcpy(selected->product_name, new_name);
    }
    
    printf("\t💰 Current Price: %d Tk\n", selected->price);
    printf("\t💰 Enter New Price (or 0 to keep current): ");
    scanf("%d", &new_price);
    
    if(new_price > 0) {
        selected->price = new_price;
    }
    
    printf("\t📊 Current Stock: %d\n", selected->stock);
    printf("\t📊 Enter New Stock (or -1 to keep current): ");
    scanf("%d", &new_stock);
    
    if(new_stock >= 0) {
        selected->stock = new_stock;
    }
    
    // Update the file
    FILE* fp_read = fopen("admin_products.dat", "r");
    FILE* fp_write = fopen("admin_products_temp.dat", "w");
    
    if(fp_read == NULL || fp_write == NULL) {
        printf("\n\t❌ Error updating product!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    // Copy all products, updating the selected one
    while(fgets(line, sizeof(line), fp_read)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "product_id:") == 0) {
            temp_product.product_id = atoi(value);
            fprintf(fp_write, "%s", line);
        } else if(strcmp(key, "shop_id:") == 0) {
            temp_product.shop_id = atoi(value);
            fprintf(fp_write, "%s", line);
        } else if(strcmp(key, "product_name:") == 0) {
            if(temp_product.product_id == selected->product_id) {
                fprintf(fp_write, "product_name: %s\n", selected->product_name);
            } else {
                fprintf(fp_write, "%s", line);
            }
        } else if(strcmp(key, "price:") == 0) {
            if(temp_product.product_id == selected->product_id) {
                fprintf(fp_write, "price: %d\n", selected->price);
            } else {
                fprintf(fp_write, "%s", line);
            }
        } else if(strcmp(key, "stock:") == 0) {
            if(temp_product.product_id == selected->product_id) {
                fprintf(fp_write, "stock: %d\n", selected->stock);
            } else {
                fprintf(fp_write, "%s", line);
            }
        } else {
            fprintf(fp_write, "%s", line);
        }
    }
    
    fclose(fp_read);
    fclose(fp_write);
    
    // Replace original file
    remove("admin_products.dat");
    rename("admin_products_temp.dat", "admin_products.dat");
    
    printf("\n\t✅ Product updated successfully!\n");
    printf("\t📦 Product: %s\n", selected->product_name);
    printf("\t💰 Price: %d Taka\n", selected->price);
    printf("\t📊 Stock: %d units\n", selected->stock);
    
    printf("\n\t⏳ Press Enter to continue...");
    while(getchar()!='\n'); // clear input buffer
    getchar();
}

// Delete product function  
void delete_product() {
    clear_screen();
    printf("\n\n\t╔═════════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                            🗑️ DELETE PRODUCT                             ║\n");
    printf("\t╚═════════════════════════════════════════════════════════════════════════╝\n\n");
    
    // Show current products
    printf("\t📦 Your Current Products:\n");
    printf("\t────────────────────────────────────────────────────────────────────────────\n");
    
    FILE* fp = fopen("admin_products.dat", "r");
    if(fp == NULL) {
        printf("\t😔 No products found!\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    struct Product products[100];
    int product_count = 0;
    char line[200];
    struct Product temp_product;
    
    // Load products for this shop
    while(fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "product_id:") == 0) {
            temp_product.product_id = atoi(value);
        } else if(strcmp(key, "shop_id:") == 0) {
            temp_product.shop_id = atoi(value);
        } else if(strcmp(key, "product_name:") == 0) {
            strcpy(temp_product.product_name, value);
        } else if(strcmp(key, "price:") == 0) {
            temp_product.price = atoi(value);
        } else if(strcmp(key, "stock:") == 0) {
            temp_product.stock = atoi(value);
        } else if(strcmp(key, "---") == 0) {
            if(temp_product.shop_id == current_admin.shop_id) {
                products[product_count] = temp_product;
                printf("\t[%d] %-32s - Price: %12d Tk,   Stock: %d\n", 
                       product_count + 1, temp_product.product_name, temp_product.price, temp_product.stock);
                product_count++;
            }
        }
    }
    fclose(fp);
    
    if(product_count == 0) {
        printf("\t😔 No products found for your shop.\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    printf("\t───────────────────────────────────────────────────────────────────────────\n");
    printf("\t🎯 Choose an option:\n");
    printf("\t   [1-%d] Select product number to delete\n", product_count);
    printf("\t   [0] ⬅️  Go back to main menu\n");
    printf("\t📝 Enter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    if(choice == 0) {
        printf("\n\t↩️  Returning to main menu...\n");
        sleep(1);
        return;
    }
    
    if(choice < 1 || choice > product_count) {
        printf("\n\t❌ Invalid choice! Please try again.\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    struct Product *selected = &products[choice - 1];
    
    printf("\n\t⚠️  Are you sure you want to delete '%s'? (y/n): ", selected->product_name);
    char confirm;
    scanf(" %c", &confirm);
    
    if(confirm != 'y' && confirm != 'Y') {
        printf("\n\t❌ Deletion cancelled.\n");
        printf("\t⏳ Press Enter to continue...");
        while(getchar()!='\n'); // clear input buffer
        getchar();
        return;
    }
    
    // Delete from file
    FILE* fp_read = fopen("admin_products.dat", "r");
    FILE* fp_write = fopen("admin_products_temp.dat", "w");
    
    if(fp_read == NULL || fp_write == NULL) {
        printf("\n\t❌ Error deleting product!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    int skip_product = 0;
    
    // Copy all products except the selected one
    while(fgets(line, sizeof(line), fp_read)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "product_id:") == 0) {
            temp_product.product_id = atoi(value);
            if(temp_product.product_id == selected->product_id) {
                skip_product = 1;
            } else {
                skip_product = 0;
            }
        }
        
        if(!skip_product) {
            fprintf(fp_write, "%s", line);
        }
    }
    
    fclose(fp_read);
    fclose(fp_write);
    
    // Replace original file
    remove("admin_products.dat");
    rename("admin_products_temp.dat", "admin_products.dat");
    
    printf("\n\t✅ Product '%s' deleted successfully!\n", selected->product_name);
    printf("\t⏳ Press Enter to continue...");
    while(getchar()!='\n'); // clear input buffer
    getchar();
}

// Sync with main system - only current admin's shop
void sync_with_main_system() {
    clear_screen();
    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                    🔄 SYNC WITH MAIN SYSTEM                          ║\n");
    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
    
    loading_animation("Checking admin products database");
    
    FILE *admin_file = fopen("admin_products.dat", "r");
    if (admin_file == NULL) {
        printf("\t❌ No admin products found to sync.\n");
        printf("\n\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    // Read and collect only current admin's products
    struct Product admin_products[20];
    int admin_count = 0;
    char admin_line[200];
    struct Product temp_product;
    
    printf("\t🔍 Looking for products for shop: %s (ID: %d)\n", current_admin.shop_name, current_admin.shop_id);
    
    while(fgets(admin_line, sizeof(admin_line), admin_file)) {
        char key[50], value[150];
        sscanf(admin_line, "%s %[^\n]", key, value);
        
        if(strcmp(key, "product_id:") == 0) {
            temp_product.product_id = atoi(value);
        } else if(strcmp(key, "shop_id:") == 0) {
            temp_product.shop_id = atoi(value);
        } else if(strcmp(key, "shop_name:") == 0) {
            strcpy(temp_product.shop_name, value);
        } else if(strcmp(key, "product_name:") == 0) {
            strcpy(temp_product.product_name, value);
        } else if(strcmp(key, "price:") == 0) {
            temp_product.price = atoi(value);
        } else if(strcmp(key, "stock:") == 0) {
            temp_product.stock = atoi(value);
        } else if(strcmp(key, "---") == 0) {
            // Only collect products that belong to current admin's shop
            if(strcmp(temp_product.shop_name, current_admin.shop_name) == 0) {
                admin_products[admin_count] = temp_product;
                printf("\t   ✓ Found: %s (Price: %d, Stock: %d)\n", 
                       temp_product.product_name, temp_product.price, temp_product.stock);
                admin_count++;
            }
        }
    }
    fclose(admin_file);
    
    printf("\t📦 Total products to sync for %s: %d\n", current_admin.shop_name, admin_count);
    
    loading_animation("Updating main products.dat file");
    
    // Read main products.dat and update only current admin's shop
    FILE *main_file = fopen("products.dat", "r");
    FILE *temp_file = fopen("temp_products.dat", "w");
    
    if (main_file == NULL || temp_file == NULL) {
        printf("\t❌ Error accessing main products database.\n");
        printf("\n\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    char line[500];
    int synced_products = 0;
    int shop_found = 0;
    int in_target_shop = 0;
    
    // Read through the file line by line
    while (fgets(line, sizeof(line), main_file)) {
        char key[50], value[150];
        
        if (sscanf(line, "%[^:]: %[^\n]", key, value) == 2) {
            if (strcmp(key, "shop") == 0) {
                if (strcmp(value, current_admin.shop_name) == 0) {
                    // Found current admin's shop - replace with admin data
                    shop_found = 1;
                    in_target_shop = 1;
                    fprintf(temp_file, "shop: %s\n", current_admin.shop_name);
                    
                    // Add admin products in e-shop format
                    for (int i = 0; i < admin_count && i < 6; i++) {
                        fprintf(temp_file, "item%d: %s\n", i+1, admin_products[i].product_name);
                        fprintf(temp_file, "price%d: %d\n", i+1, admin_products[i].price);
                        fprintf(temp_file, "quantity%d: %d\n", i+1, admin_products[i].stock);
                    }
                    synced_products = admin_count;
                    printf("\t   ✓ Updated shop: %s with %d products\n", current_admin.shop_name, admin_count);
                } else {
                    // Different shop - keep as is
                    in_target_shop = 0;
                    fprintf(temp_file, "%s", line);
                }
            } else if (strcmp(line, "---\n") == 0) {
                // End of shop section
                if (in_target_shop) {
                    fprintf(temp_file, "---\n");
                    in_target_shop = 0;
                } else {
                    fprintf(temp_file, "%s", line);
                }
            } else {
                // Regular line - only copy if not in target shop
                if (!in_target_shop) {
                    fprintf(temp_file, "%s", line);
                }
            }
        } else {
            // Line doesn't match key:value format (like --- or empty lines)
            if (!in_target_shop) {
                fprintf(temp_file, "%s", line);
            } else if (strcmp(line, "---\n") == 0) {
                fprintf(temp_file, "---\n");
                in_target_shop = 0;
            }
        }
    }
    
    // If shop not found, add as new shop
    if (!shop_found && admin_count > 0) {
        fprintf(temp_file, "shop: %s\n", current_admin.shop_name);
        for (int i = 0; i < admin_count && i < 6; i++) {
            fprintf(temp_file, "item%d: %s\n", i+1, admin_products[i].product_name);
            fprintf(temp_file, "price%d: %d\n", i+1, admin_products[i].price);
            fprintf(temp_file, "quantity%d: %d\n", i+1, admin_products[i].stock);
        }
        fprintf(temp_file, "---\n");
        synced_products = admin_count;
        printf("\t   ✓ Added new shop: %s with %d products\n", current_admin.shop_name, admin_count);
    }
    
    fclose(main_file);
    fclose(temp_file);
    
    // Replace original file
    remove("products.dat");
    rename("temp_products.dat", "products.dat");
    
    loading_animation("Synchronizing inventory data");
    
    printf("\n\t✅ Successfully synchronized %s with main e-shop system!\n", current_admin.shop_name);
    printf("\t🛍️  %d products from %s are now visible to customers.\n", synced_products, current_admin.shop_name);
    printf("\t📊 Only your shop's inventory data has been updated.\n");
    printf("\t🔄 Changes will appear in main e-shop immediately.\n");
    
    printf("\n\t⏳ Press Enter to continue...");
    getchar();
}

// Sales analytics function
void sales_analytics() {
    clear_screen();
    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                         📊 SALES ANALYTICS                           ║\n");
    printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("\t📈 Sales Summary for: %s\n", current_admin.shop_name);
    printf("\t──────────────────────────────────────────────────────────────────────\n");
    
    // Read order history to calculate real analytics
    FILE* fp = fopen("order_history.dat", "r");
    if(fp == NULL) {
        printf("\t😔 No order history found.\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    int total_orders = 0;
    int total_items_sold = 0;
    float total_revenue = 0.0;
    char line[200];
    
    // Track current order data
    char current_user[50] = "";
    char current_order_id[30] = "";
    char current_date[20] = "";
    float current_order_total = 0.0;
    int current_order_items = 0;
    int order_has_my_products = 0;
    
    while(fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        
        // Skip empty lines and comments
        if(strlen(line) <= 1 || line[0] == '/' || line[0] == '#') {
            continue;
        }
        
        if(sscanf(line, "%s %[^\n]", key, value) != 2) {
            continue;
        }
        
        if(strcmp(key, "user:") == 0) {
            // Start of new order - reset tracking
            strcpy(current_user, value);
            order_has_my_products = 0;
            current_order_total = 0.0;
            current_order_items = 0;
            strcpy(current_order_id, "");
            strcpy(current_date, "");
        } 
        else if(strcmp(key, "order_id:") == 0) {
            strcpy(current_order_id, value);
        }
        else if(strcmp(key, "date:") == 0) {
            strcpy(current_date, value);
        }
        else if(strcmp(key, "item:") == 0) {
            // Parse item: "Product Name x2 = 1000"
            char item_name[100];
            int quantity = 0;
            float subtotal = 0.0;
            
            // Extract item name (everything before " x")
            char *x_pos = strstr(value, " x");
            if(x_pos != NULL) {
                int name_len = x_pos - value;
                if(name_len > 0 && name_len < 100) {
                    strncpy(item_name, value, name_len);
                    item_name[name_len] = '\0';
                    
                    // Extract quantity and subtotal
                    if(sscanf(x_pos, " x%d = %f", &quantity, &subtotal) == 2) {
                        // Sanity check for reasonable values
                        if(quantity > 0 && quantity <= 1000 && subtotal > 0 && subtotal <= 1000000) {
                            // Check if this item belongs to current admin's shop
                            FILE* fp_products = fopen("admin_products.dat", "r");
                            if(fp_products != NULL) {
                                char prod_line[200];
                                int found_in_my_shop = 0;
                                int checking_shop_id = -1;
                                
                                while(fgets(prod_line, sizeof(prod_line), fp_products)) {
                                    char prod_key[50], prod_value[150];
                                    if(sscanf(prod_line, "%s %[^\n]", prod_key, prod_value) == 2) {
                                        if(strcmp(prod_key, "shop_id:") == 0) {
                                            checking_shop_id = atoi(prod_value);
                                        } 
                                        else if(strcmp(prod_key, "product_name:") == 0 && 
                                                checking_shop_id == current_admin.shop_id) {
                                            if(strcmp(prod_value, item_name) == 0) {
                                                found_in_my_shop = 1;
                                                break;
                                            }
                                        }
                                    }
                                }
                                fclose(fp_products);
                                
                                if(found_in_my_shop) {
                                    order_has_my_products = 1;
                                    current_order_items += quantity;
                                    current_order_total += subtotal;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(strcmp(key, "total:") == 0) {
            // End of order - finalize if it has my products
            if(order_has_my_products && current_order_total > 0) {
                total_orders++;
                total_items_sold += current_order_items;
                total_revenue += current_order_total;
            }
        }
        else if(strcmp(key, "---") == 0) {
            // Order separator - reset for next order
            order_has_my_products = 0;
            current_order_total = 0.0;
            current_order_items = 0;
        }
    }
    fclose(fp);
    
    // Calculate averages
    float avg_order_value = (total_orders > 0) ? (total_revenue / total_orders) : 0.0;
    
    // Display results with proper formatting
    printf("\t💰 Total Revenue: %.2f Taka\n", total_revenue);
    printf("\t📦 Total Products Sold: %d items\n", total_items_sold);
    printf("\t🛒 Total Orders: %d orders\n", total_orders);
    printf("\t📊 Average Order Value: %.2f Taka\n", avg_order_value);
    
    // Determine best selling category
    if(total_orders > 0 && total_items_sold > 0) {
        printf("\t🔥 Best Selling Category: Popular Items\n");
    } else {
        printf("\t🔥 Best Selling Category: No sales yet\n");
    }
    
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("\t📅 Report Generated: %02d-%02d-%04d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    
    printf("\t──────────────────────────────────────────────────────────────────────\n");
    printf("\n\t💡 Tips for Growth:\n");
    if(total_orders == 0) {
        printf("\t   • 🎯 Start marketing your products to get first sale\n");
        printf("\t   • 📢 Share product details on social media\n");
        printf("\t   • 💰 Offer discount coupons for new customers\n");
        printf("\t   • 🤝 Ask friends and family to try your products\n");
        printf("\t   • 📱 Create online ads and campaigns\n");
    } else {
        printf("\t   • 📈 Add more products to increase sales\n");
        printf("\t   • ✨ Update product descriptions for better appeal\n");
        printf("\t   • 💲 Monitor competitor pricing\n");
        printf("\t   • 🗣️  Engage with customer feedback\n");
        printf("\t   • 📱 Run digital marketing campaigns\n");
        printf("\t   • 🎁 Create bundle offers and promotions\n");
    }
    
    printf("\n\t⏳ Press Enter to continue...");
    getchar();
}

// View orders function
void view_all_orders() {
    clear_screen();
    printf("\n\n\t╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                             🛒 ORDER MANAGEMENT                          ║\n");
    printf("\t╚══════════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("\t📋 Recent Orders for: %s\n", current_admin.shop_name);
    printf("\t────────────────────────────────────────────────────────────────────────────\n");
    
    FILE *order_file = fopen("order_history.dat", "r");
    if (order_file == NULL) {
        printf("\t❌ No order history found.\n");
        printf("\n\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    char line[500];
    int order_count = 0;
    float total_revenue = 0;
    
    printf("\t┌─────────────────┬─────────────────────┬────────────────┬─────────────────┐\n");
    printf("\t│   Order ID      │     Customer Name   │     Amount     │       Date      │\n");
    printf("\t├─────────────────┼─────────────────────┼────────────────┼─────────────────┤\n");
    
    // Track current order data
    char current_user[50] = "";
    char current_order_id[30] = "";
    char current_date[20] = "";
    float current_order_total = 0.0;
    int order_has_my_products = 0;
    
    while (fgets(line, sizeof(line), order_file)) {
        char key[50], value[150];
        
        // Skip empty lines and comments
        if(strlen(line) <= 1 || line[0] == '/' || line[0] == '#') {
            continue;
        }
        
        if(sscanf(line, "%s %[^\n]", key, value) != 2) {
            continue;
        }
        
        if (strcmp(key, "user:") == 0) {
            strcpy(current_user, value);
            order_has_my_products = 0;
            current_order_total = 0.0;
            strcpy(current_order_id, "");
            strcpy(current_date, "");
        } 
        else if (strcmp(key, "order_id:") == 0) {
            strcpy(current_order_id, value);
        } 
        else if (strcmp(key, "date:") == 0) {
            strcpy(current_date, value);
        } 
        else if (strcmp(key, "item:") == 0) {
            // Parse item: "Product Name x2 = 1000"
            char item_name[100];
            int quantity = 0;
            float subtotal = 0.0;
            
            // Extract item name (everything before " x")
            char *x_pos = strstr(value, " x");
            if(x_pos != NULL) {
                int name_len = x_pos - value;
                if(name_len > 0 && name_len < 100) {
                    strncpy(item_name, value, name_len);
                    item_name[name_len] = '\0';
                    
                    // Extract quantity and subtotal
                    if(sscanf(x_pos, " x%d = %f", &quantity, &subtotal) == 2) {
                        // Sanity check for reasonable values
                        if(quantity > 0 && quantity <= 1000 && subtotal > 0 && subtotal <= 1000000) {
                            // Check if this item belongs to current admin's shop
                            FILE* fp_products = fopen("admin_products.dat", "r");
                            if(fp_products != NULL) {
                                char prod_line[200];
                                int found_in_my_shop = 0;
                                int checking_shop_id = -1;
                                
                                while(fgets(prod_line, sizeof(prod_line), fp_products)) {
                                    char prod_key[50], prod_value[150];
                                    if(sscanf(prod_line, "%s %[^\n]", prod_key, prod_value) == 2) {
                                        if(strcmp(prod_key, "shop_id:") == 0) {
                                            checking_shop_id = atoi(prod_value);
                                        } 
                                        else if(strcmp(prod_key, "product_name:") == 0 && 
                                                checking_shop_id == current_admin.shop_id) {
                                            if(strcmp(prod_value, item_name) == 0) {
                                                found_in_my_shop = 1;
                                                break;
                                            }
                                        }
                                    }
                                }
                                fclose(fp_products);
                                
                                if(found_in_my_shop) {
                                    order_has_my_products = 1;
                                    current_order_total += subtotal;
                                }
                            }
                        }
                    }
                }
            }
        } 
        else if (strcmp(key, "total:") == 0) {
            // End of order - display if it has my products
            if (order_has_my_products && current_order_total > 0 && strlen(current_user) > 0) {
                // Truncate long names to fit in table
                char short_user[20], short_order[16], short_date[13];
                
                if(strlen(current_user) > 19) {
                    strncpy(short_user, current_user, 16);
                    short_user[16] = '\0';
                    strcat(short_user, "...");
                } else {
                    strcpy(short_user, current_user);
                }
                
                if(strlen(current_order_id) > 15) {
                    strncpy(short_order, current_order_id, 12);
                    short_order[12] = '\0';
                    strcat(short_order, "...");
                } else {
                    strcpy(short_order, current_order_id);
                }
                
                if(strlen(current_date) > 12) {
                    strncpy(short_date, current_date, 12);
                    short_date[12] = '\0';
                } else {
                    strcpy(short_date, current_date);
                }
                
                printf("\t│ %-15s │ %-19s │ %14.0f │ %-15s │\n", 
                       short_order, short_user, current_order_total, short_date);
                order_count++;
                total_revenue += current_order_total;
            }
        }
    }
    
    if (order_count == 0) {
        printf("\t│                             No orders found                              │\n");
    }
    
    printf("\t└─────────────────┴─────────────────────┴────────────────┴─────────────────┘\n");
    printf("\n\t📊 Summary: %d orders | Total Revenue: %.0f Taka\n", order_count, total_revenue);
    
    fclose(order_file);
    printf("\n\t⏳ Press Enter to continue...");
    getchar();
}

// Revenue report function
void revenue_report() {
    clear_screen();
    printf("\n\n\t╔════════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                          💰 REVENUE REPORT                             ║\n");
    printf("\t╚════════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("\t📊 Detailed Financial Analysis for: %s\n", current_admin.shop_name);
    printf("\t───────────────────────────────────────────────────────────────────────\n");
    
    FILE *order_file = fopen("order_history.dat", "r");
    if (order_file == NULL) {
        printf("\t❌ No sales data found.\n");
        printf("\n\t⏳ Press Enter to continue...");
        getchar();
        return;
    }
    
    char line[500];
    float total_revenue = 0, today_revenue = 0, week_revenue = 0, month_revenue = 0;
    int total_orders = 0, today_orders = 0;
    
    time_t now;
    struct tm *current_time;
    time(&now);
    current_time = localtime(&now);
    
    char today_date[20];
    sprintf(today_date, "%02d-%02d-%04d", 
            current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900);
    
    // Track current order data
    char current_user[50] = "";
    char current_date[20] = "";
    float current_order_total = 0.0;
    int order_has_my_products = 0;
    
    while (fgets(line, sizeof(line), order_file)) {
        char key[50], value[150];
        
        // Skip empty lines and comments
        if(strlen(line) <= 1 || line[0] == '/' || line[0] == '#') {
            continue;
        }
        
        if(sscanf(line, "%s %[^\n]", key, value) != 2) {
            continue;
        }
        
        if (strcmp(key, "user:") == 0) {
            strcpy(current_user, value);
            order_has_my_products = 0;
            current_order_total = 0.0;
            strcpy(current_date, "");
        } 
        else if (strcmp(key, "date:") == 0) {
            strcpy(current_date, value);
        } 
        else if (strcmp(key, "item:") == 0) {
            // Parse item: "Product Name x2 = 1000"
            char item_name[100];
            int quantity = 0;
            float subtotal = 0.0;
            
            // Extract item name (everything before " x")
            char *x_pos = strstr(value, " x");
            if(x_pos != NULL) {
                int name_len = x_pos - value;
                if(name_len > 0 && name_len < 100) {
                    strncpy(item_name, value, name_len);
                    item_name[name_len] = '\0';
                    
                    // Extract quantity and subtotal
                    if(sscanf(x_pos, " x%d = %f", &quantity, &subtotal) == 2) {
                        // Sanity check for reasonable values
                        if(quantity > 0 && quantity <= 1000 && subtotal > 0 && subtotal <= 1000000) {
                            // Check if this item belongs to current admin's shop
                            FILE* fp_products = fopen("admin_products.dat", "r");
                            if(fp_products != NULL) {
                                char prod_line[200];
                                int found_in_my_shop = 0;
                                int checking_shop_id = -1;
                                
                                while(fgets(prod_line, sizeof(prod_line), fp_products)) {
                                    char prod_key[50], prod_value[150];
                                    if(sscanf(prod_line, "%s %[^\n]", prod_key, prod_value) == 2) {
                                        if(strcmp(prod_key, "shop_id:") == 0) {
                                            checking_shop_id = atoi(prod_value);
                                        } 
                                        else if(strcmp(prod_key, "product_name:") == 0 && 
                                                checking_shop_id == current_admin.shop_id) {
                                            if(strcmp(prod_value, item_name) == 0) {
                                                found_in_my_shop = 1;
                                                break;
                                            }
                                        }
                                    }
                                }
                                fclose(fp_products);
                                
                                if(found_in_my_shop) {
                                    order_has_my_products = 1;
                                    current_order_total += subtotal;
                                }
                            }
                        }
                    }
                }
            }
        } 
        else if (strcmp(key, "total:") == 0) {
            if (order_has_my_products && current_order_total > 0 && strlen(current_user) > 0) {
                total_revenue += current_order_total;
                total_orders++;
                
                // Check if order is from today
                if (strstr(current_date, today_date)) {
                    today_revenue += current_order_total;
                    today_orders++;
                }
                
                // Add to monthly and weekly totals (simplified - treating all as recent)
                month_revenue += current_order_total;
                week_revenue += current_order_total;
            }
        }
    }
    
    fclose(order_file);
    
    // Calculate growth rate (mock calculation)
    float growth_rate = (total_orders > 0) ? (float)(today_orders * 100) / total_orders : 0;
    float monthly_target = 50000.0;
    float target_achievement = (month_revenue / monthly_target) * 100;
    
    printf("\t📅 Today's Sales: %.0f Taka (%d orders)\n", today_revenue, today_orders);
    printf("\t📆 Recent Sales: %.0f Taka\n", week_revenue);
    printf("\t📅 Total Revenue: %.0f Taka (%d orders)\n", total_revenue, total_orders);
    printf("\t📈 Daily Growth: +%.1f%%\n", growth_rate);
    printf("\t🎯 Monthly Target: %.0f Taka (%.1f%% achieved)\n", monthly_target, target_achievement);
    
    printf("\n\t📋 Performance Metrics:\n");
    printf("\t   ├─ Average Order Value: %.0f Taka\n", total_orders > 0 ? total_revenue / total_orders : 0);
    printf("\t   ├─ Best Selling Period: %s\n", today_orders > 0 ? "Today" : "Previous days");
    printf("\t   └─ Status: %s\n", target_achievement >= 100 ? "Target Achieved! 🎉" : "Keep Going! 💪");
    
    printf("\n\t⏳ Press Enter to continue...");
    getchar();
}
