#! /usr/bin/env ruby
class Person
  def initialize(car_maker, postalnumber, license_duration, gender, age)
    @car_maker = car_maker
    @postalnumber = postalnumber
    @license_duration = license_duration
    @gender = gender
    @age = age
    @points = 0
  end

  def evaluate_policy(filename)
    self.instance_eval(File.read(filename))

    @points
  end

  def rule(*args)
    conds = args[0..-2]
    points = args[-1]
    @points = eval(@points.to_s + points.to_s) if conds.all?
  end

  def car(scores)
    if scores.keys.include?(@car_maker)
      @points = eval(@points.to_s + scores[@car_maker].to_s)
    end
  end

  def pn(scores)
    if scores.keys.include?(@postalnumber)
      @points = eval(@points.to_s + scores[@postalnumber].to_s)
    end
  end

  def exp(scores)
    scores.keys.each do |range|
      if range.include?(@license_duration)
        @points = eval(@points.to_s + scores[range].to_s)
      end
    end
  end

  def gender(scores)
    if scores.keys.include?(@gender)
      @points = eval(@points.to_s + scores[@gender].to_s)
    end
  end

  def age(scores)
    scores.keys.each do |range|
      if range.include?(@age)
        @points = eval(@points.to_s + scores[range].to_s)
      end
    end
  end
end

#karl = Person.new("Volvo", "58435", 2, "M", 32)
#karl.evaluate_policy("policy.rb")
