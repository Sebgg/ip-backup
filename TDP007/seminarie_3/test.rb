require './uppgift1'
require './uppgift2'
require 'test/unit'

class TestFaculty < Test::Unit::TestCase
  def test_uppg1
    kalle = Person.new("Volvo", "58435", 2, "M", 32)
    assert_equal(kalle.evaluate_policy("policy.rb"), 15.66)

    karl = Person.new("BMW", "58727", 17, "M", 60)
    assert_equal(karl.evaluate_policy("policy.rb"), 21)
    
    biggan = Person.new("Volvo", "58937", 41, "F", 20)
    assert_equal(biggan.evaluate("policy.rb"), 30)
  end

  def test_uppg2
    testparser = SimpleParser.new
    assert_equal(testparser.evaluate('(or true false)'), true)
    assert_equal(testparser.evaluate('(not true)'), false)
    assert_equal(testparser.evaluate('(not (true and (true or false)))', false)
  end
end
