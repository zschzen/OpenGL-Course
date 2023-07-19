#pragma once

#include "Delegates.h"

template <typename T>
class Observable;

template <typename T>
class ObservableValue
{
public:
    ObservableValue(Observable<T>& observable) : observable(observable) {}

    void operator=(const T& newValue)
    {
        observable.SetValue(newValue);
    }

    operator T()
    {
        return observable.Get();
    }

private:
    Observable<T>& observable;
};

template <typename T>
class ObservableConverter
{
public:
    ObservableConverter(Observable<T>& observable) : observable(observable) {}

    operator T()
    {
        return observable.Get();
    }

private:
    Observable<T>& observable;
};

template <typename T>
class Observable
{
public:
    using ValueType = T;
    using EventDelegate = Delegate<const T&>;

    /** Constructor with an initial value */
    Observable(const T& initialValue) : value(initialValue) {}

    /** Set the value and notify subscribers if it has changed */
    void SetValue(const T& newValue)
    {
        if (value != newValue)
        {
            value = newValue;
            NotifySubscribers();
        }
    }

    /** Get the current value */
    T& Get() { return value; }

    /** Subscribe to the value change event */
    void Subscribe(typename EventDelegate::FuncType subscriber)
    {
        valueChangedEvent += subscriber;
    }

    /** Unsubscribe from the value change event */
    void Unsubscribe(typename EventDelegate::FuncType subscriber)
    {
        valueChangedEvent -= subscriber;
    }

    /** Get a helper class for the assignment operator */
    ObservableValue<T> operator=(const T& newValue)
    {
        SetValue(newValue);
        return ObservableValue<T>(*this);
    }

    /** Get value operator */
    operator T()
    {
        return value;
    }

    /** Get a helper class for the conversion operator */
    operator ObservableConverter<T>() const
    {
        return ObservableConverter<T>(*this);
    }

private:
    T value;
    EventDelegate valueChangedEvent;

    /** Notify all subscribers about the value change */
    void NotifySubscribers() const
    {
        valueChangedEvent(value);
    }
};
