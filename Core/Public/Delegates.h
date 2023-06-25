/**
 * @file DelegatesHelper.h
 * @brief A header file that defines class templates for storing and invoking methods and functions with different signatures
 * @author Leandro Peres
 * @date June 2023s
 * @version 0.1
 * @copyright MIT License
 */

#pragma once

#include <functional>
#include <vector>

/**
 * @brief A class template that can be used to store and invoke a method of an object
 * @tparam T The type of the object that contains the method
 * @tparam Args The types of the arguments that the method should accept
 *
 * Examples:
 *
 * class MyClass {
 * public:
 *   void myMethod(int x) {
 *     std::cout << "myMethod called with argument " << x << std::endl;
 *   }
 * };
 *
 * MyClass myObject;
 * MethodPointer<MyClass, int> myMethodPointer(&myObject, &MyClass::myMethod);
 * myMethodPointer(42); // Prints "myMethod called with argument 42"
 */
template <typename T, typename... Args>
class MethodPointer
{
public:
    // Type aliases for the method types
    using MethodType = void (T::*)(Args...);
    using ConstMethodType = void (T::*)(Args...) const;

    /** 
     * @brief Constructs a MethodPointer object for non-const methods
     * @param obj The object that contains the method
     * @param method The method to store
     */
    MethodPointer(T* obj, MethodType method) : Obj(obj), Method(method), bIsConst(false) { }

    /** 
     * @brief Constructs a MethodPointer object for const methods
     * @param obj The object that contains the method
     * @param method The method to store
     */
    MethodPointer(const T* obj, ConstMethodType method) : Obj(const_cast<T*>(obj)),
                                                          Method(reinterpret_cast<MethodType>(method)),
                                                          bIsConst(true)
    {
    }

    /**
     * @brief Invokes the stored method
     * @param args The arguments to pass to the method
     */
    void Invoke(Args... args) const
    {
        if (bIsConst)
            (Obj->*reinterpret_cast<ConstMethodType>(Method))(args...);
        else
            (Obj->*Method)(args...);
    }

    /** 
     * @brief Invokes the stored method
     * @param args The arguments to pass to the method
     */
    void operator()(Args... args) const { Invoke(args...); }

    /** 
     * @brief Compares two MethodPointer objects
     * @param other The other MethodPointer object to compare to
     * @return true if the two objects are equal, false otherwise
     */
    bool operator==(const MethodPointer& other) const
    {
        return Obj == other.Obj && Method == other.Method && bIsConst == other.bIsConst;
    }

private:
    /** The object that contains the method */
    T* Obj;

    /**
     * The method to store.
     * These two members share the same memory location, meaning that only one of them can be used at a time.
     */
    union
    {
        MethodType Method;
        ConstMethodType ConstMethod;
    };

    /** True if the stored method is const, false otherwise */
    bool bIsConst;
};

/*
 * This class template can be used to create delegate objects that can store and invoke
 * functions with different signatures. The template takes a variable number of arguments
 * representing the types of the arguments that the stored functions should accept.
 *
 * Examples:
 *
 * Delegate<> d; // or DelegateNoArgs d;
 * d += []() { std::cout << "Live long and prosper" << std::endl; };
 * d(); // prints "Live long and prosper"
 *
 * Delegate<int, float> d;
 * d += [](int i, float f) { std::cout << i << ", " << f << std::endl; };
 * d(42, 6.28f); // prints "42, 6.28"
 *
 * class Foo { void Foo(int i, float f) { std::cout << i << ", " << f << std::endl; } }
 * Foo foo;
 * Delegate<int, float> d;
 * d += MethodPointer<Foo, int, float>(&foo, &Foo::Foo);
 * d -= MethodPointer<Foo, int, float>(&foo, &Foo::Foo);
 */
template <typename... Args>
class Delegate
{
public:
    // Type alias for a std::function with the same signature as the stored functions
    using FuncType = std::function<void(Args...)>;

    /** Clears the Actions vector */
    void Clear() { Actions.clear(); }

    /** 
     * @brief Adds a function to the Actions vector
     * @param func The function to add
     */
    void operator+=(FuncType func)
    {
        Actions.push_back(func);
    }

    /** Removes a function from the Actions vector */
    void operator-=(FuncType func)
    {
        // Used to match the function to remove with the function in the vector
        const auto matchTarget = [&](const FuncType& f)
        {
            return f.template target<void(*)(Args...)>() == func.template target<void(*)(Args...)>();
        };

        Actions.erase(std::remove_if(Actions.begin(), Actions.end(), matchTarget), Actions.end());
    }

    /** 
     * @brief Invokes all the functions in the Actions vector
     * @param args The arguments to pass to the functions
     */
    void Invoke(Args... args) const
    {
        for (const auto& f : Actions)
        {
            if (!f) continue;
            f(args...);
        }
    }

    /** 
     * @brief Invokes all the functions in the Actions vector
     * @param args The arguments to pass to the functions
     */
    void operator()(Args... args) const { Invoke(args...); }

private:
    /** The stored functions */
    std::vector<FuncType> Actions;
};

// Delegate with no arguments is a special case
using DelegateNoArgs = Delegate<>;
