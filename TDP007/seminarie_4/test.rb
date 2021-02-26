require 'test/unit'
require './constraint-parser.rb'

class Tester < Test::Unit::TestCase
  def test_adder
    #testing the Adder class from constraint_networks.rb

    a = Connector.new('a')
    b = Connector.new('b')
    c = Connector.new('c')

    Adder.new(a, b, c)

    a.user_assign(2)
    b.user_assign(2)

    assert_equal(c.value, 4)

    a.forget_value "user"
    a.user_assign(-2)
    assert_equal(c.value, 0)

    b.forget_value "user"
    c.user_assign(10)
    assert_equal(b.value, 12)
  end

  def test_multip
    #testing the Multiplier class from constraint_networks.rb
    a = Connector.new('a')
    b = Connector.new('b')
    c = Connector.new('c')

    Multiplier.new(a, b, c)

    a.user_assign(2)
    c.user_assign(8)
    assert_equal(b.value, 4)

    c.forget_value "user"
    b.user_assign(43)
    assert_equal(c.value, 86)

    a.forget_value "user"
    a.user_assign(13)
    assert_equal(c.value, 559)
  end

  def test_c2f
    #testing our implementation of the celsius2fahrenheit equation
    c, f = celsius2fahrenheit

    c.user_assign(100)
    assert_equal(f.value, 212)

    c.user_assign(0)
    assert_equal(f.value, 32)

    c.user_assign(100)
    assert_equal(f.value, 212)

    c.forget_value "user"
    f.user_assign(100)
    assert_equal(c.value, 37)
  end

  def test_parser_first
    #testing the parser with the celsius2fahrenheit equation
    cp = ConstraintParser.new
    c, f = cp.parse("9*c=5*(f-32)")

    c.user_assign 100
    assert_equal(f.value, 212)

    c.user_assign -20
    assert_equal(f.value, -4)

    c.user_assign -40
    assert_equal(f.value, -40)

    c.forget_value "user"
    f.user_assign 451
    assert_equal(c.value, 232)
  end

  def test_parser_second
    #testing parser with gravity equation
    cp = ConstraintParser.new
    g, f, m=cp.parse("g=f/m")

    f.user_assign 50
    m.user_assign 9
    assert_equal(g.value, 5)

    #testing parser with Ohms law
    u, r, i=cp.parse("u=r*i")

    r.user_assign 200
    i.user_assign 4
    assert_equal(u.value, 800)
  end
end
