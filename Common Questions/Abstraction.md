Abstraction means exposing what someone needs to use, while hiding how it is actually done.

Imagine you are designing a FoodDeliveryApp. When a user places an order, the app might have to validate the restaurant, calculate delivery charges, contact a payment service, create a delivery task, update several databases, and send notifications. But the person using your OrderService should not need to know all of that.
That separation is abstraction.

### Difference between Abstraction vs Encapsulation

### Why is abstraction important in LLD?
Suppose we are designing a ride-booking application. The part of the application that books a driver should not need to know whether the driver is selected using a nearest-driver algorithm, a priority-based algorithm, or some external service. It only needs a well-defined interface through which it can request a driver. The implementation details can remain hidden behind that interface. This reduces coupling and makes the system easier to change and extend.

### How do we do abstraction in C++?
In C++, we can create abstractions using the abstract class. 
``` cpp
class PaymentMethod {
public:
    virtual void pay(double amount) = 0;
};
```
`=0` makes the pay method a pure virtual function. Pure virtual functions define that every implementation of this class must have a this kind of method, without actually writing its definition itself. So the above method means that every Payment Method must know how to pay. 

Even a single pure virtual function's presence makes the class abstract. Here, PaymentMethod exposes the pay() operation as a contract, while the concrete classes provide the actual implementation. The user of PaymentMethod only needs to know that pay() exists and what it is supposed to do; they do not need to know how a particular payment method performs the payment internally.

> Note: we cannot create an object of an abstract class because it contains at least one method whose implementation is not present, but we can create a pointer or reference of the abstract base-class type that refers to an object of a concrete derived class.

Example of a concrete implementation:
``` cpp
class CreditCard : public PaymentMethod {
public:
    void pay(double amount) override {
        // credit card payment
    }
};

int main(){
    CreditCard card;

    PaymentMethod* ptr = &card;
    PaymentMethod& ref = card;
}

// Both ptr and ref have the abstract base-class type, but the actual object is a CreditCard.
```

### Does abstraction always require inheritance?
Asnwer is NO. Abstraction is a design principle in LLD. Inheritance, abstract classes and polymorphism are the mere tools that help in acheiving this.

Example of abstraction without using abstract classes.
```cpp
class Coffee{
public:
    void makeCoffee() {
        heatWater();
        grindBeans();
        brew();
    }
private:
    void heatWater(){/*Actual Implementation*/}
    void grindBeans() {/*Actual Implementation*/}
    void brew() {/*Actual Implementation*/}
};
```
User is only exposed with the makeCoffee function, he doesn't know how the watet is heated, or beans are grinded.


### Can an abstract class have normal / implemented functions
Yes, abstract classes can have normal functions too, and they will still be abstract. This functionality is useful, when many subclasses have a common behavior, so rather than implementing that behavior separately for each class, we can directly implement it in the parent class.
An abstract class can contain normal implemented methods, data members, constructors, and destructors. It remains abstract as long as it has at least one pure virtual function.

### What is an interface? How is it different from abstract class? 
An interface is generally an abstract class designed purely as a contract. It means that an abstract class may contain normal methods as well, but interface is generally stricter in such cases, and allows only virtual functions. C++ doesn't have any keyword like interface, we create it through abstract classes only.

Rules of interface:
- It should have pure virtual functions.
- You cannot create an object of the interface directly.
- A concrete derived class must implement all pure virtual functions.
- You normally use public inheritance - this is not a hard coded rule but is preferred because if we use any other access specifier, it would restrict / narrow down the scope of the data members / methods present in the interface, which breaks the Liskov's Substitution Principle.
- The interface normally does not contain implementation-specific state. Do not put things like emailAddress, retryCount, etc., in it.
- Give the interface a virtual destructor.

### Can an interface / abstract class have a constructor?
Yes, in C++, an interface / abstract class can have a constructor. It is not used to create an object of that class, rather it is used to initialise the data members present in that class which are not part of the child class whose object is constructed.

So, we cannot call the constructor of an abstract class like this for the later example: 

`Notification n("Amazon");`
``` cpp
class Notification {
protected:
    string sender;

public:
    Notification(string s) {
        sender = s;
    }

    virtual void send() = 0;
};
```
But this is completely valid:
``` cpp
class EmailNotification : public Notification {
public:
    EmailNotification(string s)
        : Notification(s) {
    }

    void send() override {
        cout << "Sending email from " << sender;
    }
};
```
This is allowed because abstract class may contain things that its child class may need

### Can an abstract class / interface have destructor
It is possible and is usually recommended for these class to have a virtual destructor so that the child class objects can be deleted by calling the parent class destructor


### Abstraction and Dependency Injection

Suppose OrderService needs a notification service. Instead of creating a specific notification implementation inside OrderService, we can make it depend on the abstraction:

class OrderService {
    Notification* notificationService;

public:
    OrderService(Notification* service)
        : notificationService(service) {}
};

OrderService does not need to know whether the notification service sends emails, SMS messages, or push notifications. The dependency is provided from outside. This reduces coupling and makes the system easier to extend and test.