#!/usr/bin/env python3
#
# Generate code for abstract syntax trees
#
# (c) Justus Languell 2022

type_map = {
    'node': 'Node*',
    'token': 'int',
    'string': 'std::string'
}

class Element:
    def __init__(self, name, type):
        self.name = name
        self.type = type_map[type.replace(',', '')]
    def __str__(self):
        return f'{self.name}:{self.type}'

class Node:
    def __init__(self, name):
        self.name = name
        self.elements = []
    def add(self, element, type):
        self.elements.append(Element(element, type))
    def __str__(self):
        return f"{self.name}({', '.join(str(e) for e in self.elements)})"
    def params(self):
        params = []
        for element in self.elements:
            if element.type == type_map['string']:
                params.append(f'const {element.type}& {element.name}')
            else:
                params.append(f'{element.type} {element.name}')
        return ', '.join(params)
    def initialization(self):
        return ', '.join([f'{e.name}({e.name})' for e in self.elements])
    def group(self):
        groups = {}
        for element in self.elements:
            if element.type in groups.keys():
                groups[element.type].append(element.name)
            else:
                groups[element.type] = [element.name]
        return groups
    def fields(self):
        body = ''
        #for type, fields in self.group().items():
        #   body += f'{type} {", ".join(fields)};\n    '
        for element in self.elements:
            body += f'{element.type} {element.name};\n    '
        return body
    def header(self):
        return f'''class {self.name} : public Node {{
  public:              
    {self.fields()} 
    {self.name}({self.params()}) : {self.initialization()} {{}}
    std::string toJSON(void);
}};
'''
    def implementation(self):
        json = f'"{{\\"_type\\": \\"{self.name}\\",'
        for element in self.elements:
            if element.type == type_map['string']:
                json += f'\\"{element.name}\\":" + {element.name} + ",'
            elif element.type == type_map['node']:
                json += f'\\"{element.name}\\": " + {element.name}->toJSON() + ",'
            elif element.type == type_map['token']:
                json += f'\\"{element.name}\\":" + Lexer::getTokenStr({element.name}) + ",'
                
        return f'''std::string {self.name}::toJSON(void) {{
    return {json}}}";
}}'''
        
if __name__ == '__main__':
    path = './src/ast.template'
    source = open(path, 'r').read()
    nodes = []
    for part in source.split('}')[:-1]:
        halfs = part.split('{')
        node = Node(halfs[0].strip())
        for element in halfs[-1].split('\n'):
            if element.strip() == '':
                continue
            line = element.split(':')
            node.add(line[0].strip(), line[-1].strip())
        nodes.append(node)
            
    decls = '\n'.join([node.header() for node in nodes])
            
    header = f'''#ifndef BOUNCE_AST
#define BOUNCE_AST

#include "common.h"
#include "lexer.h"

class Node {{
  public:
    virtual std::string toJSON() = 0;
}};

{decls}
#endif
'''
    print(header)
    
    impl = '#include "ast.h"\n\n' + '\n'.join([node.implementation() for node in nodes])
    
    open('./src/ast.h', 'w').write(header)
    open('./src/ast.cc', 'w').write(impl)