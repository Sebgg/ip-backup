#! /usr/bin/env ruby

def get_username(line)
  name_pos = /[a-zA-ZåäöÅÄÖ]{1,}:\s/ =~ line

  $'
end

puts get_username('inputname: Brian')
