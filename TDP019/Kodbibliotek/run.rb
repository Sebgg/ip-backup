require './rules'

if ARGV.length == 1
  $VERBOSE = nil
  a = Lang.new
  a.log(false)
  a.file_parse(ARGV[0])
else
  abort("No file specified!")
end

# a = Lang.new
# a.log(false)
# a.file_parse("./test.txt")
