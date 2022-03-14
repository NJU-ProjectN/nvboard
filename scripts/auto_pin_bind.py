import sys
import os

class IndentWriter():
  def __init__(self):
    self.fp = None
    self.indent_level = 0
    self.newline = True
  
  def open(self, filename):
    self.fp = open(filename, 'w')
    self.indent_level = 0
    self.newline = True
  
  def close(self):
    self.fp.close()
    self.fp = None

  # TODO:: Add '{', '}' in comment detection
  def writeline(self, wrline):
    if len(wrline) == 0:
      return
    
    if '{' in wrline:
      indclose = wrline[:wrline.find('{')].count('}')
    else:
      indclose = wrline.count('}')
    
    if indclose > self.indent_level:
      print("Internal Error: Indent level closed beyond 0")
      exit(-1)
    
    if self.newline:
      self.fp.write('\t'*(self.indent_level-indclose) )
    
    self.fp.write(wrline)
    
    self.indent_level += wrline.count('{') - wrline.count('}')
    self.newline = wrline[-1] == '\n'
  
  def write(self, wrstr):
    wrlines = wrstr.split('\n')
    for line in wrlines[:-1]:
      self.writeline(line+'\n')
    self.writeline(wrlines[-1])



def inout_dict(board_file_path):
  inout = {}
  with open(board_file_path, "r") as f:
    for i, line in enumerate(f):    
      # Remove comment
      if '#' in line:
        line = line[:line.find('#')]
      line = line.strip()
      
      # Skip empty lines
      if line == '':
        continue
      
      lineseg = line.split()
      if len(lineseg) == 2:
        direction = lineseg[0].strip()
        pinname = lineseg[1].strip()
        
        if direction.startswith('rt_'):
          is_real_time_str = 'true'
          direction = direction[3:]
        else:
          is_real_time_str = 'false'
        
        if direction == "input":
          is_output_str = 'false'
        elif direction == "output":
          is_output_str = 'true'
        else:
          print(f"Board Line {i}: Error: Invalid pin direction \"{direction}\"")
          exit(-1)
        
        inout[pinname] = (is_output_str, is_real_time_str)
        
      else:
        print(f"Board Line {i}: Error: Invalid line \"{line}\"")
        exit(-1)

  return inout

def check_pin_valid(inout, pin):
  if not pin in inout:
    print(f"Error: Invalid pin {pin}")
    exit(1)
    
def bind_pin(wr, inout, signal, pin):
  check_pin_valid(inout, pin)
  pinDesc = inout[pin]
  wr.write(f"nvboard_bind_pin( &top->{signal}, {pinDesc[1]}, {pinDesc[0]}, 1, {pin});\n")

def bind_vec_pins(wr, inout, signal, pins):
  for pin in pins:
    check_pin_valid(inout, pin)
  pinDesc = inout[pins[0]]
  wr.write(f"nvboard_bind_pin( &top->{signal}, {pinDesc[1]}, {pinDesc[0]}, {len(pins)}")
  for pin in pins:
    wr.write(f", {pin}")
  wr.write(");\n")

def init_info(wr, top):
  wr.write( (
  "#include <nvboard.h>\n"
  f'#include "V{top}.h"\n'
  "\n"
  f"void nvboard_bind_all_pins(V{top}* top) {{\n"
  ) )


def print_usage():
  print("Usage: python3 auto_pin_bind.py <.nxdc constraint file path> <auto_bind.c output file path>")

if __name__ == "__main__":
  # Parse board descriptor for pin list
  nvboard_path = os.environ.get('NVBOARD_HOME')
  inout = inout_dict(nvboard_path + "/board/N4")
  
  if len(sys.argv) != 3:
    print("Error: Bad command line arguments")
    print_usage()
    exit(-1)
  
  cons_path = sys.argv[1]
  output_path = sys.argv[2]
  
  cons_f = open(cons_path, "r")
  bind_wr = IndentWriter()
  bind_wr.open(output_path)

  # Parse toplevel module name
  topline = cons_f.readline()
  sptopline = topline.split('=')
  if not '=' in topline or sptopline[0].strip() != 'top' or len(sptopline) != 2:
    print("Line 1: Error: Invalid top module specification")
    print("        Usage: in line 1: top=module_name")
    exit(1)
  
  top = sptopline[1].strip(' \n')

  init_info(bind_wr, top)

  for i, line in enumerate(cons_f):
    # Remove comment
    if '#' in line:
      line = line[:line.find('#')]
    line = line.strip()

    # Skip empty lines
    if line == '':
      continue
    
    # Multi-pin assignment
    if ' (' in line or ')' in line:
      # Stupid method to check missing bracelet
      if ' (' in line and line.endswith(')'):
        signal = line[:line.find('(')].strip()
        pins = [u.strip() for u in line[line.find('(')+1:-1].split(',') if u.strip() != '']
        
        bind_vec_pins(bind_wr, inout, signal, pins)
      else:
        print(f"Line {i+1}: Error: Invalid multi-pin assignment")
        exit(1)
    # Single pin assignment
    elif ' ' in line:
      spline = line.split(' ')
      if len(spline) == 2:
        signal = spline[0].strip()
        pin = spline[1].strip()

        bind_pin(bind_wr, inout, signal, pin)
      else:
        print(f"Line {i+1}: Error: Invalid line")
        exit(1)

    # Single word line, ignore to comply with previous version

  bind_wr.write("}\n")
  bind_wr.close()
