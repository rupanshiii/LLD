### 1. What is the difference between struct and class in C++?

In C++, struct and class are almost identical. The main difference is their default access modifier and default inheritance visibility.

1. Default access modifier

    This is the most important difference.

| Feature | `struct` | `class` |
|---------|----------|---------|
| Default member access | `public` | `private` |
| Default inheritance | `public` | `private` |
| Can have constructors? | Yes | Yes |
| Can have methods? | Yes | Yes |
| Can have inheritance? | Yes | Yes |
| Can have virtual functions? | Yes | Yes |
| Can use encapsulation? | Yes | Yes |

### 2. What is the difference between using MyClass obj; and MyClass* obj = new MyClass() in C++?

#### 1. C++ Object Creation

**Automatic object**
MyClass obj;
* `obj` is an **actual object**, not a reference/pointer.
* It has **automatic storage duration** when declared as a local variable.
* Its lifetime is tied to its scope.
* Typically implemented using the **stack**.

**Dynamic object**
MyClass* obj = new MyClass();
* `new MyClass()` creates the **actual object**.
* The object has **dynamic storage duration**.
* Its lifetime is independent of the scope containing the pointer.
* Typically implemented using the **heap**.
* It must eventually be destroyed/deallocated, traditionally using `delete`.

```text
Stack                    Heap
┌──────────┐             ┌──────────────┐
│ obj      │ ─────────→  │ MyClass      │
│ pointer  │             │ actual object│
└──────────┘             └──────────────┘
```

---

#### 2. Storage Duration

**Storage duration = how long the storage associated with an object exists.**

**Automatic storage duration**

```cpp
void foo() {
    MyClass obj;
}
```

* Storage is automatically managed according to the object's scope.
* `obj` is destroyed when its scope ends.
* Typically associated with the stack.

```text
foo() starts
    ↓
obj exists
    ↓
foo() ends
    ↓
obj destroyed
```

**Dynamic storage duration**

```cpp
MyClass* obj = new MyClass();
```

* Storage is obtained dynamically.
* Lifetime is not tied to the scope of the pointer.
* Object remains alive until it is destroyed/deallocated.
* Typically associated with the heap.

**Static storage duration**

```cpp
static MyClass obj;
```

or a global object:

```cpp
MyClass obj;
```

* Storage exists for essentially the entire execution of the program.
* Typically associated with static/data storage.

---

#### 3. Stack vs Heap Is a Simplification

The C++ standard talks about **storage duration**, not literally "stack" and "heap."

For learning, this model is useful:

```text
Automatic → typically Stack
Dynamic   → typically Heap
Static    → typically Data/Static storage
```

The compiler is free to implement these concepts differently as long as the required C++ behavior is preserved.

So:

> **Storage duration is the language concept; stack/heap is the typical implementation model.**

---

#### 4. Returning an Object by Value

This is completely valid:

```cpp
MyClass createObject() {
    MyClass obj;
    return obj;
}

int main() {
    MyClass x = createObject();
}
```

Important:

* Returning `obj` does **not** make it a dynamic object.
* `x` is an object in the caller.
* The object is returned **by value**.
* An actual copy may happen in some situations, but modern C++ can use **copy elision**, meaning the result can be constructed directly in `x`.
* obj lifecycle ends in createObject() method itself

Conceptually:

```text
createObject()
      ↓
return by value
      ↓
caller receives an object
      ↓
MyClass x
```

So:

> **Returning an object ≠ dynamically allocating an object.**

---

#### 5. Returning a Dynamically Allocated Object

```cpp
MyClass* createObject() {
    MyClass* obj = new MyClass();
    return obj;
}

int main() {
    MyClass* x = createObject();
}
```

Here:

* `obj` is a pointer variable.
* The actual `MyClass` object was created using `new`.
* `x` receives a pointer to that **same object**.
* The object continues to exist after `createObject()` returns.
* Traditionally, the caller eventually uses `delete x`.

```text
createObject()

obj (pointer)
     │
     ↓
[ MyClass object ]
     ↑
     │
x (pointer)
```

There is no copying of the `MyClass` object merely because the pointer is returned.

---

#### 6. Return by Value vs Return Pointer

