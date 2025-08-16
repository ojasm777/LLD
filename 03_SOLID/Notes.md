# **Comprehensive Notes on SOLID Design Principles**

This guide explains the first three SOLID principles—Single Responsibility, Open/Closed, and Liskov Substitution—to help you write cleaner, more maintainable, and scalable code.

## **Introduction: Why Do We Need Design Principles? 🤔**

In real-world software projects, you can have thousands of classes. Without proper guidelines, the codebase can become a "big ball of mud"—a tangled mess where everything is tightly connected.

**Analogy:** Imagine a wall in your house with electrical wires, internet cables, and water pipes all bundled together without any organization. If one wire faults, finding and replacing it is a nightmare because you risk damaging everything else.

Similarly, in software, if your classes are tightly coupled (highly dependent on each other), you face major problems:

- **Low Maintainability:** Adding a new feature or fixing a bug becomes incredibly difficult and risky, often creating new bugs.
- **Poor Readability:** New developers will struggle to understand the code, slowing down development.
- **High Risk of Bugs:** A small change in one part of the system can have disastrous, unforeseen consequences elsewhere.

**SOLID principles** are a set of five guidelines, introduced by **Robert C. Martin (Uncle Bob)**, that help developers avoid these problems by creating robust and well-organized object-oriented designs.

---

## **The 5 SOLID Principles**

SOLID is an acronym for:

- **S** - **Single Responsibility Principle (SRP)**
- **O** - **Open/Closed Principle (OCP)**
- **L** - **Liskov Substitution Principle (LSP)**
- **I** - **Interface Segregation Principle (ISP)**
- **D** - **Dependency Inversion Principle (DIP)**

Let's dive deep into the first three.

---

## **S: Single Responsibility Principle (SRP) 🎯**

### **The Principle**

> "A class should have only one reason to change."

This means a class should have **one, and only one, job or responsibility.** All of its methods and attributes should be dedicated to fulfilling that single purpose.

### **Real-World Analogy 🚗**

A **TV remote** has one job: to control the TV. All its buttons (power, volume, channel) are related to that single responsibility. If you added buttons to control your air conditioner and refrigerator to the same remote, it would become complex, confusing, and hard to fix if something went wrong. That's a violation of SRP.

### **Example: Violation of SRP**

Imagine a `ShoppingCart` class that does everything:

- Calculates the total price of items.
- Prints an invoice for the customer.
- Saves the cart contents to a database.

~~~cpp
// Violation of SRP
class ShoppingCart {
public:
    void addProduct(Product p);
    double calculateTotalPrice(); // Responsibility 1: Business Logic
    void printInvoice();          // Responsibility 2: Presentation Logic
    void saveToDatabase();        // Responsibility 3: Data Persistence Logic
};
~~~

This class has **three reasons to change**:

1. The logic for price calculation might change (e.g., adding discounts).
2. The format of the invoice might change.
3. The database technology might change (e.g., moving from SQL to NoSQL).

### **Example: Following SRP**

To fix this, we break down the class into smaller classes, each with a single responsibility.

1. **`ShoppingCart` Class:** Only manages the items in the cart and calculates the total.

~~~cpp
class ShoppingCart {
public:
    void addProduct(Product p);
    double calculateTotalPrice();
};
~~~

2. **`InvoicePrinter` Class:** Only handles printing the invoice.

~~~cpp
class InvoicePrinter {
public:
    void printInvoice(ShoppingCart cart);
};
~~~

3. **`CartRepository` Class:** Only handles saving the cart to the database.

~~~cpp
class CartRepository {
public:
    void saveToDatabase(ShoppingCart cart);
};
~~~

Now, each class has only one reason to change. The system is cleaner, easier to understand, and safer to modify.

**Key Misconception:** SRP does **not** mean a class should only have one method. A class can have many methods, as long as they all contribute to its single responsibility.

---

## **O: Open/Closed Principle (OCP) 🔌**

### **The Principle**

> "Software entities (classes, modules, functions, etc.) should be open for extension, but closed for modification."

This means you should be able to **add new functionality** to a class without **changing its existing source code**.

### **Real-World Analogy 📱**

Your **smartphone** is a perfect example. The core operating system is "closed" for modification—you don't rewrite iOS or Android to add a feature. However, it is "open" for extension because you can install new apps from the App Store to add endless new capabilities.

