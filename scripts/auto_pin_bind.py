#!/usr/bin/env python3
import sys
import os

class BoardDescParser():
  def __init__(self):
      self.pins = {}
  
  def parseLine(self, lid, lineseg):
    direction = lineseg[0].strip()
    pinname = lineseg[1].strip()
    
    if direction != "input" and direction != "output":
      print(f"Board Line {lid}: Error: Invalid pin direction \"{direction}\"")
      exit(-1)
    
    self.pins[pinname] = 1
  
  def parseFile(self, path):
    self.pins = {}
    with open(path, "r") as f:
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
          self.parseLine(i, lineseg)
        else:
          print(f"Board Line {i}: Error: Invalid line \"{line}\"")
          exit(-1)
  
  def checkPinValid(self, pin):
    return pin in self.pins


class NxdcParser():
  def __init__(self) -> None:
      self.toplevel = ""
      self.binds = []
  
  def parseToplevel(self, topline):
    sptopline = topline.split('=')
    if not '=' in topline or sptopline[0].strip() != 'top' or len(sptopline) != 2:
      print("Line 1: Error: Invalid top module specification")
      print("        Usage: in line 1: top=module_name")
      exit(1)
    
    self.toplevel = sptopline[1].strip(' \n')
  
  def parseVec(self, lid, line):
    # Stupid method to check missing bracelet
    if ' (' in line and line.endswith(')'):
      signal = line[:line.find('(')].strip()
      pins = [u.strip() for u in line[line.find('(')+1:-1].split(',') if u.strip() != '']
      
      self.binds.append( (signal, pins) )
    else:
      print(f"Line {lid+1}: Error: Invalid multi-pin assignment")
      exit(1)
  
  def parsePin(self, lid, line):
    spline = line.split(' ')
    if len(spline) == 2:
      signal = spline[0].strip()
      pin = spline[1].strip()
      
      self.binds.append( (signal, pin) )
    else:
      print(f"Line {lid+1}: Error: Invalid line")
      exit(1)
    
  def parseFile(self, path):
    cons_f = open(path, "r")
    
    self.parseToplevel(cons_f.readline())
    
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
        self.parseVec(i, line)
      # Single pin assignment
      elif ' ' in line:
        self.parsePin(i, line)
      else:
      # Single word line, ignore to comply with previous version
        pass


class IndentWriter():
  def __init__(self):
    self.fp = None
    self.indent_level = 0
    self.newline = True
    self.inside_comment = False
  
  def open(self, filename):
    self.fp = open(filename, 'w')
    self.indent_level = 0
    self.newline = True
  
  def close(self):
    self.fp.close()
    self.fp = None

  def writeline(self, wrline):
    if len(wrline) == 0:
      return
    
    if '//' in wrline:
      # Check indent before comment and proceed
      cmtInd = wrline.find('//')
      self.writeline(wrline[:cmtInd])
      self.inside_comment = True
      wrline = wrline[cmtInd:]
    
    if not self.inside_comment:
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
    
    if not self.inside_comment:
      self.indent_level += wrline.count('{') - wrline.count('}')
    
    self.newline = wrline[-1] == '\n'
    if self.newline:
      self.inside_comment = False
  
  def write(self, wrstr):
    wrlines = wrstr.split('\n')
    for line in wrlines[:-1]:
      self.writeline(line+'\n')
    self.writeline(wrlines[-1])


class AutoBindWriter():
  def __init__(self, iwriter, board):
    self.iw = iwriter
    self.board = board
  
  def bindPin(self, signal, pin):
    if not self.board.checkPinValid(pin):
      print(f"Error: Invalid pin {pin}")
      exit(1)
    self.iw.write(f"nvboard_bind_pin( &top->{signal}, 1, {pin});\n")
  
  def bindVec(self, signal, pins):
    for pin in pins:
      if not self.board.checkPinValid(pin):
        print(f"Error: Invalid pin {pin}")
        exit(1)
    self.iw.write(f"nvboard_bind_pin( &top->{signal}, {len(pins)}")
    for pin in pins:
      self.iw.write(f", {pin}")
    self.iw.write(");\n")
  
  def bind(self, signal, pin):
    if type(pin) is list:
      self.bindVec(signal, pin)
    elif type(pin) is str:
      self.bindPin(signal, pin)
    else:
      print(f"Error: Invalid bind from {signal} to {pin}")
      exit(-1)

  def writeHead(self, top):
    self.iw.write( (
    "#include <nvboard.h>\n"
    f'#include "V{top}.h"\n'
    "\n"
    f"void nvboard_bind_all_pins(V{top}* top) {{\n"
    ) )
  
  def writeTail(self):
    self.iw.write("}\n")

def print_usage():
  print("Usage: python3 auto_pin_bind.py nxdc_constraint_file_path auto_bind_c_output_file_path")

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Error: Bad command line arguments")
    print_usage()
    exit(-1)
  
  nvboard_path = os.environ.get('NVBOARD_HOME')
  if nvboard_path is None:
    print("Error: NVBOARD_HOME is not set")
    exit(-1)
  
  cons_path = sys.argv[1]
  output_path = sys.argv[2]
  boardfile_path = os.path.join(nvboard_path, "board/N4")

  if not os.path.exists(cons_path):
    print(f"Error: Constraint file doesn't exist:")
    print(f"       {cons_path}")
    exit(-1)
  
  if not os.path.exists(boardfile_path):
    print(f"Error: Board file doesn't exist:")
    print(f"       {boardfile_path}")
    exit(-1)
  
  # Parse board descriptor for pin list
  board = BoardDescParser()
  board.parseFile(boardfile_path)
  
  # Parse nxdc constraint file
  constr = NxdcParser()
  constr.parseFile(cons_path)
  # Write bind file with indent
  bind_wr = IndentWriter()
  bind_wr.open(output_path)
  
  abw = AutoBindWriter(bind_wr, board)
  
  abw.writeHead(constr.toplevel)
  for signal, pin in constr.binds:
    abw.bind(signal, pin)
  abw.writeTail()
  
  bind_wr.close()
