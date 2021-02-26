require "./sprak"
require 'test/unit'

class Spraktest < Test::Unit::TestCase
    def test_simple

        rd = Sprak.new
        code = File.read("./code")



        assert_equal(90, rd.parse('return 90'))

        assert_equal(10, rd.parse('
            if true
            {
                return 10
            }
            '))

        assert_equal(10, rd.parse('
            if false
            {
                return 20
            }
            else if true
            {
                return 10
            }
            '))
        assert_equal(10, rd.parse('
            if false
            {
                return 20
            }
            else if false
            {
                return 20
            }
            else
            {
                return 10
            }

            '))
        assert_equal(10, rd.parse('
            if true or false
            {
                return 10
            }
            '))
        assert_equal(true, rd.parse('
            if false or false or false or false or true
            {
                return true
            }
            '))
        assert_equal(false, rd.parse('
            if true and true
            {
                return not true
            }
            '))
        assert_equal(true, rd.parse('
            if 10 < 9
            {
                return true
            }
            '))
        assert_equal(10, rd.parse('
            func test()
            {
                a = 10
                return a
            }
            return test()

            '))
        assert_equal(1337, rd.parse('
            func test(a)
            {
                return a
            }
            return test(1337)

            '))
        assert_equal(10, rd.parse('
            func test()
            {
                if false {
                  a = 5
                }
                else {
                  a = 10
                }

                return a
            }
            return test()

            '))
        assert_equal(77777777777777, rd.parse('
            func test()
            {
                if true
                  {
                  println "hahaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  if true{
                    return 77777777777777
                  }
                  return 5
                  }
                elif false
                  {
                  return 10
                  }
                  else {
                  return 9999
                  }

                return 20
            }
            return test()
            '))


    end
end
