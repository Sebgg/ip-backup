#! /usr/bin/env ruby
require './uppgift5'
require 'date'

class Person
  def initialize(name = "John", surname = "Smith", age = 0)
    @name = PersonName.new(name, surname)
    @age = age
    calculate_year
  end

  def info
    @name.fullname + ' ' + @age.to_s
  end

  def birthyear
    @birthyear
  end

  def age
    @age
  end

  def calculate_year
    temp = DateTime.now
    @birthyear = temp.year - @age
  end

  def calculate_age
    temp = DateTime.now
    @age = temp.year - @birthyear
  end

  def age=(age)
    @age = age
    calculate_year
  end

  def birthyear=(birthyear)
    @birthyear = birthyear
    calculate_age
  end

  def name=(name)
    @name.fullname = name
  end
end

kalle = Person.new("Kalle", "Kula", 50)

puts kalle.info
kalle.birthyear = 1900
puts kalle.info
