import sys
import re
import os

def inout_dict(file_path):
  f = open(file_path, "r")
  inout = {}
  line = f.readline()
  while line:
    line = line.strip(' \n')
    line = line.split(' ')
    if len(line) == 2:
      direction = line[0].replace(' ', '')
      is_real_time_str = 'false'
      is_output_str = ''
      if direction.startswith('rt_'):
        is_real_time_str = 'true'
        direction = direction[3:]
      if direction == "input":
        is_output_str = 'false'
      elif direction == "output":
        is_output_str = 'true'
      else:
        print("invalid direction")
        exit(-1)
      inout[line[1].replace(' ', '')] = (is_output_str, is_real_time_str)
    line = f.readline()
  return inout


def write_line_dep(f, output_str):
  global dep, start_of_line
  if(len(output_str) == 0):
    return
  if(output_str == "\n"):
    start_of_line = True
    f.write('\n')
    return

  dep = dep - output_str.count('}')
  if start_of_line:
    for i in range(dep):
      f.write('\t')
  f.write(output_str)

  start_of_line = output_str[-1] == '\n'

  dep = dep + output_str.count('{')
  if dep < 0:
    print("invalid syntax")
    exit(-1)


def write_dep(f, output_str):
  lines = re.split('(\n)', output_str)
  for line in lines:
    write_line_dep(f, line)


def check_pin_valid(inout, pin):
  if inout.get(pin, -1) == -1:
    print("invalid pin " + pin)
    exit(1)


def bind_pin(f, inout, signal, pin):
  pin = pin.replace(' ', '')
  check_pin_valid(inout, pin)
  signal_addr = "&top->" + signal
  strs = inout[pin]
  write_dep(bind_f, "nvboard_bind_pin(" + signal_addr + ", " + strs[1] + ", " + strs[0] + ", 1, " + pin + ");\n")


def bind_vec_pins(f, inout, signal, pins):
  for idx in range(len(pins)):
    pins[idx] = pins[idx].replace(' ', '')
    check_pin_valid(inout, pins[idx])
  signal_addr = "&top->" + signal
  strs = inout[pins[0]]
  write_dep(bind_f, "nvboard_bind_pin(" + signal_addr + ", "  + strs[1] + ", " + strs[0] + ", " + str(len(pins)))
  for idx in range(len(pins)):
    write_dep(bind_f, ", " + pins[idx])
  write_dep(bind_f, ");\n")


def init_info(f, top):
  write_dep(bind_f, "#include <nvboard.h>\n#include \"V" + top + ".h\"\n\nvoid nvboard_bind_all_pins(V" + top + "* top){\n")

dep = 0
start_of_line = True

nvboard_path = os.environ.get('NVBOARD_HOME')
inout = inout_dict(nvboard_path + "/board/N4")

if len(sys.argv) <= 1:
  print("constrait file missing!")
  exit(-1)
cons_path = sys.argv[1]

if len(sys.argv) <= 2:
  print("output file missing!")
  exit(-1)
output_path = sys.argv[2]


cons_f = open(cons_path, "r")
bind_f = open(output_path, "w")

line = cons_f.readline()
line = line.split('=')
if not line or len(line) <= 1:
  print("top module should be specified at the top of constraint file, usage: top=top_name")
  exit(-1)

top = line[1].strip(' \n')

init_info(bind_f, top)

line = cons_f.readline()
while line:
  line = line.strip(')\n')
  line = line.split('(')

  if(len(line) == 1):
    line = line[0].split(' ')
  if len(line) == 1:
    line = cons_f.readline()
    continue
  signal = line[0].strip(' ')
  pins = line[1].split(',')

  if len(pins) == 1:
    bind_pin(bind_f, inout, signal, pins[0])
  else:
    bind_vec_pins(bind_f, inout, signal, pins)

  line = cons_f.readline()

write_dep(bind_f, "}")
