#!/usr/bin/env python3
#
# Add license headers to the codebase
#
# (c) Justus Languell 2022

import os, datetime

name = 'Justus Languell'
license_email = 'jus@gtsbr.org'
project = 'Capstone'
put_license_before = '#include'

def get_project_year(project):
    return '2021-' + str(datetime.date.today().year)

def make_jdoc(text):
    comment = '/**\n'
    for line in text.split('\n'):
        comment += ' * ' + line + '\n'
    return comment + ' */'

def get_license():
    return f'''Copyright (C) {get_project_year(project)} {name}
This file is part of {project}, which is licensed under the MIT license.
For more details, see ./license.txt or write <{license_email}>.'''

def extension(path):
    return os.path.splitext(path)[-1]

def insert(put_before, source):
            return make_jdoc(get_license()) + '\n' + put_before + put_before.join(source.split(put_before)[1:])


def with_license(path, source):
    if extension(path).endswith('cc'):
        return insert('#include', source)
    elif extension(path).endswith('h'):
        return insert('#ifndef', source)
    else:
        return source

def validate(path):
    if extension(path).endswith('cc') or extension(path).endswith('h'):
        source = open(path, 'r').read()
        source = with_license(path, source)
        open(path, 'w').write(source)

if __name__ == '__main__':
    for root, subdirs, files in os.walk(os.path.realpath('./src/')):
        [validate(root + '/' + file) for file in files]

        