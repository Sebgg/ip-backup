#! /usr/bin/env ruby

class String
  def acronym
    self.split.map{|x| x[0]}.join.upcase
  end
end

puts 'Laugh out loud'.acronym
