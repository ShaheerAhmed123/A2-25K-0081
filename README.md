# Assignment 02: Advanced Object-Oriented Programming in C++

**Course:** Object-Oriented Programming (FAST-NUCES)  
**Instructor:** Talha Shahid  
**Due Date:** 18-May-2026  
**Marks:** 40

---

## Project Overview

This assignment extends Assignment 01 by implementing advanced OOP concepts to build a **Vehicle Management System** with messaging capabilities. The system models a marketplace where sellers can list vehicles, buyers can inquire, and an admin oversees transactions.

### Key Features
- Vehicle inventory management (Cars and Bikes)
- Multi-user system (Buyers, Sellers, Admins)
- Secure messaging between users
- Price comparison and sorting utilities
- Stock filtering by price range

---

## OOP Concepts Implemented

### 1. **Inheritance** (08 Marks)

#### Hierarchy Structure

```
Vehicle (Base Class)
├── Car (Derived)
└── Bike (Derived)

User (Abstract Base Class)
├── Buyer (Derived)
├── Seller (Derived with Multiple Inheritance)
│   └── Also inherits from: Manageable
└── Admin (Derived)
```

#### Implementation Code

**Vehicle Hierarchy:**
```cpp
class Vehicle : public Displayable {
private:
    float price;
protected:
    string brand;
    int year;
public:
    static int count;
    Vehicle(string b, int y, float p) : brand(b), year(y), price(p) {
        count++;
    }
    virtual void show() const override { /* ... */ }
    float getPrice() const { return price; }
};

class Car : public Vehicle {
    string type;
public:
    Car(string b, int y, float p, string t) : Vehicle(b, y, p), type(t) {}
    void show() const override {
        cout << "[CAR] ";
        Vehicle::show();
        cout << " | " << type << endl;
    }
};

class Bike : public Vehicle {
public:
    Bike(string b, int y, float p) : Vehicle(b, y, p) {}
    void show() const override {
        cout << "[BIKE] ";
        Vehicle::show();
        cout << endl;
    }
};
```

**User Hierarchy:**
```cpp
class User : public Communicator {
private:
    string email;
protected:
    string name;
public:
    User(string n, string e) : name(n), email(e) {}
    virtual void profile() = 0;  // Pure virtual (Abstraction)
    friend void verifyUser(const User& u);
    virtual ~User() {}
};

class Buyer : public User {
public:
    Buyer(string n, string e) : User(n, e) {}
    void send(string m) override { /* ... */ }
    void profile() override { /* ... */ }
};

class Seller : public User, public Manageable {  // MULTIPLE INHERITANCE
    Vehicle* stock[5];
    int n;
public:
    void add() override { /* ... */ }
    void del() override { /* ... */ }
    void post(Vehicle* v) { /* ... */ }
};
```

#### Justification
- **6 Inheritance Relationships** implemented across two hierarchies
- **Inheritance promotes code reuse**: Common vehicle properties (price, brand, year) are defined once in the base class
- **Supports extensibility**: New vehicle types can be added by simply inheriting from Vehicle
- **Multiple inheritance in Seller** allows combining user communication features with inventory management

---

### 2. **Polymorphism** (05 Marks)

#### Function Overloading & Overriding

**Virtual Function Overriding:**
```cpp
// Base class
class Vehicle : public Displayable {
public:
    virtual void show() const override {
        cout << brand << " (" << year << ") - $" << price;
    }
};

// Derived classes override the behavior
class Car : public Vehicle {
public:
    void show() const override {
        cout << "[CAR] ";
        Vehicle::show();
        cout << " | " << type << endl;
    }
};

class Bike : public Vehicle {
public:
    void show() const override {
        cout << "[BIKE] ";
        Vehicle::show();
        cout << endl;
    }
};
```

**Function Overloading in Seller:**
```cpp
// Same function name, different parameters
void listStock() {
    for (int i = 0; i < n; i++)
        stock[i]->show();
}

void listStock(float maxPrice) {  // Overloaded version
    for (int i = 0; i < n; i++)
        if (stock[i]->getPrice() <= maxPrice)
            stock[i]->show();
}
```

