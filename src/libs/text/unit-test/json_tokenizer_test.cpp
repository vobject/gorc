#include "test/test.hpp"
#include "text/json_tokenizer.hpp"
#include "io/memory_file.hpp"

using namespace gorc;

class json_tokenizer_test_fixture : public test::fixture {
public:
    memory_file mf;

    void set_file(std::string const &s)
    {
        mf.write(s.data(), s.size());
    }
};

#define tok_assert(t, v) \
    expect_eq(tok.get_type(), token_type::t); \
    expect_eq(tok.get_value(), v); \
    tok.advance()

#define tok_error(v, r) \
    expect_eq(tok.get_type(), token_type::error); \
    expect_eq(tok.get_value(), v); \
    expect_eq(tok.get_reason(), r);

#define tok_eof() \
    expect_eq(tok.get_type(), token_type::end_of_file)

begin_suite_fixture(json_tokenizer_test, json_tokenizer_test_fixture);

test_case(empty_file)
{
    json_tokenizer tok(mf);
    tok_eof();
}

test_case(whitespace_file)
{
    set_file("    \t\t\n"
             "\n"
             "    ");
    json_tokenizer tok(mf);
    tok_eof();
}

test_case(identifier_cannot_start_with_digit)
{
    set_file("958_ac_3");
    json_tokenizer tok(mf);

    tok_assert(integer, "958");
    tok_assert(identifier, "_ac_3");
    tok_eof();
}

test_case(identifiers)
{
    set_file("abcABC123 _abc");
    json_tokenizer tok(mf);

    tok_assert(identifier, "abcABC123");
    tok_assert(identifier, "_abc");
    tok_eof();
}

test_case(string_escape_sequences)
{
    set_file("\"asdf \\\\ \\\" \\t \\n\"\n"
             "\"string containing \\0 null\"");
    json_tokenizer tok(mf);

    tok_assert(string, "asdf \\ \" \t \n");

    std::string other = "string containing ";
    other.push_back('\0');
    other += " null";

    tok_assert(string, other);
    tok_eof();
}

test_case(eof_in_string_literal)
{
    set_file("\"asdf");
    json_tokenizer tok(mf);

    tok_error("asdf", "unexpected eof in string literal");
}

test_case(eof_in_string_escape_sequence)
{
    set_file("\"asdf \\");
    json_tokenizer tok(mf);

    tok_error("asdf ", "unexpected eof in string literal escape sequence");
}

test_case(unknown_string_escape_sequence)
{
    set_file("\"asdf \\z\"");
    json_tokenizer tok(mf);

    tok_error("asdf ", "unknown escape sequence \\z");
}

test_case(hex_integers)
{
    set_file("0x01 0X01 0x0a 0x0A 0X0a 0X0A");
    json_tokenizer tok(mf);

    tok_assert(hex_integer, "0x01");
    tok_assert(hex_integer, "0X01");
    tok_assert(hex_integer, "0x0a");
    tok_assert(hex_integer, "0x0A");
    tok_assert(hex_integer, "0X0a");
    tok_assert(hex_integer, "0X0A");
    tok_eof();
}

test_case(hex_integer_errors)
{
    set_file("0x0a 0x 0x");
    json_tokenizer tok(mf);

    tok_assert(hex_integer, "0x0a");
    tok_error("0x", "expected hex integer");
}

test_case(hex_integer_out_of_range)
{
    set_file("0x0afg");
    json_tokenizer tok(mf);

    tok_error("0x0af", "digit out of range");
}

test_case(line_comments)
{
    set_file("# asdf \n"
             "foo#asdf\n"
             "foo");
    json_tokenizer tok(mf);

    tok_assert(identifier, "foo");
    tok_assert(identifier, "foo");
    tok_eof();
}

test_case(punctuators)
{
    set_file("+");
    json_tokenizer tok(mf);

    tok_assert(punctuator, "+");
    tok_eof();
}

test_case(control_code_not_recognized)
{
    std::string str;
    str.push_back(0x0E);
    set_file(str);
    json_tokenizer tok(mf);

    tok_error("\x0E", "unknown input value");
}

