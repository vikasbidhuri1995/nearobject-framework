
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <notstd/flextype_wrapper.hxx>
#include <notstd/utility.hxx>

namespace notstd::test
{
template <
    typename ElementT,
    flex_array_type ElementTArrayIndex = flex_array_type::Anysize
>
struct test_flex_type
{
    using flex_element_t = typename ElementT;
    static constexpr flex_array_type flex_element_type_array_index = ElementTArrayIndex;

    uint32_t value;
    std::size_t num_elements;
    flex_element_t elements[notstd::to_underlying(flex_element_type_array_index)];

    template <std::size_t NumElements>
    static inline constexpr std::size_t total_size()
    {
        return offsetof(test_flex_type, elements[NumElements]);
    }
};

using test_flex_type_element_byte = uint8_t;

struct test_flex_type_element_compound
{
    uint32_t Data1;
    uint8_t Data2;
};

/**
 * @brief The MSVC compiler gets really confused when using this, as it attempts
 * to deduce a template argument (and fails) even though the argument was
 * explicitly specified. So, it's commented out for now.
 * 
 * @tparam value_t 
 */
// template <
//     template <
//         typename, flex_array_type
//     > typename value_t
// >
// requires std::is_base_of_v<value_t, test_flex_type>
// struct test_flex_wrapper : 
//     public notstd::flextype_wrapper<value_t, typename value_t::flex_element_t, value_t::flex_array_type>
// {
//     test_flex_wrapper(std::size_t numElements) :
//         flextype_wrapper(numElements)
//     {}   
// };
} // namespace notstd::test

TEST_CASE("flextype_wrapper can be used as value container", "[basic]")
{
    using namespace notstd;
    using namespace notstd::test;

    static constexpr std::size_t NumElements = 20;

    SECTION("value type is correctly reflected with single byte flex-element")
    {
        // TODO
    }

    SECTION("value type is correctly reflected with compound flex-element")
    {
        using flex_type = test_flex_type<test_flex_type_element_compound>;
        using flex_wrapper_type = flextype_wrapper<flex_type, flex_type::flex_element_t>;

        constexpr static auto SizeTotal = flex_type::total_size<NumElements>();

        flex_wrapper_type wrapper{NumElements};

        // Ensure the total size matches.
        REQUIRE(wrapper.Size() == SizeTotal);

        // Populate the value.
        flex_type& value = wrapper;
        value.num_elements = NumElements;
        for (uint8_t i = 0U; i < NumElements; i++) {
            value.elements[i] = { i, i };
        }

        // Verify the value in the buffer reflects the populated value.
        auto& buffer = wrapper.Buffer();
        flex_type& valueFromBuffer = *reinterpret_cast<flex_type*>(std::data(buffer));
        REQUIRE(std::memcmp(&valueFromBuffer, &value, SizeTotal) == 0);
    }
}

TEST_CASE("flextype_wrapper can be used with an existing value", "[basic]")
{
    using namespace notstd::test;

    SECTION("wrapped value matches original value")
    {
    }
}
