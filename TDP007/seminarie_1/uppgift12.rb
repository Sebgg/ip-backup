#! /usr/bin/env ruby

def regnr(string)
  if(/([^IQV][A-Z]{3}[0-9]{3})/ =~ string)
    '"' + $&.gsub(/\s/, '') + '"'
  else
    false
  end
end

puts regnr("Min bil heter FMA297.")
puts regnr("XQT784")
