#! /usr/bin/env ruby

def score_diff(game_string)
  game_arr = game_string.split
  (game_arr[6].to_i - game_arr[8].to_i).abs
end

def temp_diff(weather_string)
  weather_arr = weather_string.split
  weather_arr[1].to_i - weather_arr[2].to_i
end

def football_listing(text_file)
  matches = File.readlines(text_file).select{ |lines| lines =~ /\d+\.\s.+/  }
  matches = matches.sort{ |scoreF,scoreA| score_diff(scoreF) <=> score_diff(scoreA) }

  puts matches
  matches[0] =~ /[A-Za-z_]+/
  puts "the team with least difference is #{$&}"
  puts "\n"
end

def weather_listing(text_file)
  days = File.readlines(text_file).select{ |lines| lines =~ /^\s*\d/  }
  days = days.sort{ |tempdiff1,tempdiff2| temp_diff(tempdiff1) <=> temp_diff(tempdiff2) }

  puts days
  days[0] =~ /^\s*\d+/
  puts "the day with least difference is day #{$&.gsub(/\s+/, "")}"
  puts "\n"
end

#football_listing('./football.txt')
#weather_listing('./weather.txt')
