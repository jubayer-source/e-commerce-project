// C Program to implement Shopping Cart with Labeled File I/O
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// Structure for user details
struct details {
    char uname[50];
    int age;
    char password[100]; // This will store the hashed password
    char email[100];
    char mobile[15];
};

// Structure for shop and item details
struct Shops {
    char shop[100];
    char item_name[7][50];  // Increased from 4 to 7 to handle up to 6 items (index 1-6)
    int item_price[7];      // Increased from 4 to 7
    int item_qty[7];        // Increased from 4 to 7
};

// New structure for items in the cart
struct CartItem {
    char name[50];
    int price;
    int quantity;
};

// Array of structures
struct Shops m[15]; // Increased size to handle more shops (up to 14 shops)
int num_shops = 0; // New variable to track the number of shops

// Array to hold the items in the user's cart
struct CartItem cart_items[100];
int cart_count = 0;

// Function prototypes
void signup();
int validate();
void login();
void cart();
void shop();
void items();
void item_order(int item);
void load_shop_data(); // New function to load data from file
void save_shop_data(); // New function to save data to file
void Shop(int shop_choice);
void run_search_options();
void generate_invoice();
void display_welcome_banner();
void display_loading_animation(char* message);
void clear_screen();
void display_user_profile();
void save_order_history(char* username);
void view_order_history(char* username);
void remove_item_from_cart();
void update_cart_quantity();

// New function prototypes for file handling and hashing
void save_user_to_file(struct details user);
int is_email_exist(char* email);
void hash_password(char* password, char* hashed_password);

// Global variables
char t_name[100], t_password1[100];
char t_password2[100], t_email[100];
char t_mobile[100];
char current_user[100]; // Store current logged in user
int flag = 1, i, j = 0, count = 0, caps = 0;
int Small = 0, total = 0, success = 0, special = 0, numbers = 0;
int x, choice, t_age, item_choice, n, shop_choice, search_choice, confirm, ch, item, shop_id;

// Driver code
int main()
{
    display_welcome_banner();
    load_shop_data(); // Load data from file at the start of the program
    
    while (1) {
        clear_screen();
        printf("\n\n\t╔══════════════════════════════════════════════════════╗\n");
        printf("\t║              🛒 WELCOME TO SHOP CART 🛒              ║\n");
        printf("\t╠══════════════════════════════════════════════════════╣\n");
        printf("\t║                                                      ║\n");
        printf("\t║  [1] 🔐 LOGIN     - Access Your Account              ║\n");
        printf("\t║  [2] 📝 SIGNUP    - Create New Account               ║\n");
        printf("\t║  [3] 🚪 EXIT      - Close Application                ║\n");
        printf("\t║                                                      ║\n");
        printf("\t╚══════════════════════════════════════════════════════╝\n");

        printf("\n\n\t🎯 Enter your choice (1-3): ");
        if (scanf("%d", &choice) != 1) {
            // Handle invalid input (non-integer)
            while (getchar() != '\n'); 
            printf("\n\n\t❌ Please enter a valid choice!!\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            continue; // Continue to the next iteration of the while loop
        }

        switch (choice) {
            case 1: {
                display_loading_animation("Logging you in");
                login();
                break;
            }
            case 2: {
                display_loading_animation("Creating your account");
                signup();
                break;
            }
            case 3: {
                clear_screen();
                printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
                printf("\t║                     👋 THANK YOU!                     ║\n");
                printf("\t║                  Visit Again Soon! 🛍️                  ║\n");
                printf("\t╚═══════════════════════════════════════════════════════╝\n\n");
                save_shop_data(); // Save the updated data before exiting
                return 0;
            }
            default: {
                printf("\n\n\t❌ Please enter a valid choice (1-3)!!\n");
                printf("\t⏳ Press Enter to continue...");
                getchar();
                getchar();
                break;
            }
        }
    }
}

// Utility Functions for Better UX
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void display_welcome_banner() {
    clear_screen();
    printf("\n\n");
    printf("\t╔══════════════════════════════════════════════════════╗\n");
    printf("\t║  🌟🌟🌟  BANGLADESH ONLINE SHOPPING SYSTEM  🌟🌟🌟   ║\n");
    printf("\t║                                                      ║\n");
    printf("\t║          ✨ Your Digital Shopping Partner ✨         ║\n");
    printf("\t║                                                      ║\n");
    printf("\t║       📱 Fast • Secure • Easy • Reliable • 24/7      ║\n");
    printf("\t╚══════════════════════════════════════════════════════╝\n");
    printf("\n\t⏳ Loading system... Please wait...\n");
    
    // Simple loading animation
    for(int i = 0; i < 20; i++) {
        printf("█");
        fflush(stdout);
        usleep(100000); // 0.1 second delay
    }
    printf(" ✅ Ready!\n\n");
    printf("\t🎉 Welcome to the future of shopping! 🎉\n");
    printf("\t⏳ Press Enter to continue...");
    getchar();
}

void display_loading_animation(char* message) {
    printf("\n\t🔄 %s", message);
    for(int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        usleep(500000); // 0.5 second delay
    }
    printf(" ✅ Done!\n");
}

void display_user_profile() {
    // Create truncated versions of name and email to fit in the box
    char display_name[30], display_email[35];
    
    // Truncate name if too long (max 25 chars)
    if (strlen(current_user) > 25) {
        strncpy(display_name, current_user, 22);
        display_name[22] = '\0';
        strcat(display_name, "...");
    } else {
        strcpy(display_name, current_user);
    }
    
    // Truncate email if too long (max 30 chars)
    if (strlen(t_email) > 30) {
        strncpy(display_email, t_email, 27);
        display_email[27] = '\0';
        strcat(display_email, "...");
    } else {
        strcpy(display_email, t_email);
    }
    
    printf("\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                    👤 USER PROFILE                    ║\n");
    printf("\t╠═══════════════════════════════════════════════════════╣\n");
    printf("\t║  Welcome back, %-25s! 👋          ║\n", display_name);
    printf("\t║  🛒 Cart Items: %-2d                                    ║\n", cart_count);
    printf("\t║  📧 Email: %-30s             ║\n", display_email);
    printf("\t╚═══════════════════════════════════════════════════════╝\n");
}

// Simple hashing function for demonstration
void hash_password(char* password, char* hashed_password) {
    char key[] = "my_secret_key";
    int key_len = strlen(key);
    int pass_len = strlen(password);

    for (int i = 0; i < pass_len; i++) {
        hashed_password[i] = password[i] ^ key[i % key_len];
    }
    hashed_password[pass_len] = '\0';
}

// Function to check if an email already exists in the file
int is_email_exist(char* email) {
    FILE* fp = fopen("users.dat", "r");
    if (fp == NULL) {
        return 0;
    }

    char line[200];
    int found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "email:", 6) == 0) {
            char key[50], value[150];
            sscanf(line, "%s %[^\n]", key, value);
            if (strcmp(value, email) == 0) {
                found = 1;
                break;
            }
        }
    }

    fclose(fp);
    return found;
}

