#include <gtest/gtest.h>
#include <StringUtils.h>

using namespace archid;

namespace {

TEST(lcase, ConvertAllTextToLowerCase) {
	EXPECT_STREQ(lcase("This > IS a str12NG").c_str(), "this > is a str12ng");
}

TEST(lcaseconcat, RemoveNonAlphanumAndConvertToLowerCase) {
	EXPECT_STREQ(lcaseconcat("This > IS a str-12NG").c_str(), "thisisastr12ng");
}

// uriEncode tests

TEST(uriEncode, UnreservedCharactersAreNotEncoded) {
	EXPECT_EQ(uriEncode("abcdefghijklmnopqrstuvwxyz"), "abcdefghijklmnopqrstuvwxyz");
	EXPECT_EQ(uriEncode("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	EXPECT_EQ(uriEncode("0123456789"), "0123456789");
	EXPECT_EQ(uriEncode("-_.~"), "-_.~");
}

TEST(uriEncode, SpaceIsEncoded) {
	EXPECT_EQ(uriEncode("Hello World"), "Hello%20World");
}

TEST(uriEncode, CommaIsEncoded) {
	EXPECT_EQ(uriEncode("skill,race"), "skill%2Crace");
}

TEST(uriEncode, MultipleSpecialCharacters) {
	EXPECT_EQ(uriEncode("Hello World!"), "Hello%20World%21");
}

TEST(uriEncode, EmptyStringReturnsEmpty) {
	EXPECT_EQ(uriEncode(""), "");
}

TEST(uriEncode, SlashIsEncoded) {
	EXPECT_EQ(uriEncode("a/b"), "a%2Fb");
}

// uriDecode tests

TEST(uriDecode, PercentEncodedSpaceIsDecoded) {
	EXPECT_EQ(uriDecode("Hello%20World"), "Hello World");
}

TEST(uriDecode, PlusSignDecodedAsSpace) {
	EXPECT_EQ(uriDecode("Hello+World"), "Hello World");
}

TEST(uriDecode, PercentEncodedCommaIsDecoded) {
	EXPECT_EQ(uriDecode("skill%2Crace"), "skill,race");
}

TEST(uriDecode, LowercaseHexIsDecoded) {
	EXPECT_EQ(uriDecode("Hello%20World%21"), "Hello World!");
	EXPECT_EQ(uriDecode("Hello%2cWorld"), "Hello,World");
}

TEST(uriDecode, EmptyStringReturnsEmpty) {
	EXPECT_EQ(uriDecode(""), "");
}

TEST(uriDecode, NoEncodingPassesThrough) {
	EXPECT_EQ(uriDecode("plain"), "plain");
}

TEST(uriDecode, InvalidPercentSequencePassesThrough) {
	EXPECT_EQ(uriDecode("%ZZ"), "%ZZ");
}

// Round-trip tests

TEST(uriRoundTrip, EncodeDecodeIsIdentity) {
	const std::string original = "Hello World! skill,race /path~value";
	EXPECT_EQ(uriDecode(uriEncode(original)), original);
}

TEST(uriRoundTrip, UnreservedCharsRoundTrip) {
	const std::string original = "abcABC012-_.~";
	EXPECT_EQ(uriEncode(original), original);
	EXPECT_EQ(uriDecode(original), original);
}

} // namespace