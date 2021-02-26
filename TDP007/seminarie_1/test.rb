require './uppgift1'
require './uppgift2'
require './uppgift5'
require './uppgift6'
require './uppgift7'
require './uppgift8'
require './uppgift10'
require './uppgift12'
require 'test/unit'

class TestFaculty < Test::Unit::TestCase
  def test_uppg1
    #Test n_times
    assert_equal(n_times(6) {puts "Hello!"}, 1..6)
    assert_equal(n_times(3) {puts "Je;;o"}, 1..3)
    #Test Repeat
    re_test = Repeat.new(3)
    assert_equal(re_test.each {puts "yikes"}, 1..3)
  end

  def test_uppg2
    assert_equal(faculty(20), (1..20).inject {|total, x| total * x})
    assert_equal(2432902008176640000, faculty(20))
    assert_equal(362880, faculty(9))
  end

  def test_uppg5
    name_test = PersonName.new("Bulle", "Bulldog")
    assert_equal("Bulle Bulldog", name_test.fullname)
    name_test.fullname = "Karl Berglin"
    assert_equal("Karl Berglin", name_test.fullname)
  end

  def test_uppg6
    anders = Person.new("Anders", "Andersson", 50)
    assert_equal(1969, anders.birthyear)
    assert_equal(50, anders.age)
    anders.age=(70)
    assert_equal('Anders Andersson 70', anders.info)
  end

  def test_uppg7
    assert_equal(2, 3.fib)
    assert_equal(8, 6.fib)
  end

  def test_uppg8
    assert_equal('LOL', 'Laugh out loud'.acronym)
    assert_equal('DWIM', 'Do what I mean!!'.acronym)
  end

  def test_uppg10
    assert_nil(get_username('inputnam3: Brian'))
    assert_equal(get_username('hallihallå: Jörgen'), "Jörgen")
  end

  def test_uppg12
    assert_equal('"FMA297"', regnr("Min bil heter FMA297."))
    assert_equal(false, regnr("XQT784"))
  end
end