**Usage Example:**
```cpp
Seller seller("Ali Motors", "alimotors123@gmail.com");
seller.post(myCar);
seller.post(myBike);

cout << "--- Full Inventory ---\n";
seller.listStock();        // Calls version with no parameters

cout << "\n--- Under $2000 ---\n";
seller.listStock(2000);    // Calls overloaded version with float parameter
```

#### Benefits
- **Runtime Polymorphism**: Different vehicle types display differently based on actual type, not pointer type
- **Code Flexibility**: The same interface (show()) works for all vehicle types
- **Function Overloading**: `listStock()` adapts to different use cases without creating multiple functions

---

### 3. **Abstraction** (10 Marks)

#### Abstract Base Classes

**Header File Design: Displayable.h**
```cpp
// Abstract class for display functionality
class Displayable {
public:
    virtual void show() const = 0;  // Pure virtual function
    virtual ~Displayable() {}
};
```

**Header File Design: Communicator.h**
```cpp
// Abstract class for communication
class Communicator {
public:
    virtual void send(string message) = 0;  // Pure virtual function
    virtual ~Communicator() {}
};
```

**Header File Design: Manageable.h**
```cpp
// Abstract class for inventory management
class Manageable {
public:
    virtual void add() = 0;   // Pure virtual function
    virtual void del() = 0;   // Pure virtual function
    virtual ~Manageable() {}
};
```

**Implementation in Source File:**
```cpp
class Vehicle : public Displayable {
public:
    virtual void show() const override {
        cout << brand << " (" << year << ") - $" << price;
    }
};

class User : public Communicator {
public:
    virtual void send(string m) = 0;  // Must be implemented by derived classes
    virtual void profile() = 0;
};

class Seller : public User, public Manageable {
public:
    void add() override {
        cout << "Stock slot allocated.\n";
    }
    void del() override {
        cout << "Stock slot freed.\n";
    }
    void send(string m) override {
        cout << name << " (Seller) replied: " << m << endl;
    }
};
```

#### Design Benefits
- **Clear Contracts**: Abstract classes define what derived classes must implement
- **Hide Implementation Details**: Users see only essential features (show, send, add, del)
- **Loose Coupling**: Client code depends on abstract interfaces, not concrete classes
- **Extensibility**: New vehicle types can be added without changing existing code

---

### 4. **Operator Overloading** (07 Marks)

#### Implemented Operators

**1. Equality Operator (==)**
```cpp
// Compares vehicles by manufacturing year
bool operator==(const Vehicle& v) const { 
    return this->year == v.year; 
}

// Usage
if (*myBike == *myCar)
    cout << "Same year manufacture\n";
```

**Reasoning:** Allows natural comparison syntax. Reading `car1 == car2` is intuitive for comparing manufacturing years rather than `car1.getYear() == car2.getYear()`.

---

**2. Addition Operator (+)**
```cpp
// Sums the prices of two vehicles
float operator+(const Vehicle& v) const { 
    return this->price + v.price; 
}

// Usage
cout << "Combined value: $" << (*myCar + *myBike) << endl;
// Output: Combined value: $36200
```

**Reasoning:** Addition semantics fit naturally—adding two vehicles means summing their prices. This is more intuitive than `myCar.getPrice() + myBike.getPrice()`.

---

**3. Less-Than Operator (<)**
```cpp
// Compares vehicles by price (useful for sorting)
bool operator<(const Vehicle& v) const { 
    return this->price < v.price; 
}

// Usage
if (*myBike < *myCar)
    cout << "\nBike is much cheaper than the car.\n";
```

**Reasoning:** Enables sorting vehicles by affordability. Comparison operators allow vehicles to be used in standard sorting algorithms.

---

#### Output Example
```
Bike is much cheaper than the car.
Combined value: $36200
```

---

### 5. **Friend Functions** (05 Marks)

#### Implementation

