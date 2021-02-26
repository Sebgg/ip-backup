
require './rdparse.rb'
require './sprak_classes.rb'
#require "./tests.rb"
# end

@@scope = Scope.new

class Sprak

  attr_accessor :logicParser

  def initialize

    @logicParser = Parser.new("sprak") do

      token((/\" \.+ \"/)) {|x|x.to_s}
      token(/\s+/)                 # ta bort whitespace
      token(/\w+/)    {|x|x.to_s}  # matchar ord
      token(/\d+/)    {|x|x.to_s}  # matchar nummer
      token(/\d+\.\d+/)   {|x|x.to_s}
      token(/\=\=/)   {|x|x}       # matcha operatorer
      token(/\!\=/)   {|x|x}
      token(/\>\=/)   {|x|x}
      token(/\<\=/)   {|x|x}
      token(/\/\/.+$/)
      token(/./)      {|x|x}       # matchar övrigt


      start :program do
        match(:stmt_list)
      end

      rule :stmt_list do
        match(:stmt,:stmt_list){|stmt, stmt_list| StmtList.new(stmt,stmt_list)}
        match(:stmt)
      end


      rule :stmt do
        match(:io_stmt)
        match(:return_stmt)
        match(:condition_stmt)
        match(:iter_stmt)
        match(:func_stmt)
        match(:assign_stmt)
        match(:expr)
      end



# ================================== I/O ===================================

      rule :io_stmt do
        match :print_stmt
        match :read_stmt
      end

      rule :print_stmt do
        match('print', :expr)   {|_,a| Print.new(a)}
        match('println', :expr) {|_,a| Println.new(a)}
      end



# =============================== ITERATION ================================

      rule :iter_stmt do
          match(:for_stmt)
          match(:while_stmt)
      end

      rule :for_stmt do
        match('for', :assign_stmt,';', :or_expr,';', :assign_stmt, '{', :stmt_list, '}') {|_,a,_,b,_,c,_,d,_| For_stmt.new(a,b,c,d)}
      end

      rule :while_stmt do
          match('while',:or_expr, '{', :stmt_list, '}' ) {|_,a,_,b,_| While_stmt.new(a,b)}
      end



# =============================== FUNCTIONS ================================

      rule :func_stmt do
        match(:func_def)
      end

      rule :func_call do
        match(:func_id,"(",:args,")") {|a,_,b,_|Func_call_stmt.new(a,b)}
        match(:func_id,'(',')')       {|a,_,_  |Func_call_stmt.new(a)}
      end

      rule :args do
        match(:arg)            {|arg| [arg]}
        match(:args,',', :arg) {|args,_,arg| args + [arg]}
      end

      rule :arg do
        match(:expr)
      end



      rule :func_def do
        match('func', :func_id,'(',:params,')','{',:stmt_list,'}'){
            |_,a,_,x,_,_,b,_|
            Func_def_stmt.new(a,x,b)
        }
        match('func', :func_id,'(',')',"{",:stmt_list,"}"){
            |_,a,_,_,_,b,_  |
            Func_def_stmt.new(a,b)}
      end

      rule :params do
        match(:param)             {|param| [param]}
        match(:params,',',:param) {|params,_,param| params + [param]}
      end

      rule :param do
        match(/[A-Za-z]+/)
      end

      rule :func_id do
        match(/[A-Za-z]+/)
      end

      rule :return_stmt do
        match('return',:expr) {|_,a| Return_stmt.new(a)}
      end


# =============================== ASSIGNMENT ================================

      rule :assign_stmt do
        match(:var,"=", :expr )  {|a,_,b| Assign_stmt.new(a,b)}
      end



# =============================== IF STATEMENTS ================================

      rule :condition_stmt do
        match(:elif_stmt)
        match(:if_stmt)
        match(:else_stmt)
      end

      rule :if_stmt do
        match('if', :expr, '{', :stmt_list,'}') {|_,a,_,b,_| If.new(a, b)}
      end

      rule :elif_stmt do
        match('elif', :expr, '{', :stmt_list,'}') {|_,a,_,b,_| Elif.new(a, b)}
      end

      rule :else_stmt do
        match('else','{','}')
        match('else','{',:stmt_list,'}')   {|_,_,b,_|Else.new(b)}

      end




      rule :expr do
        match(:or_expr)
      end

# =============================== AND/OR ================================

        rule :or_expr do
          match(:or_expr, 'or', :and_expr) { |a,_,b| Or_expr.new(a,b) }
          match(:and_expr)
        end

        rule :and_expr do
        match(:and_expr, 'and', :and_expr) { |a,_,b| And_expr.new(a,b) }
        match(:not_expr)
        end

        rule :not_expr do
        match('not', :not_expr) { |_,b| Not_expr.new(b) }
        match(:comp_expr)
        end



# =============================== LOGIC ================================

      rule :comp_expr do
        match(:add_expr, :logic_comp, :add_expr) {|a,x,b| Comp_expr.new(a,x,b)}
        match(:add_expr)
      end

      rule :logic_comp do
        match('==')
        match('<=')
        match('>=')
        match('>')
        match('<')
        match('!=')
      end



# =============================== MATH ================================

      rule :add_expr do
        match(:add_expr, '+', :mult_expr) { |a,x,b| Add_expr.new(a,x,b) }
        match(:add_expr, '-', :mult_expr) { |a,x,b| Add_expr.new(a,x,b) }
        match(:mult_expr)
      end

      rule :mult_expr do
        match(:mult_expr, '*', :mult_expr) { |a,x,b| Mult_expr.new(a,x,b) }
        match(:mult_expr, '/', :mult_expr) { |a,x,b| Mult_expr.new(a,x,b) }
        match(:variables)
      end

      rule :variables do
        match(:func_call)
        match(:digit)
        match(:string)
        match(:boolean)
        match(:var)
      end

      rule :digit do
        match(/\d+/)  {|a| Digit.new(a)}
      end

      rule :string do
        match(/"/, /.+/, /"/) {|_,x,_| String.new(x)}
      end

      rule :boolean do
        match('true') {|a|Boolean.new(true)}
        match('false'){|a|Boolean.new(false)}
      end

      rule :var do
        match(/[A-Za-z]+/) {|a| Get_Var_stmt.new(a)}
      end


    end

    @logicParser.logger.level = Logger::WARN
  end

  def parse(code)
    print "]]] Sprak >>> \n"
    result = @logicParser.parse(code)
    result.eval
  end

  def log(state = true)
    if state
      @logicParser.logger.level = Logger::DEBUG
    else
      @logicParser.logger.level = Logger::WARN
    end
  end

end




if ARGV.length == 1
  code = ARGV[0].to_str
  rd = Sprak.new
  code = File.read(code)
  rd.parse(code)
else
  puts "ERROR: Missing input parameters"
end