// Function to save user details to a file
void save_user_to_file(struct details user) {
    FILE* fp = fopen("users.dat", "a");
    if (fp == NULL) {
        printf("Error: Could not open user data file.\n");
        return;
    }
    
    char hashed_pass[100];
    hash_password(user.password, hashed_pass);

    fprintf(fp, "name: %s\n", user.uname);
    fprintf(fp, "age: %d\n", user.age);
    fprintf(fp, "password: %s\n", hashed_pass);
    fprintf(fp, "email: %s\n", user.email);
    fprintf(fp, "mobile: %s\n", user.mobile);
    fprintf(fp, "---\n");

    fclose(fp);
}

// Signup function
void signup()
{
    clear_screen();
    printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                    📝 SIGNUP PAGE                     ║\n");
    printf("\t╠═══════════════════════════════════════════════════════╣\n");
    printf("\t║       Create your account and start shopping! 🛍️       ║\n");
    printf("\t╚═══════════════════════════════════════════════════════╝\n\n");

    while (getchar() != '\n');
    
    printf("\t👤 Enter Your Full Name: ");
    fgets(t_name, sizeof(t_name), stdin);
    t_name[strcspn(t_name, "\n")] = 0;

    printf("\t📧 Enter Your Email: ");
    scanf("%s", t_email);

    printf("\t🔒 Enter your password (include uppercase, lowercase, number, and special character; 6-12 characters long): ");
    scanf("%s", t_password1);

    printf("\t🔒 Confirm Password: ");
    scanf("%s", t_password2);

    printf("\t📱 Enter Mobile Number (01...): ");
    scanf("%s", t_mobile);

    printf("\n\t🔍 Validating your information...\n");
    
    x = validate();
    if (x == 1) {
        if (is_email_exist(t_email)) {
            printf("\n\t❌ Account Already Exists! Please Login.\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            getchar();
        } else {
            struct details new_user;
            strcpy(new_user.uname, t_name);
            new_user.age = 25; // Default age since we don't ask for it
            strcpy(new_user.password, t_password1);
            strcpy(new_user.email, t_email);
            strcpy(new_user.mobile, t_mobile);

            save_user_to_file(new_user);
            strcpy(current_user, t_name); // Store current user
            
            clear_screen();
            printf("\n\n\t🎉 Account Successfully Created! 🎉\n");
            printf("\t✨ Welcome to Shop Cart, %s! ✨\n\n", t_name);
            
            display_loading_animation("Setting up your profile");
            printf("\t🚀 You are now logged in and ready to shop!\n\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            getchar();
            
            run_search_options();
        }
    } else {
        printf("\n\t❌ Signup failed. Please fix the errors above.\n");
        printf("\t⏳ Press Enter to try again...");
        getchar();
        getchar();
    }
}

// Validation function with comprehensive error checking
int validate()
{
    int validation_status = 1;

    // Validate name
    for (i = 0; t_name[i] != '\0'; i++) {
        if (!((t_name[i] >= 'a' && t_name[i] <= 'z') || (t_name[i] >= 'A' && t_name[i] <= 'Z') || t_name[i] == '.' || t_name[i] == ' ')) {
            printf("\n\tERROR: Please enter a valid Name! Name can only contain letters, spaces, and dots.\n");
            validation_status = 0;
            break;
        }
    }

    // Validate email
    int at_pos = -1, dot_pos = -1;
    for (i = 0; t_email[i] != '\0'; i++) {
        if (t_email[i] == '@') {
            at_pos = i;
        } else if (t_email[i] == '.') {
            dot_pos = i;
        }
    }
    if (at_pos < 1 || dot_pos < at_pos + 2 || dot_pos == strlen(t_email) - 1) {
        printf("\n\tERROR: Please enter a valid E-Mail address (e.g., example@domain.com).\n");
        validation_status = 0;
    }


    // Validate password length
    if (strlen(t_password1) < 6 || strlen(t_password1) > 12) {
        printf("\n\tERROR: Your password length must be between 6 to 12 characters.\n");
        validation_status = 0;
    }
    
    // Validate password match
    if (strcmp(t_password1, t_password2) != 0) {
        printf("\n\tERROR: Password Mismatch. The confirmed password does not match.\n");
        validation_status = 0;
    }
    
    // Validate password complexity
    caps = 0, Small = 0, numbers = 0, special = 0;
    for (i = 0; t_password1[i] != '\0'; i++) {
        if (t_password1[i] >= 'A' && t_password1[i] <= 'Z')
            caps++;
        else if (t_password1[i] >= 'a' && t_password1[i] <= 'z')
            Small++;
        else if (t_password1[i] >= '0' && t_password1[i] <= '9')
            numbers++;
        else if (t_password1[i] == '@' || t_password1[i] == '&' || t_password1[i] == '#' || t_password1[i] == '*' || t_password1[i] == '!' || t_password1[i] == '$' || t_password1[i] == '%')
            special++;
    }
    if (caps < 1 || Small < 1 || numbers < 1 || special < 1) {
        printf("\n\tERROR: Please enter a strong password. It must contain at least one uppercase, lowercase, number, and special character (@, &, #, *, !, $, %%).\n");
        validation_status = 0;
    }

    // Validate mobile number
    if (strlen(t_mobile) != 11 || strncmp(t_mobile, "01", 2) != 0) {
        printf("\n\tERROR: Please enter a valid Bangladeshi Mobile Number (e.g., 01*********). The number must be 11 digits long and start with '01'.\n");
        validation_status = 0;
    } else {
        for (i = 0; i < strlen(t_mobile); i++) {
            if (!isdigit(t_mobile[i])) {
                printf("\n\tERROR: Mobile number must contain only digits.\n");
                validation_status = 0;
                break;
            }
        }
    }
    
    return validation_status;
}

// Login function (enhanced)
void login()
{
    clear_screen();
    printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                     🔐 LOGIN PAGE                     ║\n");
    printf("\t╠═══════════════════════════════════════════════════════╣\n");
    printf("\t║          Welcome back! Please sign in 👋              ║\n");
    printf("\t╚═══════════════════════════════════════════════════════╝\n\n");
    
    printf("\t📧 Enter Your Email: ");
    scanf("%s", t_email);
    printf("\t🔒 Enter Your Password: ");
    scanf("%s", t_password1);

    printf("\n\t🔍 Verifying credentials...\n");

    FILE* fp = fopen("users.dat", "r");
    if (fp == NULL) {
        printf("\n\t❌ No accounts found. Please signup first!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return;
    }

    char file_uname[50], file_hashed_pass[100], file_email[100];
    char line[200];
    int user_found = 0;
    char hashed_input_pass[100];

    hash_password(t_password1, hashed_input_pass);

    while (fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if (strcmp(key, "name:") == 0) {
            strcpy(file_uname, value);
        } else if (strcmp(key, "password:") == 0) {
            strcpy(file_hashed_pass, value);
        } else if (strcmp(key, "email:") == 0) {
            strcpy(file_email, value);
            
            if (strcmp(file_email, t_email) == 0) {
                user_found = 1;
            }
        } else if (strcmp(key, "---") == 0) {
            if (user_found) {
                if (strcmp(file_hashed_pass, hashed_input_pass) == 0) {
                    strcpy(current_user, file_uname); // Store current user
                    fclose(fp);
                    
                    clear_screen();
                    printf("\n\n\t🎉 Login Successful! 🎉\n");
                    printf("\t✨ Welcome back, %s! ✨\n\n", file_uname);
                    
                    display_loading_animation("Loading your dashboard");
                    display_user_profile();
                    
                    printf("\n\t⏳ Press Enter to continue shopping...");
                    getchar();
                    getchar();
                    
                    run_search_options();
                    return;
                } else {
                    printf("\n\t❌ Invalid Password!\n");
                    printf("\t⏳ Press Enter to try again...");
                    getchar();
                    getchar();
                    fclose(fp);
                    return;
                }
            }
            user_found = 0;
        }
    }
    fclose(fp);
    
    if (!user_found) {
        printf("\n\t❌ Account doesn't exist. Please signup first!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
    }
}

// Enhanced function to handle post-login search options
void run_search_options() {
    while(1) {
        clear_screen();
        display_user_profile();
        
        printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
        printf("\t║                  🛍️ SHOPPING DASHBOARD                 ║\n");
        printf("\t╠═══════════════════════════════════════════════════════╣\n");
        printf("\t║                                                       ║\n");
        printf("\t║  [1] 🏪 Browse by Shop   - Explore different stores   ║\n");
        printf("\t║  [2] 🔍 Browse by Item   - Search specific products   ║\n");
        printf("\t║  [3] 🛒 View Cart        - Check your selected items  ║\n");
        printf("\t║  [4] 📋 Order History    - View past purchases        ║\n");
        printf("\t║  [5] 🚪 Logout           - Exit safely                ║\n");
        printf("\t║                                                       ║\n");
        printf("\t╚═══════════════════════════════════════════════════════╝\n");
        
        printf("\n\t🎯 Choose your option (1-5): ");
        
        if (scanf("%d", &search_choice) != 1) {
            while (getchar() != '\n');
            printf("\n\t❌ Please enter a valid choice (1-5)!\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            continue;
        }

        switch (search_choice) {
            case 1: {
                display_loading_animation("Loading shops");
                shop();
                break;
            }
            case 2: {
                display_loading_animation("Loading items");
                items();
                break;
            }
            case 3: {
                display_loading_animation("Loading your cart");
                cart();
                break;
            }
            case 4: {
                display_loading_animation("Loading order history");
                view_order_history(current_user);
                break;
            }
            case 5: {
                clear_screen();
                printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
                printf("\t║                    👋 LOGGED OUT                      ║\n");
                printf("\t╠═══════════════════════════════════════════════════════╣\n");
                printf("\t║                                                       ║\n");
                printf("\t║             Thanks for shopping with us! 🛍️            ║\n");
                printf("\t║                Come back again soon! ✨               ║\n");
                printf("\t║                                                       ║\n");
                printf("\t╚═══════════════════════════════════════════════════════╝\n");
                printf("\n\t⏳ Press Enter to continue...");
                getchar();
                getchar();
                return;
            }
            default: {
                printf("\n\t❌ Please enter a valid choice (1-5)!\n");
                printf("\t⏳ Press Enter to continue...");
                getchar();
                getchar();
                break;
            }
        }
    }
}

// New function to load shop data from file
void load_shop_data() {
    FILE* fp = fopen("products.dat", "r");
    if (fp == NULL) {
        printf("products.dat not found. Creating a new file with default data.\n");
        num_shops = 4;
        
        // Default data setup with new structure
        strcpy(m[1].shop, "Aarong");
        strcpy(m[1].item_name[1], "Cotton Panjabi"); m[1].item_price[1] = 1450; m[1].item_qty[1] = 5;
        strcpy(m[1].item_name[2], "Handloom Saree"); m[1].item_price[2] = 2200; m[1].item_qty[2] = 3;
        strcpy(m[1].item_name[3], "Leather Bag"); m[1].item_price[3] = 3200; m[1].item_qty[3] = 0;

        strcpy(m[2].shop, "Yellow");
        strcpy(m[2].item_name[1], "Casual Shirt"); m[2].item_price[1] = 1250; m[2].item_qty[1] = 10;
        strcpy(m[2].item_name[2], "Jeans Pant"); m[2].item_price[2] = 1800; m[2].item_qty[2] = 7;
        strcpy(m[2].item_name[3], "Graphic Tee"); m[2].item_price[3] = 850; m[2].item_qty[3] = 2;

        strcpy(m[3].shop, "Ecstasy");
        strcpy(m[3].item_name[1], "Formal Shirt"); m[3].item_price[1] = 1650; m[3].item_qty[1] = 8;
        strcpy(m[3].item_name[2], "Blazer"); m[3].item_price[2] = 4500; m[3].item_qty[2] = 4;
        strcpy(m[3].item_name[3], "Slim Fit Trousers"); m[3].item_price[3] = 1900; m[3].item_qty[3] = 6;
        
        strcpy(m[4].shop, "Easy");
        strcpy(m[4].item_name[1], "T Shirt"); m[4].item_price[1] = 1650; m[4].item_qty[1] = 4;
        strcpy(m[4].item_name[2], "Tops"); m[4].item_price[2] = 4500; m[4].item_qty[2] = 4;
        strcpy(m[4].item_name[3], "Short Pent"); m[4].item_price[3] = 1900; m[4].item_qty[3] = 6;
        
        save_shop_data();
        return;
    }

    char line[200];
    int shop_index = 0;
    int item_index = 0;

    while (fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        if (sscanf(line, "%[^:]: %[^\n]", key, value) == 2) {
            if (strcmp(key, "shop") == 0) {
                shop_index++;
                strcpy(m[shop_index].shop, value);
                item_index = 0;
            } else if (strncmp(key, "item", 4) == 0) {
                item_index = atoi(&key[4]);
                strcpy(m[shop_index].item_name[item_index], value);
            } else if (strncmp(key, "price", 5) == 0) {
                item_index = atoi(&key[5]);
                m[shop_index].item_price[item_index] = atoi(value);
            } else if (strncmp(key, "quantity", 8) == 0) {
                item_index = atoi(&key[8]);
                m[shop_index].item_qty[item_index] = atoi(value);
            }
        }
    }
    num_shops = shop_index;
    fclose(fp);
}

// New function to save shop data to file
void save_shop_data() {
    FILE* fp = fopen("products.dat", "w");
    if (fp == NULL) {
        printf("Error: Could not save product data.\n");
        return;
    }

    for (int i = 1; i <= num_shops; i++) {
        fprintf(fp, "shop: %s\n", m[i].shop);
        // Find the maximum item index for this shop
        int max_items = 0;
        for (int j = 1; j <= 6; j++) {
            if (strlen(m[i].item_name[j]) > 0) {
                max_items = j;
            }
        }
        // Save only the items that exist
        for (int j = 1; j <= max_items; j++) {
            if (strlen(m[i].item_name[j]) > 0) {
                fprintf(fp, "item%d: %s\n", j, m[i].item_name[j]);
                fprintf(fp, "price%d: %d\n", j, m[i].item_price[j]);
                fprintf(fp, "quantity%d: %d\n", j, m[i].item_qty[j]);
            }
        }
        fprintf(fp, "---\n");
    }

    fclose(fp);
}


// Enhanced Shop function
void shop()
{
    clear_screen();
    printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                    🏪 SELECT SHOP                     ║\n");
    printf("\t╚═══════════════════════════════════════════════════════╝\n\n");
    
    for(int i = 1; i <= num_shops; i++) {
        printf("\t[%d] %s\n", i, m[i].shop);
    }
    printf("\t[%d] 🔙 Back to Dashboard\n", num_shops + 1);
    
    printf("\n\t🎯 Please select shop (1-%d): ", num_shops + 1);

    if (scanf("%d", &shop_choice) != 1) {
        while (getchar() != '\n');
        printf("\n\t❌ Please Enter a valid choice\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        return;
    }

    if (shop_choice > num_shops + 1 || shop_choice < 1) {
        printf("\n\t❌ Please Enter a valid choice (1-%d)\n", num_shops + 1);
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        shop();
    } else if (shop_choice == num_shops + 1) {
        return;
    } else {
        Shop(shop_choice);
    }
}

// Enhanced function to display items in a shop with quantity
void Shop(int shop_choice)
{
    while (1) {
        clear_screen();
        
        // Create a properly centered shop name display
        char shop_title[100];
        sprintf(shop_title, "🛍️  %s STORE", m[shop_choice].shop);
        int title_len = strlen(shop_title);
        int padding = (70 - title_len) / 2; // 70 is the box width minus borders
        
        printf("\n\n\t╔══════════════════════════════════════════════════════════════════════╗\n");
        printf("\t║  %*s%s%*s    ║\n", padding, "", shop_title, 70 - title_len - padding, "");
        printf("\t╚══════════════════════════════════════════════════════════════════════╝\n\n");
        
        printf("\t🏷️  Available Products:\n");
        printf("\t───────────────────────────────────────────────────────────────────────\n");
        
        // Find the maximum item count for this shop
        int max_items = 0;
        for(int i = 1; i <= 6; i++) {
            if(strlen(m[shop_choice].item_name[i]) > 0) {
                max_items = i;
            }
        }
        
        for(int i = 1; i <= max_items; i++) {
            if(strlen(m[shop_choice].item_name[i]) > 0) {
                if(m[shop_choice].item_qty[i] > 0) {
                    printf("\t[%d] %-32s💰%12d Tk   📦 Stock:%3d\n", 
                           i, m[shop_choice].item_name[i], m[shop_choice].item_price[i], m[shop_choice].item_qty[i]);
                } else {
                    printf("\t[%d] %-32s💰%12d Tk   ❌ OUT OF STOCK\n", 
                           i, m[shop_choice].item_name[i], m[shop_choice].item_price[i]);
                }
            }
        }
        
        printf("\t────────────────────────────────────────────────────────────────────────\n");
        printf("\t[%d] 🛒 View Cart\n", max_items + 1);
        printf("\t[%d] 🔙 Back to Shop List\n", max_items + 2);
        printf("\n\t🎯 Enter Your Choice (1-%d): ", max_items + 2);
        
        if (scanf("%d", &item_choice) != 1) {
            while (getchar() != '\n');
            printf("\n\t❌ Please Enter a valid choice\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            continue;
        }
        
        int quantity = 0;
        if (item_choice >= 1 && item_choice <= max_items) {
            // Handle item selection
            if (m[shop_choice].item_qty[item_choice] == 0) {
                printf("\n\t😔 Sorry, '%s' is out of stock!\n", m[shop_choice].item_name[item_choice]);
                printf("\t⏳ Press Enter to continue...");
                getchar();
                getchar();
            } else {
                printf("\n\t📦 Enter quantity for %s (Max: %d): ", 
                       m[shop_choice].item_name[item_choice], m[shop_choice].item_qty[item_choice]);
                scanf("%d", &quantity);
                
                if (quantity <= 0 || quantity > m[shop_choice].item_qty[item_choice]) {
                    printf("\n\t❌ Invalid quantity! Please enter between 1 and %d.\n", 
                           m[shop_choice].item_qty[item_choice]);
                    printf("\t⏳ Press Enter to continue...");
                    getchar();
                    getchar();
                } else {
                    // Add item to cart
                    strcpy(cart_items[cart_count].name, m[shop_choice].item_name[item_choice]);
                    cart_items[cart_count].price = m[shop_choice].item_price[item_choice];
                    cart_items[cart_count].quantity = quantity;
                    cart_count++;
                    
                    // Decrease the stock
                    m[shop_choice].item_qty[item_choice] -= quantity;
                    
                    printf("\n\t✅ %s added to cart successfully!\n", m[shop_choice].item_name[item_choice]);
                    printf("\t⏳ Press Enter to continue...");
                    getchar();
                    getchar();
                }
            }
        } else if (item_choice == max_items + 1) {
            cart();
            return;
        } else if (item_choice == max_items + 2) {
            return;
        } else {
            printf("\n\t❌ Please Enter a valid choice (1-%d)\n", max_items + 2);
            printf("\t⏳ Press Enter to continue...");
            getchar();
            getchar();
        }
    }
}

// Enhanced Items function with quantity display
void items()
{
    while (1) {
        clear_screen();
        printf("\n\n\t╔══════════════════════════════════════════════════════════════════════════╗\n");
        printf("\t║                          🔍 BROWSE ALL ITEMS                             ║\n");
        printf("\t╚══════════════════════════════════════════════════════════════════════════╝\n\n");
        
        int item_counter = 1;
        printf("\t🛍️  All Available Products:\n");
        printf("\t────────────────────────────────────────────────────────────────────────────\n");
        
        // Calculate total items across all shops
        int total_items = 0;
        for(int i = 1; i <= num_shops; i++) {
            printf("\n\t🏪 %s:\n", m[i].shop);
            for(int j = 1; j <= 6; j++) {
                if(strlen(m[i].item_name[j]) > 0) {
                    if(m[i].item_qty[j] > 0) {
                        printf("\t[%d] %-32s 💰 %12d Tk   📦 Stock: %3d\n", 
                               item_counter, m[i].item_name[j], m[i].item_price[j], m[i].item_qty[j]);
                    } else {
                        printf("\t[%d] %-32s 💰 %12d Tk   ❌ OUT OF STOCK\n", 
                               item_counter, m[i].item_name[j], m[i].item_price[j]);
                    }
                    item_counter++;
                    total_items++;
                }
            }
        }
        
        printf("\t────────────────────────────────────────────────────────────────────────────\n");
        printf("\t[%d] 🛒 View Cart\n", total_items + 1);
        printf("\t[%d] 🔙 Back to Dashboard\n", total_items + 2);
        printf("\n\t🎯 Enter Your Choice (1-%d): ", total_items + 2);
        
        if (scanf("%d", &item) != 1) {
            while (getchar() != '\n');
            printf("\n\t❌ Please Enter a valid choice\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            continue;
        }

        if (item > total_items + 2 || item < 1) {
            printf("\n\t❌ Please Enter a valid choice (1-%d)\n", total_items + 2);
            printf("\t⏳ Press Enter to continue...");
            getchar();
            getchar();
        } else if (item == total_items + 1) {
            cart();
            return;
        } else if (item == total_items + 2) {
            return;
        } else {
            item_order(item);
        }
    }
}

// Enhanced item_order function with quantity management
void item_order(int item)
{
    // Find which shop and item this corresponds to
    int current_item = 1;
    int found_shop = 0, found_item = 0;
    
    for(int i = 1; i <= num_shops && !found_shop; i++) {
        for(int j = 1; j <= 6; j++) {
            if(strlen(m[i].item_name[j]) > 0) {
                if(current_item == item) {
                    found_shop = i;
                    found_item = j;
                    break;
                }
                current_item++;
            }
        }
    }
    
    if(!found_shop) {
        printf("\n\t❌ Item not found!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return;
    }

    int quantity = 0;

    if (m[found_shop].item_qty[found_item] == 0) {
        printf("\n\t😔 Sorry, '%s' is out of stock!\n", m[found_shop].item_name[found_item]);
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return;
    }

    printf("\n\t📦 Enter quantity for %s (Max: %d): ", 
           m[found_shop].item_name[found_item], m[found_shop].item_qty[found_item]);
    scanf("%d", &quantity);
    
    if (quantity <= 0 || quantity > m[found_shop].item_qty[found_item]) {
        printf("\n\t❌ Invalid quantity! Please enter between 1 and %d.\n", 
               m[found_shop].item_qty[found_item]);
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
    } else {
        strcpy(cart_items[cart_count].name, m[found_shop].item_name[found_item]);
        cart_items[cart_count].price = m[found_shop].item_price[found_item];
        cart_items[cart_count].quantity = quantity;
        cart_count++;
        
        m[found_shop].item_qty[found_item] -= quantity;
        printf("\n\t✅ %s added to cart successfully!\n", m[found_shop].item_name[found_item]);
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
    }
}

// Enhanced Cart function
void cart()
{
    clear_screen();
    char confirm_order;

    printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                     🛒 YOUR CART                      ║\n");
    printf("\t╚═══════════════════════════════════════════════════════╝\n");

    if (cart_count == 0) {
        printf("\n\t😔 Your cart is empty!\n");
        printf("\t🛍️ Let's add some items to your cart.\n");
        printf("\n\t⏳ Press Enter to continue shopping...");
        getchar();
        getchar();
        return;
    }

    // Display a summary of the items in the cart
    int grand_total = 0;
    printf("\n\n\t********************* ORDER SUMMARY *********************\n\n");
    printf("\t%-25s %-10s %-10s %-10s\n", "Item Name", "Price", "Qty", "Subtotal");
    printf("\t----------------------------------------------------------\n");

    for (int i = 0; i < cart_count; i++) {
        int subtotal = cart_items[i].price * cart_items[i].quantity;
        printf("\t%-25s %-10d %-10d %-10d\n", cart_items[i].name, cart_items[i].price, cart_items[i].quantity, subtotal);
        grand_total += subtotal;
    }
    
    printf("\t----------------------------------------------------------\n");
    printf("\t%-45s = %d 💰\n", "TOTAL AMOUNT:", grand_total);
    printf("\n\t***************************************************\n\n");
    
    printf("\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                   CART OPTIONS                        ║\n");
    printf("\t╠═══════════════════════════════════════════════════════╣\n");
    printf("\t║  [1] ✅ Confirm Order   [2] ❌ Remove Item            ║\n");
    printf("\t║  [3] 📝 Update Quantity [4] 🔙 Continue Shopping      ║\n");
    printf("\t╚═══════════════════════════════════════════════════════╝\n");
    
    printf("\n\t🎯 Choose option (1-4): ");
    int cart_option;
    scanf("%d", &cart_option);
    
    switch(cart_option) {
        case 1:
            // Confirm order
            printf("\n\t💳 Do you wish to place this order? (y/n): ");
            while (getchar() != '\n'); 
            scanf(" %c", &confirm_order);
            
            if (confirm_order == 'y' || confirm_order == 'Y') {
                // Clear any remaining input buffer before showing invoice
                while (getchar() != '\n');
                
                generate_invoice(); // This will show invoice and wait for user input
                save_order_history(current_user);
                save_shop_data(); // Save the updated stock to the file
                
                // Display thank you message in a nice box
                printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
                printf("\t║                 🎉 ORDER COMPLETED! 🎉                ║\n");
                printf("\t╠═══════════════════════════════════════════════════════╣\n");
                printf("\t║                                                       ║\n");
                printf("\t║             Thank You for your Order! 🛍️               ║\n");
                printf("\t║            Your items are on the way! 🚚              ║\n");
                printf("\t║                                                       ║\n");
                printf("\t║              Welcome back anytime! ✨                 ║\n");
                printf("\t║                                                       ║\n");
                printf("\t╚═══════════════════════════════════════════════════════╝\n");
                printf("\n\t⏳ Press Enter to continue...");
                getchar();
                
                // Reset the cart for the next order after user confirmation
                cart_count = 0;
                total = 0;
            } else {
                printf("\n\t❌ Order cancelled. Returning to cart...\n");
                printf("\t⏳ Press Enter to continue...");
                getchar();
                getchar();
                cart(); // Return to cart
            }
            break;
            
        case 2:
            remove_item_from_cart();
            break;
            
        case 3:
            update_cart_quantity();
            break;
            
        case 4:
            printf("\n\t🛍️ Continue shopping...\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            getchar();
            break;
            
        default:
            printf("\n\t❌ Invalid option. Returning to cart...\n");
            printf("\t⏳ Press Enter to continue...");
            getchar();
            getchar();
            cart();
            break;
    }
}

// Enhanced function to generate and display the invoice
void generate_invoice() {
    int grand_total = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    clear_screen();
    printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                     🧾 INVOICE                        ║\n");
    printf("\t╠═══════════════════════════════════════════════════════╣\n");
    printf("\t║  Date: %02d-%02d-%04d                      Time: %02d:%02d    ║\n", 
           tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
    printf("\t║  Customer: %-38s     ║\n", current_user);
    printf("\t╚═══════════════════════════════════════════════════════╝\n\n");
    
    printf("\t%-25s %-10s %-10s %-10s\n", "Item Name", "Price", "Qty", "Subtotal");
    printf("\t----------------------------------------------------------\n");

    for (int i = 0; i < cart_count; i++) {
        int subtotal = cart_items[i].price * cart_items[i].quantity;
        printf("\t%-25s %-10d %-10d %-10d\n", cart_items[i].name, cart_items[i].price, cart_items[i].quantity, subtotal);
        grand_total += subtotal;
    }
    
    printf("\t----------------------------------------------------------\n");
    printf("\t%-45s = %d 💰\n", "TOTAL AMOUNT:", grand_total);
    printf("\n\t********************************************************\n");
    printf("\t   🎉 Payment Successful! Order ID: #%d%02d%02d%02d%02d%02d 🎉\n", 
           tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("\t********************************************************\n\n");
    
    printf("\t💾 Invoice saved successfully!\n");
    printf("\t⏳ Press Enter to continue...");
    getchar(); // Wait for user input before proceeding
}

// Function to save order history
void save_order_history(char* username) {
    FILE* fp = fopen("order_history.dat", "a");
    if (fp == NULL) {
        printf("Error: Could not save order history.\n");
        return;
    }
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    fprintf(fp, "user: %s\n", username);
    fprintf(fp, "order_id: #%d%02d%02d%02d%02d%02d\n", 
            tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fp, "date: %02d-%02d-%04d %02d:%02d\n", 
            tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
    
    int grand_total = 0;
    for (int i = 0; i < cart_count; i++) {
        int subtotal = cart_items[i].price * cart_items[i].quantity;
        fprintf(fp, "item: %s x%d = %d\n", cart_items[i].name, cart_items[i].quantity, subtotal);
        grand_total += subtotal;
    }
    fprintf(fp, "total: %d\n", grand_total);
    fprintf(fp, "---\n");
    
    fclose(fp);
}

// Function to view order history
void view_order_history(char* username) {
    clear_screen();
    printf("\n\n\t╔═══════════════════════════════════════════════════════╗\n");
    printf("\t║                   📋 ORDER HISTORY                    ║\n");
    printf("\t╚═══════════════════════════════════════════════════════╝\n\n");
    
    FILE* fp = fopen("order_history.dat", "r");
    if (fp == NULL) {
        printf("\t😔 No order history found.\n");
        printf("\t🛍️ Start shopping to see your orders here!\n");
        printf("\n\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    char line[200];
    int found_orders = 0;
    int order_count = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        char key[50], value[150];
        sscanf(line, "%s %[^\n]", key, value);
        
        if (strcmp(key, "user:") == 0 && strcmp(value, username) == 0) {
            found_orders = 1;
            order_count++;
            printf("\t╔═══════════════════════════════════════════════════════╗\n");
            printf("\t║                      🛒 ORDER #%-2d                     ║\n", order_count);
            printf("\t╠═══════════════════════════════════════════════════════╣\n");
        } else if (found_orders && strcmp(key, "order_id:") == 0) {
            printf("\t║ 🆔 Order ID: %-38s   ║\n", value);
        } else if (found_orders && strcmp(key, "date:") == 0) {
            printf("\t║ 📅 Date: %-42s   ║\n", value);
            printf("\t╠═══════════════════════════════════════════════════════╣\n");
        } else if (found_orders && strcmp(key, "item:") == 0) {
            printf("\t║   • %-49s ║\n", value);
        } else if (found_orders && strcmp(key, "total:") == 0) {
            printf("\t╠═══════════════════════════════════════════════════════╣\n");
            printf("\t║ 💰 Total Amount: %-33s Tk ║\n", value);
            printf("\t╚═══════════════════════════════════════════════════════╝\n\n");
        } else if (strcmp(key, "---") == 0) {
            found_orders = 0;
        }
    }
    
    if (order_count == 0) {
        printf("\t😔 No orders found for your account.\n");
        printf("\t🛍️ Start shopping to see your orders here!\n");
    }
    
    fclose(fp);
    printf("\n\t⏳ Press Enter to continue...");
    getchar();
    getchar();
}

// Enhanced function to remove item from cart and return stock
void remove_item_from_cart() {
    if (cart_count == 0) {
        printf("\n\t😔 Cart is empty!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    printf("\n\t🗑️ Remove Item from Cart:\n");
    printf("\t─────────────────────────────────────────────────────────\n");
    for (int i = 0; i < cart_count; i++) {
        printf("\t[%d] %-20s (Qty: %d)\n", i+1, cart_items[i].name, cart_items[i].quantity);
    }
    printf("\t[%d] 🔙 Back to Cart Options\n", cart_count + 1);
    printf("\t─────────────────────────────────────────────────────────\n");
    
    printf("\n\t🎯 Enter item number to remove (1-%d) or %d to go back: ", cart_count, cart_count + 1);
    int item_to_remove;
    scanf("%d", &item_to_remove);
    
    if (item_to_remove == cart_count + 1) {
        // User chose to go back to cart options
        printf("\n\t🔙 Going back to cart options...\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return; // This will go back to cart() function
    } else if (item_to_remove >= 1 && item_to_remove <= cart_count) {
        char removed_item[50];
        int removed_quantity;
        
        strcpy(removed_item, cart_items[item_to_remove - 1].name);
        removed_quantity = cart_items[item_to_remove - 1].quantity;
        
        // Return the quantity back to stock
        for (int i = 1; i <= num_shops; i++) {
            for (int j = 1; j <= 6; j++) {
                if (strcmp(m[i].item_name[j], removed_item) == 0) {
                    m[i].item_qty[j] += removed_quantity;
                    break;
                }
            }
        }
        
        // Shift items to remove the selected one
        for (int i = item_to_remove - 1; i < cart_count - 1; i++) {
            cart_items[i] = cart_items[i + 1];
        }
        cart_count--;
        
        printf("\n\t✅ %s (Qty: %d) removed successfully!\n", removed_item, removed_quantity);
        printf("\t🔄 Stock has been returned to the store.\n");
    } else {
        printf("\n\t❌ Invalid item number!\n");
    }
    
    printf("\t⏳ Press Enter to continue...");
    getchar();
    getchar();
    cart(); // Return to cart
}

// Enhanced function to update cart quantity with stock checking
void update_cart_quantity() {
    if (cart_count == 0) {
        printf("\n\t😔 Cart is empty!\n");
        printf("\t⏳ Press Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    printf("\n\t📝 Update Item Quantity:\n");
    printf("\t─────────────────────────────────────────────────────────\n");
    for (int i = 0; i < cart_count; i++) {
        printf("\t[%d] %-20s (Current Qty: %d)\n", i+1, cart_items[i].name, cart_items[i].quantity);
    }
    printf("\t─────────────────────────────────────────────────────────\n");
    
    printf("\n\t🎯 Enter item number to update (1-%d): ", cart_count);
    int item_to_update;
    scanf("%d", &item_to_update);
    
    if (item_to_update >= 1 && item_to_update <= cart_count) {
        char selected_item[50];
        strcpy(selected_item, cart_items[item_to_update - 1].name);
        
        // Find the original stock of this item
        int available_stock = 0;
        int current_cart_qty = cart_items[item_to_update - 1].quantity;
        
        // Search through all shops to find this item and its current stock
        for (int i = 1; i <= num_shops; i++) {
            for (int j = 1; j <= 6; j++) {
                if (strcmp(m[i].item_name[j], selected_item) == 0) {
                    available_stock = m[i].item_qty[j];
                    goto found_item; // Break out of both loops
                }
            }
        }
        found_item:
        
        // The maximum they can have is their current cart quantity plus what's available in stock
        // This represents what they can achieve by taking more from available stock
        int max_possible = current_cart_qty + available_stock;
        
        printf("\n\t📋 Item: %s\n", selected_item);
        printf("\t📦 Current Quantity in Cart: %d\n", current_cart_qty);
        printf("\t🏪 Available Stock in Store: %d\n", available_stock);
        printf("\t✅ Maximum You Can Have: %d\n", max_possible);
        printf("\t─────────────────────────────────────────\n");
        
        printf("\t📦 Enter new quantity (1-%d): ", max_possible);
        int new_quantity;
        scanf("%d", &new_quantity);
        
        if (new_quantity > 0 && new_quantity <= max_possible) {
            // Calculate the difference
            int quantity_diff = new_quantity - current_cart_qty;
            
            // Update the cart
            cart_items[item_to_update - 1].quantity = new_quantity;
            
            // Update the stock (decrease if more items taken, increase if items returned)
            for (int i = 1; i <= num_shops; i++) {
                for (int j = 1; j <= 6; j++) {
                    if (strcmp(m[i].item_name[j], selected_item) == 0) {
                        m[i].item_qty[j] -= quantity_diff; // This will subtract if positive, add if negative
                        break;
                    }
                }
            }
            
            printf("\n\t✅ Quantity updated successfully!\n");
            printf("\t📊 New quantity in cart: %d\n", new_quantity);
        } else {
            printf("\n\t❌ Invalid quantity! Please enter between 1 and %d.\n", max_possible);
        }
    } else {
        printf("\n\t❌ Invalid item number!\n");
    }
    
    printf("\t⏳ Press Enter to continue...");
    getchar();
    getchar();
    cart(); // Return to cart
}