**Friend Function 1: debugMsg (Message class)**
```cpp
class Message {
    string text;
    int fromID, toID;
public:
    Message(int f, int t, string txt) : fromID(f), toID(t), text(txt) {}
    
    friend void debugMsg(const Message& m);  // Declares friend function
};

void debugMsg(const Message& m) {
    cout << "[Log] Message: " << m.text << endl;
}

// Usage
Message msg(101, 202, "I'm interested in the Civic.");
debugMsg(msg);  // Access private data
// Output: [Log] Message: I'm interested in the Civic.
```

**Justification:** 
- Logging/debugging requires access to private message content
- Friend function is minimal and specific to this purpose
- Better than making text public or adding unnecessary getter functions

---

**Friend Function 2: priceDiff (Vehicle class)**
```cpp
class Vehicle {
private:
    float price;
    // ...
public:
    friend void priceDiff(const Vehicle& a, const Vehicle& b);
};

void priceDiff(const Vehicle& a, const Vehicle& b) {
    cout << "[Price Check] Difference: $" << abs(a.price - b.price) << endl;
}

// Usage
priceDiff(*myCar, *myBike);
// Output: [Price Check] Difference: $33800
```

**Justification:**
- Requires access to private price data from two separate objects
- Friend function provides clean interface for price comparison utility
- Avoids exposing getPrice() which might allow invalid modifications

---

**Friend Function 3: verifyUser (User class)**
```cpp
class User {
private:
    string email;
protected:
    string name;
public:
    friend void verifyUser(const User& u);
};

void verifyUser(const User& u) {
    cout << "[Security] Verified contact: " << u.email << endl;
}

// Usage
verifyUser(seller);
// Output: [Security] Verified contact: alimotors123@gmail.com
```

**Justification:**
- Security verification requires access to private email data
- Friend function keeps sensitive data private while enabling security checks
- Clear separation of concern (verification logic outside User class)

---

## Program Output

```
--- Full Inventory ---
[CAR] Honda Civic (2024) - $35000 | Sedan
[BIKE] Honda CD70 (2023) - $1200

--- Under $2000 ---
[BIKE] Honda CD70 (2023) - $1200

[Price Check] Difference: $33800
[Security] Verified contact: alimotors123@gmail.com

Bike is much cheaper than the car.
Combined value: $36200

Total vehicles created: 2

[Log] Message: I'm interested in the Civic.
Mr. Khan (Buyer) sent: Can I see the car tomorrow?
Ali Motors (Seller) replied: Sure, come at 3 PM.
ADMIN NOTICE: All transactions are logged for safety.
```

---

## File Structure

```
Assignment_02/
├── README.md                  # This documentation
├── A2.cpp                    # Main implementation
├── Displayable.h             # Abstract base for display
├── Communicator.h            # Abstract base for messaging
├── Manageable.h              # Abstract base for inventory
```

---

## Design Principles Applied

| Principle | Implementation |
|-----------|-----------------|
| **DRY (Don't Repeat Yourself)** | Common vehicle properties in base Vehicle class |
| **SOLID - Single Responsibility** | Each class has one primary responsibility |
| **SOLID - Open/Closed** | Open for extension, closed for modification |
| **SOLID - Liskov Substitution** | Derived classes can replace base classes |
| **Encapsulation** | Private data with controlled access |
| **Loose Coupling** | Abstract interfaces reduce dependencies |

---

## Compilation & Execution

**Compile:**
```bash
g++ -std=c++17 A2.cpp -o A2
```

**Run:**
```bash
./A2
```

---

## Key Concept Summary

✅ **Inheritance** - 6 relationships across Vehicle and User hierarchies  
✅ **Polymorphism** - Virtual functions and operator overloading  
✅ **Abstraction** - Pure virtual functions in abstract base classes  
✅ **Operator Overloading** - ==, +, < for intuitive operations  
✅ **Friend Functions** - debugMsg(), priceDiff(), verifyUser()  
✅ **Multiple Inheritance** - Seller class inherits from User and Manageable  
✅ **Proper Memory Management** - Dynamic allocation and cleanup  
