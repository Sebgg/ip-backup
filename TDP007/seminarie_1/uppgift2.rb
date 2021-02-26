#! /usr/bin/usr ruby

puts (1..20).inject {|total, x| total * x}

def faculty(n)
  (1..n).inject {|total, x| total * x}
end

puts faculty(9)
