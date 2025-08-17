# Advanced SOLID Principles: A Comprehensive Guide

This guide continues from the first three SOLID principles, offering a deep dive into advanced guidelines for **Liskov Substitution**, and then covering **Interface Segregation** and **Dependency Inversion**.

---

## A Deeper Look at Liskov Substitution Principle (LSP) 🧐

LSP is simple in theory but is the most frequently violated principle in practice. To ensure a subclass is truly substitutable for its parent, it must adhere to a strict contract. These guidelines, broken into three rule sets, ensure that contract is never broken.

**Core Idea Recap:**  
A child class must not just inherit from a parent class; it must behave like the parent class from the client's perspective. It should extend the parent's functionality, never narrow it down.

---

### 1. The Signature Rule

This governs the method signatures (arguments, return types, exceptions) between the parent and child class methods.

**Method Argument Rule:**  
A child's overriding method should accept the same or a broader (more generic) type of arguments than the parent method.

- *Why?* A client is programmed to work with the parent's contract. If the parent method accepts an `Animal`, the client might pass a `Dog` or a `Cat`. If the child narrows the argument to only accept a `Dog`, the client's code will break when it tries to pass a `Cat`.  
Most modern languages like C++ and Java enforce identical argument types to prevent this confusion.

**Return Type Rule (Covariance):**  
A child's overriding method can return the same or a narrower (more specific) type than the parent method.

- *Why?* If a client expects an `Animal` back from a method, receiving a `Dog` (which is an `Animal`) is perfectly fine and doesn't break the contract. The child is providing a more specific version of what was promised.

**Exception Rule:**  
A child's overriding method should not throw exceptions of a broader type than the parent method. It can throw the same exception type or a narrower (more specific) subtype.

- *Why?* The client writes catch blocks based on the exceptions the parent contract promises to throw (e.g., `catch (LogicError)`). If the child throws a completely different or broader exception (e.g., `RuntimeException`), the client's catch block will miss it, causing the program to crash.

---

### 2. The Property Rule

This governs the internal state and rules (invariants) of the classes.

**Class Invariant Rule:**  
An invariant is a rule about a class's state that must always be true (e.g., balance in a `BankAccount` can never be negative). A child class must maintain all invariants of its parent class.

- *Why?* The client relies on these invariants. If a `BankAccount` promises a non-negative balance, but a child class like `CheatAccount` allows the balance to go negative, it breaks the client's expectation and can lead to critical failures. The child can make the rule stricter (e.g., balance must be `> 10`), but never weaker.

**History Constraint Rule:**  
A child class cannot introduce state changes that the parent wouldn't allow. Properties in the parent class that are immutable (unchangeable) must remain immutable in the child class.

- *Why?* This is best seen in the `FixedDepositAccount` example from the previous notes. The parent `Account` class had a history constraint: *"withdrawals are always possible."* The `FixedDepositAccount` child class broke this history by disallowing withdrawals, thus violating LSP. It changed the fundamental behavior promised by the parent.

---

### 3. The Method Rule

This governs the conditions required before and after a method executes.

**Pre-condition Rule:**  
A pre-condition is a condition that must be true before a method is called. A child's overriding method cannot have stronger (more restrictive) pre-conditions than the parent method. It can have the same or weaker pre-conditions.

- *Why?* Imagine a parent `User` class has a `setPassword` method with a pre-condition: *"password must be >= 8 characters."* A client knows this and will always provide a valid password. If a child `AdminUser` class strengthens this pre-condition to *"password must be >= 8 characters AND contain a special character,"* the client's valid 8-character password will now fail, breaking the contract. The child can, however, weaken the pre-condition (e.g., ">= 6 characters"), as the client's 8-character password would still be valid.

**Post-condition Rule:**  
A post-condition is a condition that must be true after a method has executed. A child's overriding method cannot have weaker post-conditions than the parent method. It can have the same or stronger post-conditions.

- *Why?* A parent `Car` class has a `brake()` method with a post-condition: *"speed must decrease."* A client relies on this. If a child `ElectricCar` class overrides `brake()` and fulfills this post-condition, it can also add a stronger post-condition: *"speed must decrease AND battery must recharge."* This is fine. But if it weakened the post-condition by, for instance, not decreasing the speed, it would be a dangerous violation of LSP.

---

## I: Interface Segregation Principle (ISP) 🧩

