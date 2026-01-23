#pragma once
#include <cstdint>
#include <exception>
#include <type_traits>
#include <utility>

namespace bmstu
{
struct nullopt_t
{
	constexpr explicit nullopt_t(int) {}
};
inline constexpr nullopt_t nullopt{0};

class bad_optional_access : public std::exception
{
   public:
	using exception::exception;

	const char* what() const noexcept override { return "Bad optional access"; }
};

template <typename T>
class optional
{
   public:
	optional() = default;

	optional(const T& value)
	{
		is_initialized_ = true;
		new (&data_[0]) T(value);
	}

	optional(T&& value)
	{
		is_initialized_ = true;
		new (&data_[0]) T(std::move(value));
	}

	optional(const optional& other) : is_initialized_(other.is_initialized_)
	{
		if (other.is_initialized_)
		{
			new (&data_[0]) T(other.value());
		}
	}

	optional(optional&& other) noexcept : is_initialized_(other.is_initialized_)
	{
		if (is_initialized_)
		{
			new (&data_[0]) T(std::move(other.value()));
		}
	}

	optional& operator=(const T& value)
	{
		if (is_initialized_)
		{
			this->value() = value;
		}
		else
		{
			new (&data_[0]) T(value);
			is_initialized_ = true;
		}
		return *this;
	}

	optional& operator=(T&& value)
	{
		if (is_initialized_)
		{
			this->value() = std::move(value);
		}
		else
		{
			new (&data_[0]) T(std::move(value));
			is_initialized_ = true;
		}
		return *this;
	}

	optional& operator=(const optional& other)
	{
		if (this != &other)
		{
			if (other.is_initialized_)
			{
				if (is_initialized_)
				{
					value() = other.value();
				}
				else
				{
					new (&data_[0]) T(other.value());
					is_initialized_ = true;
				}
			}
			else
			{
				reset();
			}
		}
		return *this;
	}

	optional& operator=(optional&& other) noexcept
	{
		if (this != &other)
		{
			if (other.is_initialized_)
			{
				if (is_initialized_)
				{
					value() = std::move(other.value());
				}
				else
				{
					new (&data_[0]) T(std::move(other.value()));
					is_initialized_ = true;
				}
			}
		}
		return *this;
	}

	T& operator*() &
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return *reinterpret_cast<T*>(&data_[0]);
	}

	const T& operator*() const&
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return *reinterpret_cast<const T*>(&data_[0]);
	}

	T* operator->()
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return reinterpret_cast<T*>(&data_[0]);
	}

	const T* operator->() const noexcept
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return reinterpret_cast<const T*>(&data_[0]);
	}

	T&& operator*() &&
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return std::move(*reinterpret_cast<T*>(&data_[0]));
	}

	T& value() &
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return *reinterpret_cast<T*>(&data_[0]);
	}

	const T& value() const&
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return *reinterpret_cast<const T*>(&data_[0]);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		if (is_initialized_)
		{
			reinterpret_cast<T*>(&data_[0])->~T();
		}
		new (&data_[0]) T(std::forward<Args>(args)...);
		is_initialized_ = true;
	}

	void reset()
	{
		if (is_initialized_)
		{
			reinterpret_cast<T*>(&data_[0])->~T();
			is_initialized_ = false;
		}
	}

	~optional() noexcept { reset(); }

	bool has_value() const { return is_initialized_; }

   private:
	alignas(T) uint8_t data_[sizeof(T)];
	bool is_initialized_ = false;
};
}  // namespace bmstu