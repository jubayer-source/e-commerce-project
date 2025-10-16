# 🛒 E-Commerce Shopping Cart System

A comprehensive command-line based e-commerce shopping cart system developed in C programming language with advanced features, professional UI, and **dedicated admin panel for shop owners**.

## 🌟 Features

### 👥 **Dual System Architecture**
- **Customer Interface** (`e-shop.c`) - Shopping experience for customers
- **Admin Panel** (`admin-panel.c`) - Management system for shop owners
- **Synchronized Database** - Real-time data sync between systems

### 🔐 User Management
- **Customer Registration** with comprehensive validation
- **Shop Owner Registration** with dedicated admin accounts
- **Secure Login** with password hashing for both systems
- **Profile Management** with user/admin dashboards

### 🏪 Shop Management
- **11+ Different Shops** with varied product categories
- **Self-Service Shop Registration** - Shop owners can register independently
- **Dynamic Product Management** - Shop owners add/edit their own products
- **Real-time Stock Management** across both systems
- **Variable Items per Shop** (unlimited with admin panel)

### 🛒 Shopping Cart
- **Add/Remove Items** with quantity management
- **Update Quantities** with stock validation
- **Cart Persistence** during session
- **Professional Invoice Generation**

### 📊 Order Management
- **Order History Tracking** with unique Order IDs
- **Date/Time Stamps** for all transactions
- **Multi-item Order Support**
- **Order Summary & Receipt**

### 🎨 User Interface
- **Professional ASCII Art** design
- **Loading Animations** for better UX
- **Proper Column Alignment** for product display
- **Unicode Box Drawings** for modern look
- **Emoji Integration** for visual appeal

## 🏗️ System Architecture

### Data Structures
```c
struct details {
    char uname[50];
    int age;
    char password[100];  // Hashed password
    char email[100];
    char mobile[15];
};

struct Shops {
    char shop[100];
    char item_name[7][50];  // Up to 6 items per shop
    int item_price[7];
    int item_qty[7];
};

struct CartItem {
    char name[50];
    int price;
    int quantity;
};
```

### File Management
- **users.dat** - User account information
- **products.dat** - Shop and product data
- **order_history.dat** - Order tracking and history

## 🚀 Getting Started

### Prerequisites
- GCC compiler
- Linux/Unix environment (recommended)
- Terminal with Unicode support

### Installation & Compilation
```bash
# Clone the repository
git clone https://github.com/jubayer-source/e-commerce-project.git
cd e-commerce-project

# Compile the customer interface
gcc -o e-shop e-shop.c

# Compile the admin panel
gcc -o admin-panel admin-panel.c

# Compile the sync tool (optional)
gcc -o sync-tool sync-tool.c

# Run the customer application
./e-shop

# Run the admin panel (separate terminal)
./admin-panel
```

## 🏢 **NEW: Admin Panel for Shop Owners**

### 🎯 **What's New?**
Shop owners can now **independently manage their business** without manual file editing!

### 🔑 **Admin Features:**
- **🏪 Shop Registration** - Register your shop with owner details
- **📦 Product Management** - Add, edit, delete products in real-time
- **📊 Sales Analytics** - View revenue, best sellers, growth metrics
- **💰 Revenue Reports** - Daily, weekly, monthly financial analysis
- **🛒 Order Tracking** - Monitor customer orders for your products
- **🔄 Auto-Sync** - Automatically sync with main customer system

### 📋 **How Shop Owners Can Get Started:**

1. **Register Your Shop:**
   ```bash
   ./admin-panel
   # Choose [2] Register Shop
   # Enter your shop details
   # Get unique Shop ID
   ```

2. **Add Your Products:**
   ```bash
   # Login to admin panel
   # Go to Product Management
   # Add products with price and stock
   # Sync with main system
   ```

3. **Monitor Your Business:**
   ```bash
   # View sales analytics
   # Check revenue reports
   # Track customer orders
   # Manage inventory
   ```

## 📋 Available Shops & Products

### 🏪 Shop Categories
1. **Aarong** - Traditional Bangladeshi clothing
2. **Yellow** - Casual wear and fashion
3. **Naviforce** - Watch collection
4. **Dukpion** - Sunglasses and eyewear
5. **Sailor** - Men's fashion and accessories
6. **Cats Eye** - Formal wear
7. **PRAN Store** - Food and beverages
8. **Walton** - Electronics and appliances
9. **Radhuni** - Spices and cooking ingredients
10. **HP** - Computer and technology products
11. **Bashundhara Group** - Household essentials

## 🎯 Usage Guide

### 1. Account Creation
- Choose option [1] for Signup
- Provide valid email and strong password
- Automatic login after successful registration

### 2. Shopping
- Browse shops or search all items
- Add products to cart with desired quantities
- Real-time stock validation

### 3. Cart Management
- View cart contents and totals
- Update quantities or remove items
- Stock automatically returns on item removal

### 4. Order Processing
- Confirm orders with invoice generation
- Unique Order ID assignment
- Order history tracking

## 💻 Technical Features

### Security
- **Password Hashing** using XOR encryption
- **Input Validation** for all user inputs
- **Email Format Verification**
- **Mobile Number Validation** (Bangladeshi format)

### Performance
- **Efficient File I/O** operations
- **Dynamic Memory Management**
- **Optimized Search Algorithms**
- **Real-time Data Persistence**

### Error Handling
- **Comprehensive Input Validation**
- **File Operation Error Checking**
- **User-friendly Error Messages**
- **Graceful Error Recovery**

## 📊 System Statistics

- **Total Lines of Code:** ~1300+
- **Functions:** 25+ modular functions
- **Data Files:** 3 persistent storage files
- **Supported Users:** Unlimited
- **Supported Products:** Variable per shop
- **Order Capacity:** Unlimited

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 👨‍💻 Developer

**Md. Jubayer Ahmad**
- GitHub: [@jubayer-source](https://github.com/jubayer-source)
- Email: jubayer@example.com

## 🙏 Acknowledgments

- Thanks to all beta testers
- Special thanks to the open source community
- Inspired by modern e-commerce platforms

---

**মাশাআল্লাহ! একটি সম্পূর্ণ বাংলাদেশী E-commerce Solution! 🇧🇩**

Made with ❤️ in Bangladesh 🇧🇩