### **How to Achieve OCP?**

OCP is primarily achieved through the use of **Abstraction (Interfaces or Abstract Classes) and Polymorphism.** You code against an abstraction, not a concrete implementation.

### **Example: Violation of OCP**

Imagine a `CartStorage` class that saves a cart to different places.

~~~cpp
// Violation of OCP
class CartStorage {
public:
    void save(ShoppingCart cart, string storageType) {
        if (storageType == "sql_db") {
            // Logic to save to SQL Database
        } else if (storageType == "mongo_db") {
            // Logic to save to MongoDB
        } else if (storageType == "file") {
            // Logic to save to a file
        }
    }
};
~~~

To add a new storage type (e.g., "Cassandra"), you **must modify** this class by adding another `else if` block. This is a violation of the "closed for modification" rule.

### **Example: Following OCP**

We introduce an abstraction (an interface) and create separate classes for each storage type.

1. **Create an Interface (Abstract Class):**

~~~cpp
class IPersistence {
public:
    virtual void save(ShoppingCart cart) = 0;
};
~~~

2. **Create Concrete Implementations:**

~~~cpp
class SqlPersistence : public IPersistence {
public:
    void save(ShoppingCart cart) override { /* Save to SQL */ }
};

class MongoPersistence : public IPersistence {
public:
    void save(ShoppingCart cart) override { /* Save to MongoDB */ }
};
~~~

Now, to add support for Cassandra, you simply create a **new class** `CassandraPersistence` that implements the `IPersistence` interface. No existing code needs to be touched. The system is **open to extension** (new persistence types) but **closed to modification**.

---

## **L: Liskov Substitution Principle (LSP) 🏦**

### **The Principle**

> "Subclasses should be substitutable for their base classes."

In simpler terms, if a class `B` is a subclass of `A`, you should be able to use an object of class `B` anywhere an object of class `A` is expected, without the program crashing or behaving unexpectedly. The subclass must honor the "contract" of the superclass and not narrow down its capabilities.

### **Real-World Analogy 💡**

A **child inheriting money** from a parent. The parent's contract is that the money can be spent. If the child inherits the money but puts it in an unbreakable piggy bank, they have violated the contract. You can no longer substitute the child for the parent in the context of "spending the inheritance" without causing an issue.

### **Example: Violation of LSP**

This principle is often violated with incorrect inheritance hierarchies. Consider a banking system:

1. **Base `Account` Class:** Has `deposit` and `withdraw` methods.

~~~cpp
class Account {
public:
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
};
~~~

2. **Subclasses:** `SavingsAccount` and `CurrentAccount` correctly implement both methods.

3. **The Problem Child:** A new account type, `FixedDepositAccount`, is created. It also inherits from `Account`. However, you **cannot withdraw** from a fixed deposit. To handle this, the developer makes the `withdraw` method throw an exception.

~~~cpp
class FixedDepositAccount : public Account {
public:
    void deposit(double amount) override { /* Logic here */ }
    void withdraw(double amount) override {
        throw logic_error("Withdrawal not allowed from Fixed Deposit!");
    }
};
~~~

A client function that works with a list of `Account` objects will now crash when it encounters a `FixedDepositAccount` and tries to call `withdraw`. The `FixedDepositAccount` is **not substitutable** for `Account`, thus violating LSP.

### **Example: Following LSP**

The problem is the inheritance model. A `FixedDepositAccount` is not truly an `Account` in the same way a `SavingsAccount` is, because it doesn't support the full contract. We solve this by creating a better hierarchy.

1. **Create a more specific base interface:**

~~~cpp
class DepositableAccount {
public:
    virtual void deposit(double amount) = 0;
};
~~~

2. **Create an interface that extends the first one:**

~~~cpp
class WithdrawableAccount : public DepositableAccount {
public:
    virtual void withdraw(double amount) = 0;
};
~~~

3. **Implement the Correct Interfaces:**
   - `SavingsAccount` and `CurrentAccount` will inherit from `WithdrawableAccount`.
   - `FixedDepositAccount` will inherit from `DepositableAccount`.

Now, no class promises functionality it cannot deliver. A client function expecting a `WithdrawableAccount` will never receive a `FixedDepositAccount` by mistake, and the substitution principle holds true.
