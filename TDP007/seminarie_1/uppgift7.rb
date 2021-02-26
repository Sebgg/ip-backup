#! /usr/bin/env ruby

class Integer
  def fib
    n_nor = 1
    n_high = 1

    1.upto(self-1) do n_nor, n_high = n_high, n_high + n_nor
    end
    n_nor
  end
end

puts 3.fib
