require './uppgift1'
require './uppgift2'
require 'test/unit'

class TestFaculty < Test::Unit::TestCase
  def test_uppg1
    assert_equal(score_diff("- - - - - - 5 - 3"), 2)
    assert_equal(score_diff("- - - - - - 3 - 5"), 2)
    assert_equal(temp_diff("- 5 3"), 2)
    assert(temp_diff("- 10 2") < 10)

    $stdout = StringIO.new
    football_listing('./football.txt')

    assert_equal($stdout.string.split("\n")[3],
    "   11. Southampton     38    12   9  17    46  -  54    45")

    assert_equal($stdout.string.split("\n")[20],
    "the team with least difference is Aston_Villa")

    $stdout = StringIO.new
    weather_listing('./weather.txt')

    assert_equal($stdout.string.split("\n")[2],
    "  13  70    59    65          55.0       0.00 H       150  3.0 150   8 10.0  83 59 1012.6")

    assert_equal($stdout.string.split("\n")[30],
    "the day with least difference is day 14")
  end

  def test_uppg2
    $stdout = StringIO.new

    listener_test

    assert_equal($stdout.string.split("\n")[1],
    "Event name: The Dark Carnival - 101.9FM")

    assert_equal($stdout.string.split("\n")[6],
    "Event date: 2008-01-04 10:00pm EST")

    assert_equal($stdout.string.split("-"*75)[3],"\n"+
    "Event name: Sinister Sundays\n" +
    "Organizer: The Bohemian\n" +
    "Location: Belleville, Ontario\n" +
    "Street address: 248 Front Street\n" +
    "Posted by: LunaSlave\n" +
    "Event date: 2008-01-06\n" +
    "Event description:\n" +
    "Gothic, Industrial, Dark Alternative w/ DJ LunaSlave\n")
  end
end
