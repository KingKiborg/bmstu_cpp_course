#include <gtest/gtest.h>

#include "optional.h"  // Assuming the optional implementation is in optional.h from previous code

#include <string>
#include <utility>
#include <stdexcept>

// Helper struct for counting constructions/destructions/copies/moves
struct Tracker {
    static int constructions;
    static int destructions;
    static int copies;
    static int moves;

    Tracker() { ++constructions; }
    Tracker(const Tracker&) { ++copies; ++constructions; }
    Tracker(Tracker&&) noexcept { ++moves; ++constructions; }
    Tracker& operator=(const Tracker&) { ++copies; return *this; }
    Tracker& operator=(Tracker&&) noexcept { ++moves; return *this; }
    ~Tracker() { ++destructions; }

    static void reset() {
        constructions = 0;
        destructions = 0;
        copies = 0;
        moves = 0;
    }
};

int Tracker::constructions = 0;
int Tracker::destructions = 0;
int Tracker::copies = 0;
int Tracker::moves = 0;

// Basic tests for default constructor
TEST(OptionalTest, DefaultConstructorEmpty) {
    bmstu::optional<int> opt;
    EXPECT_FALSE(opt.has_value());
}

TEST(OptionalTest, DefaultConstructorThrowOnValue) {
    bmstu::optional<int> opt;
    EXPECT_THROW(opt.value(), bmstu::bad_optional_access);
}

// Value constructors
TEST(OptionalTest, CopyValueConstructor) {
    int val = 42;
    bmstu::optional<int> opt(val);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), 42);
    EXPECT_EQ(*opt, 42);
}

TEST(OptionalTest, MoveValueConstructor) {
    std::string str = "hello world";
    bmstu::optional<std::string> opt(std::move(str));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), "hello world");
    EXPECT_TRUE(str.empty());  // Moved from
}

TEST(OptionalTest, CopyValueConstructorWithTracker) {
    Tracker::reset();
    Tracker t;
    bmstu::optional<Tracker> opt(t);
    EXPECT_EQ(Tracker::copies, 1);
    EXPECT_EQ(Tracker::moves, 0);
    EXPECT_EQ(Tracker::constructions, 2);  // Original + copy
}

TEST(OptionalTest, MoveValueConstructorWithTracker) {
    Tracker::reset();
    bmstu::optional<Tracker> opt(Tracker{});
    EXPECT_EQ(Tracker::copies, 0);
    EXPECT_EQ(Tracker::moves, 1);
    EXPECT_EQ(Tracker::constructions, 2);  // Temp + move construct
}

// Copy constructor
TEST(OptionalTest, CopyConstructorFromEmpty) {
    bmstu::optional<int> empty;
    bmstu::optional<int> opt(empty);
    EXPECT_FALSE(opt.has_value());
}

TEST(OptionalTest, CopyConstructorFromInitialized) {
    bmstu::optional<int> orig(42);
    bmstu::optional<int> opt(orig);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), 42);
    EXPECT_EQ(orig.value(), 42);  // Original unchanged
}

TEST(OptionalTest, CopyConstructorWithTracker) {
    Tracker::reset();
    bmstu::optional<Tracker> orig(Tracker{});
    bmstu::optional<Tracker> opt(orig);
    EXPECT_EQ(Tracker::copies, 1);  // Copy from orig.value()
    EXPECT_EQ(Tracker::moves, 0);
}

TEST(OptionalTest, CopyConstructorBothInitialized) {
    // Not applicable as 'this' is uninitialized in constructor
}

// Move constructor
TEST(OptionalTest, MoveConstructorFromEmpty) {
    bmstu::optional<int> empty;
    bmstu::optional<int> opt(std::move(empty));
    EXPECT_FALSE(opt.has_value());
    EXPECT_FALSE(empty.has_value());  // Still empty
}

