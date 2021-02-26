#! /usr/bin/env ruby
require './rdparse'

class SimpleParser
  def initialize
    @simpleParser = Parser.new("simple parser") do
      token(/\s+/)
      token(/\d+/) {|m| m.to_i }
      token(/\w+/) {|m| m}
      token(/./) {|m| m }

      start :VALID do
        match(:ASSIGN)
        match(:EXPR)
      end

      rule :ASSIGN do
        match('(', 'set', :VAR, :EXPR, ')') {|_, _, a, b, _| a = b}
      end

      rule :EXPR do
        #==or====
        match('(', 'or', :EXPR, :EXPR, ')') {|_, _, a, b, _| a || b}

        #==and===
        match('(', 'and', :EXPR, :EXPR, ')') {|_, _, a, b, _| a && b}

        #==not===
        match('(', 'not', :EXPR, ')') {|_, _, a, _| !a}

        #==term==
        match(:TERM)
      end

      rule :TERM do
        match('true') { true }
        match('false') { false }
        match(:VAR)
      end

      rule :VAR do
        match(Integer)
        match(Float)
        match(String)
        match(TrueClass)
        match(FalseClass)
      end
    end
  end

  def done(str)
    ["quit","exit","bye",""].include?(str.chomp)
  end

  def execute
    print "[simple parser] "
    str = gets
    if done(str) then
      puts "Bye."
    else
      puts "=> #{@simpleParser.parse str}"
      execute
    end
  end

  #function for testing the parsers calculations
  def evaluate(expression)
    @simpleParser.parse expression
  end

  def log(state = true)
    if state
      @simpleParser.logger.level = Logger::DEBUG
    else
      @simpleParser.logger.level = Logger::WARN
    end
  end
end