test_case(integers)
{
    set_file("0 12345");
    json_tokenizer tok(mf);

    tok_assert(integer, "0");
    tok_assert(integer, "12345");
    tok_eof();
}

test_case(integers_out_of_range)
{
    set_file("182ABCD");
    json_tokenizer tok(mf);

    tok_error("182", "digit out of range");
}

test_case(integer_signs)
{
    set_file("+659 -38");
    json_tokenizer tok(mf);

    // Signs are sent as separate tokens
    tok_assert(punctuator, "+");
    tok_assert(integer, "659");
    tok_assert(punctuator, "-");
    tok_assert(integer, "38");
    tok_eof();
}

test_case(hex_integer_signs)
{
    set_file("+0xfaf -0XABA");
    json_tokenizer tok(mf);

    // Signs are sent as separate tokens
    tok_assert(punctuator, "+");
    tok_assert(hex_integer, "0xfaf");
    tok_assert(punctuator, "-");
    tok_assert(hex_integer, "0XABA");
}

test_case(oct_integer_signs)
{
    set_file("-0777 +0173");
    json_tokenizer tok(mf);

    tok_assert(punctuator, "-");
    tok_assert(oct_integer, "0777");
    tok_assert(punctuator, "+");
    tok_assert(oct_integer, "0173");
}

test_case(octal)
{
    set_file("01234567");
    json_tokenizer tok(mf);

    tok_assert(oct_integer, "01234567");
    tok_eof();
}

test_case(octal_out_of_range)
{
    set_file("0128");
    json_tokenizer tok(mf);

    tok_error("012", "digit out of range");
}

test_case(octal_first_out_of_range)
{
    set_file("09");
    json_tokenizer tok(mf);

    tok_error("0", "digit out of range");
}

test_case(decimal)
{
    set_file("0.12 1.2 1.2e50 1.2E5 1.2e+5 1.2e-5 1e5");
    json_tokenizer tok(mf);

    tok_assert(real, "0.12");
    tok_assert(real, "1.2");
    tok_assert(real, "1.2e50");
    tok_assert(real, "1.2E5");
    tok_assert(real, "1.2e+5");
    tok_assert(real, "1.2e-5");
    tok_assert(real, "1e5");
    tok_eof();
}

test_case(decimal_out_of_range)
{
    set_file("0.1234X");
    json_tokenizer tok(mf);

    tok_error("0.1234", "digit out of range");
}

test_case(decimal_fractional_part_missing)
{
    set_file("1234.  asdf");
    json_tokenizer tok(mf);

    tok_error("1234.", "expected fractional part");
}

test_case(decimal_exponent_part_missing)
{
    set_file("1234.1234e asdf");
    json_tokenizer tok(mf);

    tok_error("1234.1234e", "expected exponent");
}

test_case(decimal_exponent_out_of_range)
{
    set_file("1234.1234e12FX");
    json_tokenizer tok(mf);

    tok_error("1234.1234e12", "digit out of range");
}

test_case(non_sign_before_number)
{
    set_file("*123 *123.0 *0xf *012");
    json_tokenizer tok(mf);

    tok_assert(punctuator, "*");
    tok_assert(integer, "123");
    tok_assert(punctuator, "*");
    tok_assert(real, "123.0");
    tok_assert(punctuator, "*");
    tok_assert(hex_integer, "0xf");
    tok_assert(punctuator, "*");
    tok_assert(oct_integer, "012");
    tok_eof();
}

test_case(decimal_leading_is_not_number)
{
    set_file(".234 .234e+53");
    json_tokenizer tok(mf);

    tok_assert(punctuator, ".");
    tok_assert(integer, "234");
    tok_assert(punctuator, ".");
    tok_assert(real, "234e+53");
    tok_eof();
}

test_case(period_leading_is_still_punct)
{
    set_file(".X");
    json_tokenizer tok(mf);

    tok_assert(punctuator, ".");
    tok_assert(identifier, "X");
    tok_eof();
}

end_suite(json_tokenizer_test);
