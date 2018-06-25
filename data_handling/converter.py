import sys
import json_to_xml
import xml_to_json
import time

def convert(file, desired_output):
  toJson = -1;
  arg2 = desired_output.upper()
  if arg2 == "JSON":
      toJson = 1;
  elif arg2 == "XML":
      toJson = 0;
  else:
      print "second argument should be either 'JSON' or 'XML'"
      quit();

  result = ''
  if not toJson and file.lower().endswith('.json'):
      result = json_to_xml.start(file)
  elif toJson and file.lower().endswith('.xml'):
      result = xml_to_json.start(file)
  else:
      with open(file, 'r') as fin:
          print fin.read()

  output = open('output.txt', 'w')
  output.write(result)
  output.close()

if __name__ == '__main__':
  import sys
  convert(sys.argv[1], sys.argv[2])
