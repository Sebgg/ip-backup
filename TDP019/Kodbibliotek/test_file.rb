require './rules'
require 'test/unit'

class TestLang
  lang_parse = Lang.new

  def test_simple_int
     assert_equal(lang_parse.evaluate("2 + 5"), 7)
  end
end
