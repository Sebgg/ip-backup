#! /usr/bin/env ruby

def n_times(n)
  for i in 1..n
    yield
  end
end

n_times(3) {puts "Hello!"}

class Repeat
  def initialize(num)
    @num = num
  end

  def each
    for i in 1..@num
      yield
    end
  end
end

do_three = Repeat.new(3)
do_three.each { puts "Hooray!" }
