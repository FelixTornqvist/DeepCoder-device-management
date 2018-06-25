import sys
import json
import xml.etree.ElementTree as ET

def collect_tags(tags, tag):
  if tag not in tags:
    tags.append(tag)

def traverse_children(root, tab):
  tags = []
  for child in root:
    collect_tags(tags, child.tag)

  json = ''
  for tag in tags:
    json += '"' + tag + '":'
    json += generate_object(root, tag, tab) + ','
  return json

def generate_properties(root, node, tab):
  props = ''
  nothing = True;
  found_text = False
  found_attributes = bool(node.attrib)
  found_cildren = False
  object_text = ''

  if node.text and len(node.text.strip()) > 0:
    found_text = True;
    object_text = '"' + node.text.strip() + '"'

  if len(node.attrib) > 0:
    found_attributes = True

  if len(list(node)) > 0:
    found_cildren = True

  if found_attributes or found_cildren:
    props += '{'
    if found_text:
      props += '"#text":' + object_text + ','

    for key in node.attrib:
      props += '"@' + key + '":'
      props += '"' + node.attrib.get(key) + '",'

    if found_attributes and not found_cildren:
      props = props[:-1]

    props += traverse_children(node, tab)
    if (len(traverse_children(node, tab))) > 0:
      found_cildren = True
      if props.endswith(','):
        props = props[:-1]

    props += '}'

  if found_text and not found_attributes and not found_cildren:
    props += object_text

  if not found_text and not found_attributes and not found_cildren:
    props = 'null'

  return props

def generate_object(root, tag, tab):
  properties = ''
  children = root.findall(tag)

  if len(children) > 1:
    properties += '['
    for child in children:
      c = generate_properties(root, child, tab)
      properties += c + ','

    if properties.endswith(','):
      properties = properties[:-1]
    properties += ']'

  else:
    child = root.find(tag)
    properties += generate_properties(root, child, tab)

  return properties

def traverse_root(root, tab):
  tags = []
  for child in root:
    collect_tags(tags, child.tag)

  json = '{"' + root.tag + '": {'
  for tag in tags:
    json += tab +  '"' + tag + '":'
    json += generate_object(root, tag, tab) + ','

  json = json[:-1]
  json += '}'
  json += '}'
  return json

def start(i):
  tree = ET.parse(i)
  root = tree.getroot()
  result = traverse_root(root, '')

  return json.dumps(json.loads(result), sort_keys=True, indent=2, separators=(',', ': '))