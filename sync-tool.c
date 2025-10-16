#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to sync admin_products.dat to products.dat
void sync_products() {
    FILE* admin_file = fopen("admin_products.dat", "r");
    FILE* customer_file = fopen("products.dat", "w");
    
    if (admin_file == NULL) {
        printf("❌ Error: Cannot open admin_products.dat\n");
        return;
    }
    
    if (customer_file == NULL) {
        printf("❌ Error: Cannot create products.dat\n");
        fclose(admin_file);
        return;
    }
    
    char line[500];
    char current_shop[100] = "";
    char products[20][100];  // Increased size for more products
    int prices[20];
    int quantities[20];
    int product_count = 0;
    int shops_synced = 0;
    
    printf("🔄 Converting and syncing products from admin to customer format...\n");
    
    while (fgets(line, sizeof(line), admin_file)) {
        if (strncmp(line, "product_id:", 11) == 0) {
            // Skip product_id line
            continue;
        }
        else if (strncmp(line, "shop_id:", 8) == 0) {
            // Skip shop_id line
            continue;
        }
        else if (strncmp(line, "shop_name:", 10) == 0) {
            // Extract shop name
            sscanf(line, "shop_name: %[^\n]", current_shop);
        }
        else if (strncmp(line, "product_name:", 13) == 0) {
            // Extract product name
            sscanf(line, "product_name: %[^\n]", products[product_count]);
        }
        else if (strncmp(line, "price:", 6) == 0) {
            // Extract price
            sscanf(line, "price: %d", &prices[product_count]);
        }
        else if (strncmp(line, "stock:", 6) == 0) {
            // Extract stock/quantity
            sscanf(line, "stock: %d", &quantities[product_count]);
            product_count++;
        }
        else if (strncmp(line, "---", 3) == 0) {
            // End of current product - check if we need to write shop data
            if (product_count > 0) {
                // Check if this is the last product for current shop
                long current_pos = ftell(admin_file);
                char next_line[500];
                int is_last_product = 1;
                
                // Read ahead to see if next product is from same shop
                if (fgets(next_line, sizeof(next_line), admin_file)) {
                    if (strncmp(next_line, "product_id:", 11) == 0) {
                        // Read next few lines to get shop name
                        char temp_shop[100] = "";
                        for (int i = 0; i < 3; i++) {
                            if (fgets(next_line, sizeof(next_line), admin_file)) {
                                if (strncmp(next_line, "shop_name:", 10) == 0) {
                                    sscanf(next_line, "shop_name: %[^\n]", temp_shop);
                                    break;
                                }
                            }
                        }
                        if (strcmp(temp_shop, current_shop) == 0) {
                            is_last_product = 0;
                        }
                    }
                }
                
                // Reset file position
                fseek(admin_file, current_pos, SEEK_SET);
                
                if (is_last_product) {
                    // Write shop data in old format for e-shop.c compatibility
                    fprintf(customer_file, "shop: %s\n", current_shop);
                    for (int i = 0; i < product_count; i++) {
                        fprintf(customer_file, "item%d: %s\n", i+1, products[i]);
                        fprintf(customer_file, "price%d: %d\n", i+1, prices[i]);
                        fprintf(customer_file, "quantity%d: %d\n", i+1, quantities[i]);
                    }
                    fprintf(customer_file, "---\n");
                    
                    shops_synced++;
                    printf("  ✓ Synced %s (%d products)\n", current_shop, product_count);
                    
                    // Reset for next shop
                    product_count = 0;
                    strcpy(current_shop, "");
                }
            }
        }
    }
    
    fclose(admin_file);
    fclose(customer_file);
    
    printf("✅ Successfully synced %d shops with all products!\n", shops_synced);
    printf("📂 admin_products.dat (new format) → products.dat (e-shop compatible format)\n");
}