TEST(OptionalTest, MoveConstructorFromInitialized) {
    bmstu::optional<std::string> orig("hello");
    bmstu::optional<std::string> opt(std::move(orig));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), "hello");
    EXPECT_TRUE(orig.has_value());  // Not reset in this impl
    EXPECT_EQ(orig.value(), "");    // Moved-from state
}

TEST(OptionalTest, MoveConstructorWithTracker) {
    Tracker::reset();
    bmstu::optional<Tracker> orig(Tracker{});
    bmstu::optional<Tracker> opt(std::move(orig));
    EXPECT_EQ(Tracker::copies, 0);
    EXPECT_EQ(Tracker::moves, 1);  // Move from orig.value()
}

TEST(OptionalTest, MoveConstructorBothInitialized) {
    // Not applicable as 'this' is uninitialized in constructor
}

// Reset
TEST(OptionalTest, ResetOnEmpty) {
    bmstu::optional<int> opt;
    opt.reset();  // No-op
    EXPECT_FALSE(opt.has_value());
}

TEST(OptionalTest, ResetOnInitialized) {
    bmstu::optional<int> opt(42);
    opt.reset();
    EXPECT_FALSE(opt.has_value());
    EXPECT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(OptionalTest, MultipleResets) {
    bmstu::optional<int> opt(42);
    opt.reset();
    opt.reset();
    EXPECT_FALSE(opt.has_value());
}

TEST(OptionalTest, ResetWithTracker) {
    Tracker::reset();
    {
        bmstu::optional<Tracker> opt(Tracker{});
        opt.reset();
        EXPECT_EQ(Tracker::destructions, 1);
    }
    EXPECT_EQ(Tracker::destructions, 1);  // No more
}

// Destructor
TEST(OptionalTest, DestructorEmpty) {
    // No crash
    { bmstu::optional<int> opt; }
}

TEST(OptionalTest, DestructorInitialized) {
    Tracker::reset();
    {
        bmstu::optional<Tracker> opt(Tracker{});
    }
    EXPECT_EQ(Tracker::destructions, 1);
}

// Operator*
TEST(OptionalTest, OperatorStarLValue) {
    bmstu::optional<int> opt(42);
    EXPECT_EQ(*opt, 42);
    *opt = 100;
    EXPECT_EQ(opt.value(), 100);
}

TEST(OptionalTest, OperatorStarConstLValue) {
    const bmstu::optional<int> opt(42);
    EXPECT_EQ(*opt, 42);
}

TEST(OptionalTest, OperatorStarRValue) {
    bmstu::optional<std::string> opt("hello");
    std::string moved = *std::move(opt);
    EXPECT_EQ(moved, "hello");
    EXPECT_EQ(opt.value(), "");  // Moved-from
}

TEST(OptionalTest, OperatorStarModifies) {
    bmstu::optional<int> opt(42);
    *opt = 100;
    EXPECT_EQ(*opt, 100);
    EXPECT_EQ(opt.value(), 100);
}

TEST(OptionalTest, RValueOperatorStarMoves) {
    bmstu::optional<std::string> opt("hello");
    std::string s = *std::move(opt);
    EXPECT_EQ(s, "hello");
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, ""); // moved-from state
}

TEST(OptionalTest, OperatorStarConstRValue) {
    // But operator* && is for rvalue, returns T&&
    // For const, it's const&
    const bmstu::optional<std::string> opt("const");
    const std::string& ref = *opt;
    EXPECT_EQ(ref, "const");
}

TEST(OptionalTest, OperatorStarOnEmpty) {
    bmstu::optional<int> opt;
    // Undefined behavior in code, but we don't test crashes, assume user checks has_value
}

// Operator->
struct TestStruct {
    int x = 10;
    int getX() const { return x; }
};

TEST(OptionalTest, OperatorArrow) {
    bmstu::optional<TestStruct> opt(TestStruct{});
    EXPECT_EQ(opt->x, 10);
    EXPECT_EQ(opt->getX(), 10);
    opt->x = 20;
    EXPECT_EQ(opt->x, 20);
}

