#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
    #define CLEAR_COMMAND "cls"
    #define ADMIN_EXE "admin-panel.exe"
    #define ESHOP_EXE "e-shop.exe"
    #define PATH_SEP "\\"
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #define CLEAR_COMMAND "clear"
    #define ADMIN_EXE "./admin-panel"
    #define ESHOP_EXE "./e-shop"
    #define PATH_SEP "/"
#endif

void clear_screen() {
    system(CLEAR_COMMAND);
}

void show_menu() {
    clear_screen();
    printf("\n");
    printf("\t╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("\t║                       🚀 E-COMMERCE SYSTEM LAUNCHER 🚀                         ║\n");
    printf("\t╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("\t║                                                                                ║\n");
    printf("\t║  Choose which application to run:                                              ║\n");
    printf("\t║                                                                                ║\n");
    printf("\t║  [1] 🏪 Admin Panel    - Shop Owner Management System                          ║\n");
    printf("\t║  [2] 🛒 E-Shop         - Customer Shopping Application                         ║\n");
    printf("\t║  [3] 🔄 Both Apps      - Run both applications together                        ║\n");
    printf("\t║  [4] 🆔 Test Login     - Quick test with sample accounts                       ║\n");
    printf("\t║  [5] 🚪 Exit           - Close launcher                                        ║\n");
    printf("\t║                                                                                ║\n");
    printf("\t╚════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n\t🎯 Enter your choice (1-5): ");
}

void launch_admin() {
    printf("\n\t🏪 Starting Admin Panel...\n");
    printf("\t📋 Test admin account available:\n");
    printf("\t   Email: test@gmail.com, Password: Test!2\n");
    printf("\t💼 For other accounts, contact system administrator\n\n");
    system(ADMIN_EXE);
}

void launch_eshop() {
    printf("\n\t🛒 Starting E-Shop Customer App...\n");
    printf("\t📋 Test customer account available:\n");
    printf("\t   Email: test@gmail.com, Password: Test!2\n");
    printf("\t📝 You can also create new account or login with existing ones\n\n");
    system(ESHOP_EXE);
}

void launch_both() {
    printf("\n\t🔄 Starting both applications...\n");
    
#ifdef _WIN32
    printf("\t🏪 Opening Admin Panel in new window...\n");
    system("start \"Admin Panel\" " ADMIN_EXE);
    
    printf("\t🛒 Opening E-Shop in new window...\n");
    system("start \"E-Shop\" " ESHOP_EXE);
    
    printf("\t✅ Both applications opened in separate windows!\n");
    printf("\t📱 Switch between windows to use both apps\n");
    
#else
    printf("\t📱 Opening both applications in new terminals...\n");
    
    // Try to open Admin Panel in new terminal window
    printf("\t🏪 Starting Admin Panel in new terminal...\n");
    system("gnome-terminal --title=\"Admin Panel\" -- ./admin-panel 2>/dev/null &");
    if (system("which gnome-terminal > /dev/null 2>&1") != 0) {
        system("xterm -title \"Admin Panel\" -e ./admin-panel 2>/dev/null &");
        if (system("which xterm > /dev/null 2>&1") != 0) {
            system("konsole --title \"Admin Panel\" -e ./admin-panel 2>/dev/null &");
        }
    }
    
    sleep(2);  // Give time for first terminal to open
    
    // Try to open E-Shop in new terminal window
    printf("\t🛒 Starting E-Shop in new terminal...\n");
    system("gnome-terminal --title=\"E-Shop\" -- ./e-shop 2>/dev/null &");
    if (system("which gnome-terminal > /dev/null 2>&1") != 0) {
        system("xterm -title \"E-Shop\" -e ./e-shop 2>/dev/null &");
        if (system("which xterm > /dev/null 2>&1") != 0) {
            system("konsole --title \"E-Shop\" -e ./e-shop 2>/dev/null &");
        }
    }
    
    printf("\t✅ Both applications started in separate terminals!\n");
    printf("\t📱 Look for new terminal windows: 'Admin Panel' and 'E-Shop'\n");
    printf("\t⏳ Press Enter to continue...");
    getchar(); getchar();
#endif
}

void launch_test_mode() {
    printf("\n\t🆔 Quick Test Mode\n");
    printf("\t════════════════════════════════════════════════════════════════\n");
    printf("\t📊 Available Test Account:\n");
    printf("\t   • test@gmail.com : Test!2\n");
    printf("\t\n");
    printf("\t💼 Professional accounts: Contact system administrator\n");
    printf("\t👥 Customer test account: test@gmail.com / Test!2\n");
    printf("\t════════════════════════════════════════════════════════════════\n");
    printf("\t\n");
    printf("\t[1] Test Admin Panel Login\n");
    printf("\t[2] Test E-Shop Customer\n");
    printf("\t🎯 Choose: ");
    
    int test_choice;
    scanf("%d", &test_choice);
    
    if (test_choice == 1) {
        printf("\n\t🏪 Opening Admin Panel for testing...\n");
        system(ADMIN_EXE);
    } else {
        printf("\n\t🛒 Opening E-Shop for testing...\n");
        system(ESHOP_EXE);
    }
}

int main() {
    int choice;
    
    while (1) {
        show_menu();
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("\n\t❌ Invalid input! Try again.\n");
#ifdef _WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
            continue;
        }
        
        switch (choice) {
            case 1:
                launch_admin();
                break;
            
            case 2:
                launch_eshop();
                break;
            
            case 3:
                launch_both();
                break;
            
            case 4:
                launch_test_mode();
                break;
            
            case 5:
                clear_screen();
                printf("\n\t👋 Thanks for using E-Commerce System!\n");
                printf("\t🛍️ Come back anytime to manage your business!\n\n");
                return 0;
            
            default:
                printf("\n\t❌ Invalid choice! Please select 1-5.\n");
#ifdef _WIN32
                Sleep(1000);
#else
                sleep(1);
#endif
                break;
        }
    }
    
    return 0;
}
