require './parser'
require './classes'

class Lang
  def initialize
    @langParser = Parser.new('lang parser') do
      token(/\!comment[^!]*\!end/) #ignores multiline comments
      token(/(!!.+$)/) #ignores single line comments
      token(/\s+/) #ignores whitespaces
      token(/\d+/) {|m| m.to_i } #match integer
      token(/"[^\"]*"/) {|m| m } #match "string"
      token(/'[^\']*'/) {|m| m } #match 'string'
      token(/Print/) {|m| m } #match print_stmts
      token(/Loop/) {|loop| loop} #match loop_stmt
      token(/is/) #part of compare_handler
      token(/If/) {|iff| iff } #match if_stmt
      token(/Elseif/) {|elseif| elseif } #match elseif_stmt
      token(/Else/) {|els| els } #match else_stmt
      token(/Return/) {|retur| retur} #match return_stmt
      token(/(\+=|-=|\*=|\/=|\+\+|--|\+|\-|\*|\/|!=|\.|,|\]|\[|{|}|\(|\)|\:|<=|>=|<|>|==|=)/) {|m| m}
      token(/([a-z]+)/) {|m| m } #match variable name
      token(/./) {|m| m }

      start :run do
        match(:stmt_list) {|stmt_list|
          StmtNode.new(stmt_list)
        }
      end

      rule :stmt_list do
        match(:stmt) {|stmt| [stmt] }
        match(:stmt_list, :stmt) {|stmt_list, stmt|
          stmt_list.append(stmt)
          stmt_list
        }
      end

      rule :stmt do
        match(:if_stmt)
        match(:print_stmt)
        match(:func_stmt)
        match(:loop_stmt)
        match(:return_stmt)
        match(:assign_stmt)
      end

      rule :print_stmt do
        match('Print', '(', :expr, ')') {|_, _, value, _| Pprint.new(value)}
      end

      ##############BeginReturn#########
      rule :return_stmt do
        match('Return', :func_call) {|_, stmt|
          Rreturn.new(stmt)
        }
        match('Return', :expr) {|_, stmt|
          Rreturn.new(stmt)
        }
      end
      ##############EndReturn###########
      ##################################
      ##############BeginAssign#########
      rule :assign_stmt do
        match(/[a-z]+/ , '=', :func_call) {|name, _, value| Variable.new(name, value)}
        match(/[a-z]+/ , '=', :expr) {|name, _, value| Variable.new(name, value)}
      end
      ##############EndAssign###########
      ##################################
      ##############BeginIf#############
      rule :if_stmt do
        match(:if_rule){|m| Iif.new(m)}
      end

      rule :if_rule do
        match(:if, :elsif, :els){|ifstmt, elsstmt, els|
          [ifstmt] + elsstmt + [els] }
        match(:if, :elsif){|ifstmt, elsstmt|
          [ifstmt] + elsstmt }
        match(:if, :els){|ifstmt, els|
          [ifstmt] + [els] }
        match(:if){|ifstmt|
          [ifstmt] }
      end

      rule :if do
        match('If', '(', :bool_logic, ')',
              '{', :stmt_list, '}'){|_, _, pred, _, _, stmts, _|
              If_handler.new(pred , stmts)}
      end

      rule :elsif do
        match('Elseif', '(', :bool_logic, ')',
              '{', :stmt_list, '}'){|_, _, pred, _, _, stmts, _|
              [If_handler.new(pred, stmts)]}
        match(:elsif, :elsif){|elsifstmt, elsifstmt2|
              elsifstmt += elsifstmt2}
      end

      rule :els do
        match('Else', '{', :stmt_list, '}'){|_, _, statements, _|
              pred = Compare_handler.new('<', Iinteger.new(1), Iinteger.new(2))
              If_handler.new(pred, statements)}
      end
      ##############EndIf###############
      ##################################
      ##############BeginLoop###########
      rule :loop_stmt do
        match(:loop_rule){|m| Lloop.new(m)}
      end

      rule :loop_rule do
        match(:while_loop)
        match(:for_loop)
        match(:times_loop)
      end

      rule :while_loop do
        match('Loop', 'while', :bool_logic,
          '{', :stmt_list, '}') {|_, _, logic, _, stmts, _|
            Loop_handler.new(logic, Iinteger.new(0),
            Iinteger.new(1), stmts)
          }
        end

      rule :for_loop do
        match('Loop', :loop_variable, 'from', :expr, 'to', :expr, '{',
              :stmt_list, '}'){|_, var, _, x1, _, x2, _, stmts, _|
                Loop_handler.new(var, x1, x2, stmts)
              }
      end

      rule :times_loop do
        match('Loop', :expr, 'times', '{', :stmt_list,
              '}'){|_, expression, _, _, stmts, _|
                Loop_handler.new(Variable.new("n/a", nil), Iinteger.new(0),
                                expression, stmts)
              }
      end

      rule :loop_variable do
        match(/[a-z]+/){|name|
              Variable.new(name, nil)
              name
            }
      end
      ##############EndLoop############
      ##################################
      ##############Boolean#############
      rule :bool_logic do
        match(:logic_operand, :logic_op, :logic_operand) {|lhs, op, rhs|
          Compare_handler.new(op, lhs, rhs)}
        match(:logic_operand, :logic_op, :bool_logic)
        match(:bool)
        match(:logic_operand)
      end

      rule :logic_op do
        match('and')
        match('or')
      end

      rule :logic_operand do
        match(:bool_expr)
        match(:bool)
      end

      rule :bool_expr do
        match(:expr, 'greater', 'than', 'or', 'equal', 'to', :expr){|lhs, _, _, _, _, _, rhs|
          Compare_handler.new('>=', lhs, rhs) }
        match(:expr, 'less', 'than', 'or', 'equal', 'to', :expr){|lhs, _, _, _, _, _, rhs|
          Compare_handler.new('<=', lhs, rhs) }
        match(:expr, 'less', 'than', :expr){|lhs, _, _, rhs|
              Compare_handler.new('<', lhs, rhs) }
        match(:expr, 'greater', 'than', :expr){|lhs, _, _, rhs|
              Compare_handler.new('>', lhs, rhs) }
        match(:expr, 'equal', 'to', :expr){|lhs, _, _, rhs|
              Compare_handler.new('==', lhs, rhs) }
        match(:expr, 'not', 'equal', 'to', :expr){|lhs, _, _, _, rhs|
              Compare_handler.new('!=', lhs, rhs) }
      end
      ##############End Boolean#########
      ##################################
      ##############BeginFunc###########
      rule :func_stmt do
        match(:func_init)
        match(:func_call)
      end

      rule :func_init do
        match('define', /[A-z]+/, '(', :args , ')',
        '{', :stmt_list, '}'){|_, name, _, params, _, _, stmts, _|
          Ffunc.new(name, params, stmts)
        }
      end

      rule :func_call do
        match(:func_var, '(', :params, ')'){|func, _, params, _|
              Func_call.new(func, params)}
      end

      rule :func_var do
        match(/[a-z]+/){|name| name}
      end
      rule :args do
        match(:args, ',', :arg) {|args, _, arg|
          args + [arg]
        }
        match(:arg) {|arg| [arg]}
      end
      rule:arg do
        match(:expr){|a|
          Variable.new(a.eval, nil)
          a}
      end

      rule :params do
        match(:params, ',', :param) {|params, _, param|
          params + [param]
        }
        match(:param) {|param| [param]}
      end

      rule :param do
        match(:func_call)
        match(:expr)
      end
      ##############EndFunc#############
      ##################################


      rule :expr do
        match(:expr, '+', :term) {|a, op, b| MathOP.new(a, op, b) }
        match(:expr, '-', :term) {|a, op, b| MathOP.new(a, op, b) }
        match(:term)
      end

      rule :term do
        match(:term, '*', :atom) {|a, op, b| MathOP.new(a, op, b) }
        match(:term, '/', :atom) {|a, op, b| MathOP.new(a, op, b) }
        match(:atom)
      end

      rule :atom do
        match(:variable)
        match(:datatype)
        match('(', :expr, ')') {|_, a, _| a }
      end

      rule :variable do
        match(/[a-z]+/) {|a|
          if look_up(a) != nil
            Value_handler.new(a)
          end
        }
      end

      rule :datatype do
        match(:bool)
        match(Float) {|a| Ffloat.new(a)}
        #match(Ffloat)
        match(Integer) {|a| Iinteger.new(a)}
        #match(Iinteger)
        match(String) {|a| Sstring.new(a)}
        #match(Sstring)
      end

      rule :bool do
        match('true') {Bbool.new(true)}
        match('false') {Bbool.new(false)}
      end
    end
  end

  def done(str)
    ["quit","exit","bye",""].include?(str.chomp)
  end

  def parse(str)
    puts "=> #{@langParser.parse str}"
  end

  def file_parse(filename)
    result = Array.new
    file = File.read(filename)
    result = @langParser.parse(file)
    result.eval
  end

  def log(state = true)
    if state
      @langParser.logger.level = Logger::DEBUG
    else
      @langParser.logger.level = Logger::WARN
    end
  end
end
