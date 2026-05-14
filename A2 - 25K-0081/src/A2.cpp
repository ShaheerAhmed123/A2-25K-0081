#include <iostream>
#include <string>
#include <cmath>
#include "Displayable.h"
#include "Manageable.h"
#include "Communicator.h"

using namespace std;

// messaging between users
class Message {
    string text;
    int fromID, toID;
public:
    Message(int f, int t, string txt) : fromID(f), toID(t), text(txt) {}

    // friend function to peek at message content
    friend void debugMsg(const Message& m);
};

void debugMsg(const Message& m) {
    cout << "[Log] Message: " << m.text << endl;
}

// vehicle hierarchy 
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

    virtual void show() const override {
        cout << brand << " (" << year << ") - $" << price;
    }

    // Reason: Allows comparing two vehicles by their manufacturing year.
    // Overloading == makes the code read naturally: if (car1 == car2)
    bool operator==(const Vehicle& v) const { return this->year == v.year; }

    // Reason: Adding two vehicles means summing their prices.
    // Using + instead of .getPrice() + .getPrice() makes expressions like total = car + bike intuitive.
    float operator+(const Vehicle& v) const { return this->price + v.price; }

    // Reason: useful for sorting vehicles by affordability.
    // Overloading < allows comparisons.
    bool operator<(const Vehicle& v) const { return this->price < v.price; }

    float getPrice() const { return price; }

    // friend function to calculate price difference 
    friend void priceDiff(const Vehicle& a, const Vehicle& b);

    virtual ~Vehicle() {}
};

int Vehicle::count = 0;

void priceDiff(const Vehicle& a, const Vehicle& b) {
    cout << "[Price Check] Difference: $" << abs(a.price - b.price) << endl;
}

// derived classes
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

// user hierarchy
class User : public Communicator {
private:
    string email;          // private for friend function
protected:
    string name;
public:
    User(string n, string e) : name(n), email(e) {}

    virtual void profile() = 0;

    friend void verifyUser(const User& u);

    virtual ~User() {}
};

void verifyUser(const User& u) {
    cout << "[Security] Verified contact: " << u.email << endl;
}

class Buyer : public User {
public:
    Buyer(string n, string e) : User(n, e) {}

    void send(string m) override {
        cout << name << " (Buyer) sent: " << m << endl;
    }

    void profile() override {
        cout << "Buyer Name: " << name << endl;
    }
};

class Seller : public User, public Manageable {
    Vehicle* stock[5];
    int n;
public:
    Seller(string n, string e) : User(n, e), n(0) {}

    void add() override {
        cout << "Stock slot allocated.\n";
    }

    void del() override {
        cout << "Stock slot freed.\n";
    }

    void post(Vehicle* v) {
        if (n < 5) stock[n++] = v;
        else cout << "Stock full!\n";
    }

    // overloaded listStock
    void listStock() {
        for (int i = 0; i < n; i++)
            stock[i]->show();
    }

    void listStock(float maxPrice) {
        for (int i = 0; i < n; i++)
            if (stock[i]->getPrice() <= maxPrice)
                stock[i]->show();
    }

    void send(string m) override {
        cout << name << " (Seller) replied: " << m << endl;
    }

    void profile() override {
        cout << "Seller: " << name << " | Ads: " << n << endl;
    }
};

class Admin : public User {
public:
    Admin(string n, string e) : User(n, e) {}

    void send(string m) override {
        cout << "ADMIN NOTICE: " << m << endl;
    }

    void profile() override {
        cout << "Admin: " << name << " (Global Access)\n";
    }
};

// main
int main() {
    // create vehicles
    Vehicle* myCar = new Car("Honda Civic", 2024, 35000, "Sedan");
    Vehicle* myBike = new Bike("Honda CD70", 2023, 1200);

    // seller posts them
    Seller seller("Ali Motors", "alimotors123@gmail.com");
    seller.post(myCar);
    seller.post(myBike);

    cout << "--- Full Inventory ---\n";
    seller.listStock();

    cout << "\n--- Under $2000 ---\n";
    seller.listStock(2000);

    // friend functions
    priceDiff(*myCar, *myBike);
    verifyUser(seller);

    // operator overloading
    if (*myBike < *myCar)
        cout << "\nBike is much cheaper than the car.\n";
    cout << "Combined value: $" << (*myCar + *myBike) << endl;

    // static member
    cout << "\nTotal vehicles created: " << Vehicle::count << endl;

    // messaging using Message class
    Message msg(101, 202, "I'm interested in the Civic.");
    debugMsg(msg);

    // chat 
    Buyer buyer("Mr. Khan", "khan123@gmail.com");
    buyer.send("Can I see the car tomorrow?");
    seller.send("Sure, come at 3 PM.");

    // admin 
    Admin admin("System", "admin@gmail.com");
    admin.send("All transactions are logged for safety.");

    // cleanup
    delete myCar;
    delete myBike;

    return 0;
}