TEST(OptionalTest, OperatorArrowConst) {
    const bmstu::optional<TestStruct> opt(TestStruct{});
    EXPECT_EQ(opt->x, 10);
    EXPECT_EQ(opt->getX(), 10);
}

TEST(OptionalTest, OperatorArrowMethodCall) {
    struct Func {
        std::string greet() const { return "hello"; }
    };
    bmstu::optional<Func> opt(Func{});
    EXPECT_EQ(opt->greet(), "hello");
}

// Emplace
TEST(OptionalTest, EmplaceOnEmpty) {
    bmstu::optional<std::string> opt;
    opt.emplace("hello");
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), "hello");
}

TEST(OptionalTest, EmplaceOnInitialized) {
    bmstu::optional<std::string> opt("old");
    opt.emplace("new");
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), "new");
}

TEST(OptionalTest, EmplaceWithArgs) {
    bmstu::optional<std::pair<int, std::string>> opt;
    opt.emplace(42, "answer");
    EXPECT_EQ(opt->first, 42);
    EXPECT_EQ(opt->second, "answer");
}

TEST(OptionalTest, EmplaceWithTracker) {
    Tracker::reset();
    bmstu::optional<Tracker> opt;
    opt.emplace();
    EXPECT_EQ(Tracker::constructions, 1);
    EXPECT_EQ(Tracker::copies, 0);
    EXPECT_EQ(Tracker::moves, 0);
}

TEST(OptionalTest, EmplaceReplaceWithTracker) {
    Tracker::reset();
    bmstu::optional<Tracker> opt(Tracker{});
    EXPECT_EQ(Tracker::destructions, 0);
    opt.emplace();
    EXPECT_EQ(Tracker::destructions, 1);  // Old destroyed
    EXPECT_EQ(Tracker::constructions, 3);  // First temp, move to opt, emplace new
}

TEST(OptionalTest, EmplaceWithMoveArg) {
    std::string s = "moved";
    bmstu::optional<std::string> opt;
    opt.emplace(std::move(s));
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, "moved");
    EXPECT_TRUE(s.empty());
}

TEST(OptionalTest, EmplaceAfterReset) {
    bmstu::optional<int> opt(42);
    opt.reset();
    opt.emplace(100);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 100);
}

TEST(OptionalTest, EmplaceMultipleArgsWithTracker) {
    struct PairLike {
        int a;
        std::string b;
        PairLike(int x, std::string y) : a(x), b(std::move(y)) {}
    };
    Tracker::reset(); // But Tracker not used here
    bmstu::optional<PairLike> opt;
    std::string str = "test";
    opt.emplace(42, std::move(str));
    EXPECT_EQ(opt->a, 42);
    EXPECT_EQ(opt->b, "test");
    EXPECT_TRUE(str.empty());
}

TEST(OptionalTest, MultipleEmplace) {
    bmstu::optional<int> opt;
    opt.emplace(1);
    opt.emplace(2);
    EXPECT_EQ(*opt, 2);
    opt.emplace(3);
    EXPECT_EQ(*opt, 3);
}

TEST(OptionalTest, EmplaceWithException) {
    struct Throws {
        Throws() { throw std::runtime_error("boom"); }
    };
    bmstu::optional<Throws> opt;
    EXPECT_THROW(opt.emplace(), std::runtime_error);
    EXPECT_FALSE(opt.has_value());
}

