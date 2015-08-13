#pragma once

#include <stdexcept>
#include <type_traits>


class EmptyOptionalError : public std::runtime_error {
public:
    EmptyOptionalError(const std::string& message)
        : std::runtime_error(message)
    {}
};


template <typename TValue>
class Optional {
public:
    typedef TValue value_type;

    Optional()
        : hasValue_(false)
    {}

    Optional(const TValue& value)
        noexcept(std::is_nothrow_copy_constructible<TValue>::value)
    {
        constructValue(value);
    }

    Optional(TValue&& value)
        noexcept(std::is_nothrow_move_constructible<TValue>::value)
    {
        constructValue(std::move(value));
    }

    Optional(const Optional& optional)
        noexcept(std::is_nothrow_copy_constructible<TValue>::value)
    {
        if (optional.hasValue()) {
            constructValue(optional.value());
        } else {
            hasValue_ = false;
        }
    }

    Optional(Optional&& optional)
        noexcept(std::is_nothrow_move_constructible<TValue>::value)
    {
        if (optional.hasValue()) {
            constructValue(optional.value());
            optional.clear();
        } else {
            hasValue_ = false;
        }
    }

    template <typename TArg>
    Optional& operator = (TArg&& rhs) {
        assign(std::forward<TArg>(rhs));
        return *this;
    }

    Optional& operator = (const Optional& rhs) {
        if (rhs.hasValue()) {
            assign(rhs.value());
        } else {
            clear();
        }
        return *this;
    }

    Optional& operator = (Optional&& rhs) {
        if (rhs.hasValue()) {
            assign(std::move(rhs.value()));
            rhs.clear();
        } else {
            clear();
        }
        return *this;
    }

    bool hasValue() const {
        return hasValue_;
    }

    operator bool() const {
        return hasValue();
    }

    const TValue& value() const& {
        requireValue();
        return value_;
    }

    TValue& value() & {
        requireValue();
        return value_;
    }

    TValue&& value() && {
        requireValue();
        return std::move(value_);
    }

    void clear() {
        hasValue_ = false;
        value_.~TValue();
    }

private:

    template <typename ... Args>
    void constructValue(Args&&... args) {
        const void* ptr = &value_;
        new (const_cast<void*>(ptr)) TValue(std::forward<Args>(args)...);
        hasValue_ = true;
    }

    void requireValue() const {
        if (!hasValue()) {
            throw EmptyOptionalError("optional is empty, but value is required");
        }
    }

    void assign(const TValue& value) {
        if (hasValue()) {
            value_ = value;
        } else {
            constructValue(value);
        }
    }

    void assign(TValue&& value) {
        if (hasValue()) {
            value_ = std::move(value);
        } else {
            constructValue(std::move(value));
        }
    }

    union { TValue value_; };
    bool hasValue_;
};

