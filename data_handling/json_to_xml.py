import sys
import json
import xml.etree.ElementTree as ET
import xml.dom.minidom as Minidom

def generate_object(d, name):
  txt = ''
  inner_tags = ''
  xml = '<' + str(name)

  keys = d.keys()
  for k in d:    
    if k[0] == '#':
      txt = d.get(k)
    elif k[0] == '_' and k[1] == '_':
      k = k[1:]
      txt = d.get(k)

    elif k[0] == '@' or k[0] == '_':
      xml += ' ' + k[1:] + '="' + d.get(k) +'"'

    else:
      new_dict = dict()
      new_dict[k] = d.get(k)
      inner_tags += generate_tag(new_dict)

  if len(txt) > 0 or len(inner_tags) > 0:
    xml += '>'
    xml += txt
    xml += inner_tags + '</' + name + '>'
  else:
    xml += ' />'

  return xml

def generate_tag(tree):
  xml = ''

  for element in tree:
    if isinstance(tree[element], dict):
      xml += generate_object(tree[element], element)
    elif isinstance(tree[element], unicode):
      xml += '<'+element+'>'+tree[element]+'</'+element+'>'
    elif isinstance(tree[element], list):
      for item in tree[element]:
        new_dict = dict()
        new_dict[element] = item
        inner_tags = generate_tag(new_dict)
        xml += inner_tags
    elif tree[element] == None:
      xml += '<' + element + '/>'

  return xml

def start(i):
  tree = json.load(open(i))
  xml_string = generate_tag(tree)
  asstring = xml_string.decode('string-escape')
  parsed = Minidom.parseString(asstring)

  return parsed.toprettyxml(indent="  ", newl="\n")