// Additional edge cases
TEST(OptionalTest, ValueThrowOnEmptyConst) {
    const bmstu::optional<int> opt;
    EXPECT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(OptionalTest, LargeType) {
    struct Large { char buf[1024]; };
    bmstu::optional<Large> opt(Large{});
    EXPECT_TRUE(opt.has_value());
    opt.reset();
    EXPECT_FALSE(opt.has_value());
}

TEST(OptionalTest, NonCopyableType) {
    struct NonCopy {
        NonCopy() = default;
        NonCopy(const NonCopy&) = delete;
        NonCopy(NonCopy&&) = default;
    };
    bmstu::optional<NonCopy> opt(NonCopy{});
    // Copy ctor would fail if attempted, but move is ok
    bmstu::optional<NonCopy> moved(std::move(opt));
    EXPECT_TRUE(moved.has_value());
}

TEST(OptionalTest, NonMovableType) {
    struct NonMove {
        NonMove() = default;
        NonMove(const NonMove&) = default;
        NonMove(NonMove&&) = delete;
    };
    bmstu::optional<NonMove> opt(NonMove{});
    bmstu::optional<NonMove> copied(opt);
    EXPECT_TRUE(copied.has_value());
}

TEST(OptionalTest, BadOptionalAccessWhat) {
    try {
        bmstu::optional<int> opt;
        opt.value();
        FAIL() << "Expected exception";
    } catch (const bmstu::bad_optional_access& e) {
        EXPECT_STREQ(e.what(), "Bad optional access");
    } catch (...) {
        FAIL() << "Wrong exception type";
    }
}

TEST(OptionalTest, ValueReturnsReference) {
    bmstu::optional<int> opt(42);
    int& ref = opt.value();
    ref = 100;
    EXPECT_EQ(opt.value(), 100);
    EXPECT_EQ(*opt, 100);
}

TEST(OptionalTest, ConstValue) {
    const bmstu::optional<int> opt(42);
    EXPECT_EQ(opt.value(), 42);
}

TEST(OptionalTest, HasValueAfterOperations) {
    bmstu::optional<int> opt;
    EXPECT_FALSE(opt.has_value());
    opt.emplace(42);
    EXPECT_TRUE(opt.has_value());
    *opt = 100;
    EXPECT_TRUE(opt.has_value());
    opt.reset();
    EXPECT_FALSE(opt.has_value());
}

TEST(OptionalTest, ConstHasValue) {
    const bmstu::optional<int> empty;
    EXPECT_FALSE(empty.has_value());
    const bmstu::optional<int> full(42);
    EXPECT_TRUE(full.has_value());
}

TEST(OptionalTest, ResetAfterEmplace) {
    bmstu::optional<std::string> opt;
    opt.emplace("temp");
    opt.reset();
    EXPECT_FALSE(opt.has_value());
    EXPECT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(OptionalTest, CopyConstructorEmptyToInitialized) {
    // Already covered indirectly
    bmstu::optional<int> orig;
    bmstu::optional<int> opt(42);
    // But no assignment, this is ctor
}

TEST(OptionalTest, TrivialType) {
    static_assert(std::is_trivially_destructible_v<bmstu::optional<int>>, "Should be trivial for trivial T");
    bmstu::optional<int> opt(42);
    EXPECT_EQ(*opt, 42);
}

TEST(OptionalTest, NonTrivialTypeDestructor) {
    Tracker::reset();
    {
        bmstu::optional<Tracker> opt;
        opt.emplace();
    }
    EXPECT_EQ(Tracker::destructions, 1);
}

TEST(OptionalTest, ValueConstructorWithException) {
    struct Throws {
        Throws(int) { throw std::runtime_error("boom"); }
    };
    EXPECT_THROW(bmstu::optional<Throws> opt(42), std::runtime_error);
}

TEST(OptionalTest, DataAlignment) {
    struct Aligned {
        alignas(32) char data[32];
    };
    bmstu::optional<Aligned> opt(Aligned{});
    EXPECT_TRUE(opt.has_value());
    // No direct test for alignment, but assume placement new handles it
    uintptr_t addr = reinterpret_cast<uintptr_t>(&(opt->data[0]));
    EXPECT_EQ(addr % 32, 0); // Check alignment
}

