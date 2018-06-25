import sys
from timeit import Timer
import converter

file = sys.argv[1]
desired_format = sys.argv[2]
expected_output = sys.argv[3]

t = Timer(lambda: converter.convert(file, desired_format))
execution_time = t.timeit(number = 1)

with open('output.txt', 'r') as out:
  result = out.read()
with open(expected_output, 'r') as exp:
  expected = exp.read()

if result == expected:
  print execution_time
else:
  print 'The test failed!' 