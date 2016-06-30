define sigcontattach
  attach $arg0
  break debug_break
  shell kill -s SIGCONT $arg0
  continue
  next 2
end