|                              | Return by value                                       | Return pointer                             |
| ---------------------------- | ----------------------------------------------------- | ------------------------------------------ |
| Example                      | `MyClass createObject()`                              | `MyClass* createObject()`                  |
| Caller receives              | Object                                                | Pointer                                    |
| Same object?                 | Caller gets the returned object/value                 | Points to same dynamic object              |
| Dynamic allocation required? | No                                                    | Yes, if using `new`                        |
| Copy necessarily happens?    | No, copy elision may occur                            | No object copy just from returning pointer |
| Lifetime                     | Determined by the resulting object's storage duration | Dynamic object can outlive the function    |
| Typical syntax               | `MyClass x = createObject();`                         | `MyClass* x = createObject();`             |

---

#### 7. Never Return a Reference to a Local Automatic Object

This is dangerous:

```cpp
MyClass& createObject() {
    MyClass obj;
    return obj;       // ❌
}
```

Why?

```text
createObject()
     ↓
obj exists
     ↓
function ends
     ↓
obj is destroyed
     ↓
returned reference refers to destroyed object
```

The reference becomes a **dangling reference**.

---

### 3. Do we really need destructors?
A destructor runs when an object is being destroyed. 
- If the object is created normally without `new`, its destructor runs automatically when the object's lifetime ends. For example, if we create `Email email;` inside a function, the object is automatically destroyed when the function ends, and its destructor runs at that point. 
- If we create the object using `new`, such as `Email* email = new Email();`, we normally need to use `delete email;` to destroy the object and release its dynamically allocated memory. When `delete` is used, the destructor runs as part of the destruction process before the memory is released.

However, we do not need to explicitly write a destructor in every class. If we do not have any special cleanup operation to perform, C++ automatically provides a destructor for us. For example, if an `Email` class does not own any special resource that needs to be cleaned up, we can simply write the class without defining a destructor. The compiler-generated destructor is enough.

There is one important exception when inheritance and polymorphism are involved. If a class is intended to be used as a polymorphic base class, we should generally give it a virtual destructor. For example, if we have `NotificationChannel* channel = new Email();` and later do `delete channel;`, the pointer is of type `NotificationChannel*`, but the actual object is an `Email`. A virtual destructor ensures that C++ correctly destroys the `Email` object and then its base-class part. If the destructor is non virtual, the pointer variable cannot destroy the object of the child class.

We do not necessarily need to write destructors in both the parent and child classes. If the child does not need any special cleanup, the compiler automatically provides its destructor. The important part is that the polymorphic base class has a virtual destructor.

So there are three separate ideas to remember. 
- First, a destructor is responsible for cleanup that should happen when an object is destroyed, but we do not need to write one if the class does not require special cleanup. 
- Second, if a class is used polymorphically as a base class, its destructor should generally be virtual, even if the destructor itself does nothing special. 
- Third, having a destructor does not mean that we manually need to delete the object. `delete` is used to destroy an object that was dynamically allocated with `new`, while the destructor defines what happens during that destruction. In modern C++, we generally prefer smart pointers such as `unique_ptr` so that we do not have to manually call `delete`.

Suppose we have `NotificationChannel* channel = new Email();`. Here, `NotificationChannel*` is the type of the pointer, while `new Email()` creates the actual object, which is an `Email` object. 
The child class already gets its own destructor automatically from C++, so the reason we make the destructor in the base class explicity is  to make destruction work correctly when we delete the object through a base-class pointer. If the base destructor is not virtual, then `delete channel` does not perform proper polymorphic destruction, and the behavior is undefined. When the base destructor is virtual, C++ can look at the actual object being pointed to, see that it is an `Email`, and start destruction with the `Email` destructor, followed by the `NotificationChannel` destructor. 
Therefore, the important distinction is that `NotificationChannel*` tells us the type through which we are accessing the object, while `new Email()` tells us the actual type of the object. A virtual destructor allows C++ to use that actual object type during destruction. This is why, whenever a class is intended to be used as a polymorphic base class, we generally give it a virtual destructor, even though the child classes already have their own destructors automatically.

`= default` is used to tell C++ to use the same default destructor with some additional functionalities like making it virtual, this could be used with constructors as well
