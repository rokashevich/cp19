import sys

if len(sys.argv) != 4:
    print('Args: char_array_name input_file output_file')
    sys.exit(1)

char_array_name = sys.argv[1]
input_file = sys.argv[2]
output_file = sys.argv[3]

with open(output_file, 'w') as f:
    f.write('#pragma once\n')
    f.write('const char '+char_array_name+'[] = R"(')
    f.write(open(input_file).read())
    f.write(')";')