// Function to sync only specific shop products
void sync_shop_products(char* shop_name) {
    FILE* admin_file = fopen("admin_products.dat", "r");
    FILE* customer_file = fopen("products.dat", "r");
    FILE* temp_file = fopen("temp_products.dat", "w");
    
    if (admin_file == NULL) {
        printf("❌ Error: Cannot open admin_products.dat\n");
        return;
    }
    
    if (customer_file == NULL || temp_file == NULL) {
        printf("❌ Error: Cannot access products.dat\n");
        fclose(admin_file);
        return;
    }
    
    printf("🔄 Syncing products for shop: %s\n\n", shop_name);
    
    // First, collect specific shop's products from admin_products.dat
    char line[500];
    char current_shop[100] = "";
    char products[20][100];
    int prices[20];
    int quantities[20];
    int product_count = 0;
    int target_shop_found = 0;
    
    while (fgets(line, sizeof(line), admin_file)) {
        if (strncmp(line, "shop_name:", 10) == 0) {
            sscanf(line, "shop_name: %[^\n]", current_shop);
            if (strcmp(current_shop, shop_name) == 0) {
                target_shop_found = 1;
            }
        }
        else if (target_shop_found && strncmp(line, "product_name:", 13) == 0) {
            sscanf(line, "product_name: %[^\n]", products[product_count]);
        }
        else if (target_shop_found && strncmp(line, "price:", 6) == 0) {
            sscanf(line, "price: %d", &prices[product_count]);
        }
        else if (target_shop_found && strncmp(line, "stock:", 6) == 0) {
            sscanf(line, "stock: %d", &quantities[product_count]);
            product_count++;
        }
        else if (strncmp(line, "---", 3) == 0) {
            if (target_shop_found && strcmp(current_shop, shop_name) != 0) {
                // We've moved to a different shop, stop collecting
                break;
            }
            target_shop_found = 0;
        }
    }
    fclose(admin_file);
    
    if (product_count == 0) {
        printf("❌ No products found for shop: %s\n", shop_name);
        fclose(customer_file);
        fclose(temp_file);
        return;
    }
    
    // Now update products.dat, replacing only the target shop
    char customer_line[500];
    int in_target_shop = 0;
    int shop_found = 0;
    
    while (fgets(customer_line, sizeof(customer_line), customer_file)) {
        char key[50], value[150];
        
        if (sscanf(customer_line, "%[^:]: %[^\n]", key, value) == 2) {
            if (strcmp(key, "shop") == 0) {
                if (strcmp(value, shop_name) == 0) {
                    // Found target shop - replace with admin data
                    shop_found = 1;
                    in_target_shop = 1;
                    fprintf(temp_file, "shop: %s\n", shop_name);
                    for (int i = 0; i < product_count; i++) {
                        fprintf(temp_file, "item%d: %s\n", i+1, products[i]);
                        fprintf(temp_file, "price%d: %d\n", i+1, prices[i]);
                        fprintf(temp_file, "quantity%d: %d\n", i+1, quantities[i]);
                    }
                } else {
                    // Different shop - keep as is
                    in_target_shop = 0;
                    fprintf(temp_file, "%s", customer_line);
                }
            } else if (!in_target_shop) {
                // Not in target shop - copy line
                fprintf(temp_file, "%s", customer_line);
            }
        } else {
            // Handle lines like "---"
            if (strcmp(customer_line, "---\n") == 0) {
                if (in_target_shop) {
                    fprintf(temp_file, "---\n");
                    in_target_shop = 0;
                } else {
                    fprintf(temp_file, "%s", customer_line);
                }
            } else if (!in_target_shop) {
                fprintf(temp_file, "%s", customer_line);
            }
        }
    }
    
    // If shop wasn't found, add it as new shop
    if (!shop_found) {
        fprintf(temp_file, "shop: %s\n", shop_name);
        for (int i = 0; i < product_count; i++) {
            fprintf(temp_file, "item%d: %s\n", i+1, products[i]);
            fprintf(temp_file, "price%d: %d\n", i+1, prices[i]);
            fprintf(temp_file, "quantity%d: %d\n", i+1, quantities[i]);
        }
        fprintf(temp_file, "---\n");
    }
    
    fclose(customer_file);
    fclose(temp_file);
    
    // Replace original file
    remove("products.dat");
    rename("temp_products.dat", "products.dat");
    
    printf("✅ Successfully synced %s with %d products!\n", shop_name, product_count);
    printf("📂 Only %s's products updated in products.dat\n", shop_name);
}

// Function to call sync from admin panel (all shops)
void sync_all_shops() {
    sync_products();
}

int main() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    🔄 PRODUCT SYNC TOOL                   ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║   Synchronizes products from admin to customer database   ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    sync_products();
    
    printf("\n💡 This tool is automatically called by admin panel after product changes.\n");
    printf("⏳ Press Enter to exit...");
    getchar();
    
    return 0;
}
