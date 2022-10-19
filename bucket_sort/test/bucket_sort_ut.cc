#include "../bucket_sort.h"

#include <gtest/gtest.h>
#include <string>

using namespace FunAndGames;
using namespace testing;

TEST(bucket_sort, sort_by_single_digit)
{
  std::vector unsorted = { 321, 432, 892, 753, 193, 229, 336, 176, 555, 712,
                           300, 715, 192, 475, 250, 622, 274, 841, 188 };
  std::vector sorted = { 300, 250, 321, 841, 432, 892, 712, 192, 622, 753,
                         193, 274, 555, 715, 475, 336, 176, 188, 229 };

  bucket_sort(
    unsorted.begin(), unsorted.end(), [](auto num) { return num % 10; });
  EXPECT_EQ(unsorted, sorted);
}

TEST(bucket_sort, sort_digit_by_digit)
{
  std::vector unsorted = { 321, 432, 892, 753, 193, 229, 336, 176, 555, 712,
                           300, 715, 192, 475, 250, 622, 274, 841, 188 };
  std::vector sorted = { 176, 188, 192, 193, 229, 250, 274, 300, 321, 336,
                         432, 475, 555, 622, 712, 715, 753, 841, 892 };

  bucket_sort(
    unsorted.begin(), unsorted.end(), [](auto num) { return num % 10; });
  bucket_sort(
    unsorted.begin(), unsorted.end(), [](auto num) { return (num / 10) % 10; });
  bucket_sort(
    unsorted.begin(), unsorted.end(), [](auto num) { return num / 100; });

  EXPECT_EQ(unsorted, sorted);
}

struct Record
{
  std::string name;
  std::string department;

  Record(auto _name, auto _department)
    : name(_name)
    , department(_department)
  {
  }

  bool operator==(const Record& other) const
  {
    return name == other.name && department == other.department;
  }
};

TEST(bucket_sort, sort_by_field)
{
  std::vector<Record> unsorted = {
    { "Dorothea Brooke", "RnD" },       { "Arthur Brooke", "HR" },
    { "Nicholas Bulstrode", "DevOps" }, { "Elinor Cadwallader", "RnD" },
    { "Humphrey Cadwallader", "RnD" },  { "Edward Casaubon", "Marketing" },
    { "James Chettam", "HR" },          { "Camden Farebrother", "RnD" },
    { "Peter Featherstone", "DevOps" }, { "Caleb Garth", "DevOps" }
  };

  std::vector<Record> sorted = {
    { "Nicholas Bulstrode", "DevOps" }, { "Peter Featherstone", "DevOps" },
    { "Caleb Garth", "DevOps" },        { "Arthur Brooke", "HR" },
    { "James Chettam", "HR" },          { "Edward Casaubon", "Marketing" },
    { "Dorothea Brooke", "RnD" },       { "Elinor Cadwallader", "RnD" },
    { "Humphrey Cadwallader", "RnD" },  { "Camden Farebrother", "RnD" }
  };

  bucket_sort(unsorted.begin(), unsorted.end(), [](auto record) {
    return record.department;
  });

  EXPECT_EQ(unsorted, sorted);
}

enum class Color
{
  RED,
  GREEN,
  BLUE
};

TEST(bucket_sort, counting_sort)
{
  std::vector unsorted = { Color::RED, Color::GREEN, Color::BLUE,
                           Color::RED, Color::GREEN, Color::BLUE,
                           Color::RED, Color::GREEN, Color::BLUE };
  std::vector sorted = { Color::RED,   Color::RED,   Color::RED,
                         Color::GREEN, Color::GREEN, Color::GREEN,
                         Color::BLUE,  Color::BLUE,  Color::BLUE };

  bucket_sort(unsorted.begin(), unsorted.end());

  EXPECT_EQ(unsorted, sorted);
}
