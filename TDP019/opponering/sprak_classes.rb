


class StmtList
  attr_accessor :stmt, :stmt_list

  def initialize(stmt,stmt_list)
    @stmt = stmt
    @stmt_list = stmt_list
  end

  def eval

    if @stmt.class == Return_stmt #stops further evaling if a statement is a return
        return @stmt.eval
    end

    if @stmt.class == If or @stmt.class == Elif or @stmt.class == Else or @stmt.class == For_stmt or @stmt.class == While_stmt #stops further evaling if a statement is a return

        temp = @stmt.eval
        if temp.class == Integer or temp.class == String or temp.class == Boolean#stops further evaling if a statement is a return

            return temp

        end

    end
#
    @stmt.eval
    @stmt_list.eval

  end
end
# //println x
#   if true{
#     println "hhheaheahhaeheaheha"
#     x = x + 20
#     return x
#   }
#
# return 5
# }
# elif true
# {
# return 10
# }
# print x
#   print 55

class Return_stmt
  attr_accessor :eval, :value
  def initialize(a)
    @value = a
  end
  def eval
    return @value.eval
  end
end


class Scope
  attr_accessor :currentscope, :scope_counter

  def initialize
    @scopevariables = [{}]
    @currentscope = @scopevariables[0]
    @scope_counter = 0

  end

  def move_up
    if @scope_counter > 0
        @currentscope = @scopevariables[@scope_counter-1]
        @scopevariables.pop()
        @scope_counter -= 1
    end
  end

  def move_down
      @scopevariables[@scope_counter] = @currentscope.clone

    @scopevariables << {}
    @scope_counter += 1
    @scopevariables[@scope_counter] = @currentscope.clone #@scopevariables[@scope_counter-1].clone

    @currentscope = @scopevariables[@scope_counter].clone

  end
  def get_scope
    @scope_counter
  end
end



class Func_call_stmt
    def initialize(a,b = nil)
      @id = a
      @args = b

    end
    def eval
      if @args != nil
        0.upto(@args.length-1) do |arg|

          if @args[arg].class.method_defined? :eval
            @args[arg] = @args[arg].eval
          end

        end
      end

      @@is_in_function = true
      @@scope.move_down
        if @args != nil #calls function with arguments
          result = @@function_keys[@id].func_call(@args)
        else
          result = @@function_keys[@id].func_call
        end

      @@scope.move_up
      @@is_in_function = false

      return result
    end
end

@@function_keys = {} #global hash with function objects

class Func_def_stmt
    attr_accessor :params

    def initialize(a,b = {},c)
      @id = a
      @params = b
      @stmt_list = c

    end
    def eval
      @@function_keys[@id] = self
    end


    def func_call(args = nil) #"eval" function for when a method is called

      if args != nil
        @args = args

        0.upto(@params.length-1) do |para|

          tmp = @args[para]
          @@scope.currentscope[@params[para].intern] = tmp

        end
        temp = @stmt_list.eval
      else
        temp = @stmt_list.eval
      end


      return temp
    end
end


class For_stmt
    def initialize(var,condition,expr,statements)
      @var = var
      @condition = condition
      @expr = expr
      @statements = statements
    end
    def eval
        @var.eval
        while @condition.eval
            @statements.eval
            @expr.eval
        end

    end
end


class While_stmt
    def initialize(condition,statements)
      @condition = condition
      @statements = statements
    end
    def eval
        while @condition.eval
            @statements.eval
        end
    end
end


class Assign_stmt
  attr_accessor :eval

  def initialize(a,b)
    @var = a
    @value = b

  end

  def eval
        ten = @value.eval
        @@scope.currentscope[@var.var] = ten

      return @value.eval
  end
end

class Get_Var_stmt
  attr_accessor :eval
  attr_reader :var
  def initialize(a)
    @var = a.intern
  end
  def eval

      return @@scope.currentscope[@var] != nil ? @@scope.currentscope[@var] : "Undefined"
  end
end


class Add_expr
  attr_accessor :eval
  def initialize(a,b,c)
    @left_term = a
    @operator = b
    @right_term = c
  end

  def eval
    return @left_term.eval.send(@operator, @right_term.eval)
  end

end

class Mult_expr
    attr_accessor :eval
    def initialize(a,b,c)
        @left_term = a
        @operator = b
        @right_term = c
    end

    def eval
        return @left_term.eval.send(@operator, @right_term.eval)
    end
end


# ===================== VARIABLES =======================

class Digit
    attr_accessor :eval
    def initialize(a)
      @term = a
    end
    def eval
        return instance_eval(@term)
    end
end

class String
    attr_accessor :eval
    def initialize(a)
      @term = a
    end

    def eval
        return @term
    end
end

class Boolean
    def initialize value
      @value = value
    end
    def eval
      return @value
    end
end


# ===================== LOGIC =======================

class And_expr
  def initialize(a,b)
      @left_term = a
      @right_term = b
  end

  def eval
      return @left_term.eval == @right_term.eval
  end
end

class Or_expr
  def initialize(a,b)
      @left_term = a
      @right_term = b
  end

  def eval
      return @left_term.eval || @right_term.eval
  end
end

class Not_expr
  def initialize value
    @value = value
  end
  def eval
    return !@value.eval
  end
end


class Comp_expr

  attr_accessor :eval
  def initialize(a,x,b)
      @left_term = a
      @operator = x
      @right_term = b
  end

  def eval
    return @left_term.eval.send(@operator, @right_term.eval)
  end
end


# ===================== I/O =======================

class Print
  attr_accessor :eval
  def initialize value
    @value = value
  end
  def eval

      print @value.eval
  end

end

class Println
  attr_accessor :eval
  def initialize value
    @value = value
  end
  def eval
    puts @value.eval
  end
end



@@prev_if_val = false

class If # condition
  attr_accessor :eval, :statements
  def initialize(a,b)
      @condition = a
      @statements = b
  end

  def eval
    @@prev_if_val = false

    if(@condition.eval)

      return @statements.eval

    else
        @@prev_if_val = true
    end
  end
end

class Elif # condition
  attr_accessor :eval, :statements
  def initialize(a,b)
      @condition = a
      @statements = b
  end

  def eval

    if (@@prev_if_val)
        if(@condition.eval)


            @@prev_if_val = false
          return @statements.eval
        end
    end
  end
end

class Else # condition
  attr_accessor :eval, :statements
  def initialize(a)
      @statements = a
  end
  def eval
  if (@@prev_if_val)
      @@prev_if_val = false
      return @statements.eval
    end
  end

end






















#