**The Principle:**  
*"Clients should not be forced to depend on interfaces they do not use."*

This means it's better to have many small, specific interfaces than one large, general-purpose ("fat") interface.

### Real-World Analogy 🍴
A Swiss Army Knife is a "fat interface." It has a knife, a screwdriver, scissors, a corkscrew, etc. If all you want to do is cut something, you are still forced to carry around the weight and complexity of all the other tools you don't need. It would be better to have a simple knife if that's the only tool required.

---

### Example: Violation of ISP

Imagine a "fat" Shape interface for a geometry application.

~~~cpp
// "Fat" interface violating ISP
class IShape {
public:
    virtual double calculateArea() = 0;
    virtual double calculateVolume() = 0; // Problematic method
};
~~~

A `Square` class can implement `calculateArea()`, but what about `calculateVolume()`?  
A 2D square has no volume. It is forced to implement a method it doesn't need, often by throwing an exception or returning `0`.

A `Cube` class can implement both methods correctly.

The `Square` class is being forced to depend on a method (`calculateVolume`) it does not use.

---

### Example: Following ISP

We segregate (split) the fat interface into smaller, more logical ones.

**Create Role-Specific Interfaces:**

~~~cpp
class I2DShape {
public:
    virtual double calculateArea() = 0;
};

class I3DShape {
public:
    virtual double calculateVolume() = 0;
};
~~~

**Implement Only What's Needed:**

~~~cpp
class Square : public I2DShape { ... } // Only needs to implement calculateArea()

class Cube : public I2DShape, public I3DShape { ... } // Implements both
~~~

Now, no class is forced to implement methods it doesn't need. The design is cleaner and avoids unnecessary exceptions.

---

## D: Dependency Inversion Principle (DIP) 🔄

**The Principle:**  
A. High-level modules should not depend on low-level modules. Both should depend on abstractions (e.g., interfaces).  
B. Abstractions should not depend on details. Details (concrete implementations) should depend on abstractions.

This principle essentially "inverts" the traditional flow of dependency. Instead of the main business logic (high-level) depending directly on technical details like a specific database (low-level), both now depend on a shared contract (an interface).

---

### Real-World Analogy 🧑‍💼

A CEO (high-level module) doesn't talk directly to individual developers (low-level modules). That would be inefficient and chaotic. Instead, there's an abstraction in between: a **Manager**.

- The CEO gives commands to the Manager (depends on the Manager interface).  
- The developers take tasks from the Manager (also depend on the Manager interface).  

The dependency is inverted. If a developer leaves, the CEO doesn't need to know or care; the Manager handles it. The system is decoupled.

---

### Example: Violation of DIP

A `UserService` (high-level business logic) directly creates and uses a `MySqlDatabase` object (low-level detail).

~~~cpp
// Violation of DIP
class MySqlDatabase {
public:
    void save(string data) { /* SQL-specific logic */ }
};

class UserService {
private:
    MySqlDatabase db; // Direct dependency on a concrete low-level class
public:
    void storeUser(string name) {
        db.save(name);
    }
};
~~~

The problem? If you want to switch from MySQL to MongoDB, you must modify the `UserService` class. This violates the **Open/Closed Principle**. The high-level module is tightly coupled to the low-level detail.

---

### Example: Following DIP

We introduce an abstraction that both modules will depend on.

**Create a Database Interface:**

~~~cpp
class IDatabase {
public:
    virtual void save(string data) = 0;
};
~~~

**Concrete Implementations Depend on the Interface:**

~~~cpp
class MySqlDatabase : public IDatabase { ... };
class MongoDatabase : public IDatabase { ... };
~~~

**High-Level Module Depends on the Interface (via Dependency Injection):**

~~~cpp
class UserService {
private:
    IDatabase& db; // Depends on the abstraction, not the detail
public:
    // The dependency is "injected" via the constructor
    UserService(IDatabase& database) : db(database) {}

    void storeUser(string name) {
        db.save(name);
    }
};
~~~

Now, the `UserService` doesn't know or care if it's talking to MySQL or MongoDB. It only knows it's talking to something that fulfills the `IDatabase` contract.  
To switch databases, you just pass a different object into the `UserService` constructor. No modification to `UserService` is needed.

---

### Key Relationship

**DIP is the key enabler of the Open/Closed Principle.**  
By depending on abstractions, our high-level modules become **open to extension** (new database types) but **closed to modification**.
