@@scopes = [{}]
@@name_scope = [[]]
@@current_scope = 0
@@return_value = nil

def new_scope
  @@current_scope += 1
  @@name_scope[@@current_scope] = []
  @@scopes[@@current_scope] = {}
end

def close_scope
  @@current_scope -= 1
  @@scopes.pop
  @@name_scope.pop
  if @@current_scope < 0
    abort("Scope out of bounds error!")
  end
end

def look_up(name)
  i = @@current_scope
  while i >= 0
    if @@name_scope[i].include? name
      return i
    end
    i -= 1
  end
  return nil
end

def variable_handler(name, value)
  if look_up(name) != nil
    @@scopes[look_up(name)][name] = value.eval
  else
    @@scopes[@@current_scope][name] = value.eval
  end
end

def func_scope_handler(name, value)
  @@name_scope[@@current_scope].append(name)
  @@scopes[@@current_scope][name] = value.eval
end

class Value_handler
  attr_accessor :name
  def initialize(name)
    @name = name
  end

  def eval
    @@scopes[look_up(@name)][@name]
  end
end

class StmtNode
  attr_accessor :stmt, :stmt_list
  def initialize(stmt_list)
    @stmt_list = stmt_list
  end

  def eval
    @stmt_list.each do |stmt|
      stmt.eval
    end
  end
end

class Variable
  attr_accessor :name, :value
  def initialize(name, value)
    @@name_scope[@@current_scope].push(name)
    @name = name
    @value = value
  end

  def eval
    variable_handler(@name, @value)
  end
end

class Rreturn
  def initialize(stmt)
    @stmt = stmt
  end

  def eval
    return @stmt.eval
  end
end

class If_handler
  attr_accessor :pred, :statements
  def initialize(pred, statements)
    @pred = pred
    @statements = statements
  end

  def eval
    nil
  end
end

class Iif
  attr_accessor :stmts_
  def initialize(stmts_)
    @stmts_ = stmts_
  end

  def eval
    new_scope
    @stmts_.each do |stmts|
      if stmts.pred.eval == true
        stmts.statements.each do |stmt|
          if stmt.instance_of? Rreturn and @@return_value == nil
            @@return_value = stmt.eval
            break
          else
            stmt.eval
          end
        end
        break
      end
    end
    close_scope
  end
end

class Loop_handler
  attr_accessor :name, :start, :stop, :stmts
  def initialize(variable, start, stop, statements)
    @name = variable
    @start = start
    @stop = stop
    @stmts = statements
  end

  def eval
    nil
  end
end

class Lloop
  attr_accessor :stmt
  def initialize(stmt)
    @stmt = stmt
  end

  def eval
    new_scope
    if @stmt.name.class == Compare_handler
      while @stmt.name.eval == true
        @stmt.stmts.each do |statement|
          if statement.instance_of? Rreturn
            @@return_value = statement.eval
            break
          else
            statement.eval
          end
        end
      end
    else
      curr_place = @stmt.start
      iterator = curr_place.eval
      variable_handler(@stmt.name, curr_place)
      while iterator < @stmt.stop.eval
        @stmt.stmts.each do |statement|
          statement.eval
        end
        variable_handler(@stmt.name, Iinteger.new(iterator + 1))
        iterator += 1
      end
      close_scope
    end
  end
end

class Ffunc
  attr_accessor :name, :params, :stmts
  def initialize(name, params, stmts)
    @name = name
    @params = params
    @stmts = stmts
  end

  def eval
    scope = look_up(@name)
    if scope != nil
      abort("Error: Function already exists!")
    else
      @@scopes[@@current_scope][@name] = self
      @@name_scope[@@current_scope].append(@name)
    end
  end

  def get_vals()
    return @params, @stmts
  end
end

class Func_call
  def initialize(name, params)
    @name = name
    @args = params
  end

  def eval
    scope = look_up(@name)
    if scope == nil
      abort("Error: Function not found!")
    end

    @params, @stmts = @@scopes[scope][@name].get_vals()

    if @args.size() != @params.size()
      abort("Error: Wrong number of arguments given!")
    end
    new_scope
    @params.each_with_index do |param, index|
      func_scope_handler(param.eval, @args[index])
    end
    @stmts.each do |stmt|
      if stmt.instance_of? Rreturn and @@return_value == nil
        @@return_value = stmt.eval
        break
      else
        stmt.eval
      end
    end
    close_scope()

    if @@return_value != nil
      retur = @@return_value
      @@return_value = nil
    end
    return retur
  end
end

class Sstring
  attr_accessor :value
  def initialize(value)
    @value = value
  end

  def eval
    return @value
  end
end

class MathOP
  attr_accessor :lhs, :rhs, :op
  def initialize(lhs, op, rhs)
    @lhs = lhs
    @rhs = rhs
    @op = op
  end

  def eval
    case @op
    when '+'
      return @lhs.eval + @rhs.eval
    when '-'
      return @lhs.eval - @rhs.eval
    when '*'
      return @lhs.eval * @rhs.eval
    when '/'
      return @lhs.eval.to_f / @rhs.eval.to_f
    else nil
    end
  end
end

class Compare_handler
  attr_accessor :op, :lhs, :rhs
  def initialize(op, lhs, rhs)
    @op = op
    @lhs = lhs
    @rhs = rhs
  end

  def eval
    case @op
    when '<'
      return @lhs.eval < @rhs.eval
    when '>'
      return @lhs.eval > @rhs.eval
    when '>='
      return @lhs.eval >= @rhs.eval
    when '<='
      return @lhs.eval <= @rhs.eval
    when '!='
      return @lhs.eval != @rhs.eval
    when '=='
      return @lhs.eval == @rhs.eval
    #simpler eval for above??
    when 'and'
      return @lhs.eval && @rhs.eval
    when 'or'
      return @lhs.eval || @rhs.eval
    when 'not'
      return !@lhs.eval
    when 'xor'
      return (@lhs.eval && !@rhs.eval) || (!@lhs.eval && @rhs.eval)
    when 'xnor'
      return @lhs.eval == @rhs.eval
    when 'nand'
      return !(@lhs.eval && @rhs.eval)
    else nil
    end
  end
end

class Iinteger
  attr_accessor :value
  def initialize(value)
    @value = value
  end

  def eval
    return @value
  end
end

class Ffloat
  attr_accessor :value
  def initialize(value)
    @value = value.to_f
  end

  def eval
    return @value
  end
end

class Bbool
  attr_accessor :value
  def initialize(value)
    if value
      @value = true
    else
      @value = false
    end
  end

  def eval
    return @value
  end
end


class Pprint
  def initialize(obj)
    @obj = obj
  end

  def eval
    puts @obj.eval
  end
end
