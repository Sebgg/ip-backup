#! /usr/bin/usr ruby

class PersonName
  def initialize(name, surname)
    @name = name
    @surname = surname
  end

  def fullname
    @name + ' ' + @surname
  end

  def fullname=(fullname)
    @name = fullname.split(' ')[0]
    @surname = fullname.split(' ')[1]
  end
end

karl = PersonName.new('Karl', 'Karlsson')
#puts karl.fullname()
karl.fullname=('Karl Bertsson')
#puts karl.fullname()
