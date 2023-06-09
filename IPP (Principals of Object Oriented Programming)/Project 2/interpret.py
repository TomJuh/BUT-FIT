"""
IPP project: interpret.py
author : xjuhas04
"""

import sys
import re
import argparse
from collections import deque
import xml.etree.ElementTree as ET

# global stack of variables
var_stack = deque()

"""
Definition of classes
"""


class Argument:
    def __init__(self, arg_type, value, order):
        self.type = arg_type
        self.value = value
        self.order = order


class Instruction:
    def __init__(self, name, args, order):
        self.name = name
        self.args = args
        self.order = order


class Frames:
    def __init__(self):
        self.GF = {}
        self.LF = deque({})
        self.TF = None


class Variable:
    def __init__(self, var_type, value):
        self.type = var_type
        self.value = value


"""
gets char from index of string
"""


def char_at_index(instruction, frames, arg_index):
    if instruction.args[arg_index].type == 'var' and check_var_type(instruction.args[arg_index], frames, 'int'):
        val = int(parse_symb(instruction.args[arg_index].value.split('@'), frames).value)
        if instruction.args[1].value[val]:
            return val
        else:
            exit(58)
    else:
        exit(500)


"""
checks if var exists in frames
"""


def check_var_exists(frames, val):
    frame = val.split('@')[0]
    var = val.split('@')[1]
    if frame == 'TF' and frames.TF is None:
        exit(55)
    return (frame == 'GF' and var in frames.GF) \
           or (frame == 'LF' and var in frames.LF) \
           or (frame == 'TF' and var in frames.TF)


"""
Checks if correct type is used
"""


def check_var_type(var, frames, var_type):
    cur_frame = var.split('@')[0]
    index = var.split('@')[1]
    if cur_frame == 'GF':
        return frames.GF[index].type == var_type
    elif cur_frame == 'LF':
        return frames.LF[-1][index].type == var_type
    elif cur_frame == 'TF':
        if frames.TF is None:
            exit(55)
        return frames.TF[index].type == var_type


"""
creates new variable from var of specific frame
"""


def parse_symb(var, frames):
    if var[0] == 'GF':
        if not frames.GF:
            exit(55)
        return Variable(frames.GF[var[1]].type, frames.GF[var[1]].value)
    elif var[0] == 'LF':
        if not frames.LF:
            exit(55)
        return Variable(frames.LF[-1][var[1]].type, frames.LF[-1][var[1]].value)
    elif var[0] == 'TF':
        if frames.TF is None:
            exit(55)
        return Variable(frames.TF[var[1]].type, frames.TF[var[1]].value)


"""
Writes value to var in specific frame
"""


def assign_to_var(var, frames, var_type, var_value):
    if var[0] == 'GF':
        frames.GF[var[1]].type = var_type
        frames.GF[var[1]].value = var_value
    elif var[0] == 'LF':
        frames.LF[-1][var[1]].type = var_type
        frames.LF[-1][var[1]].value = var_value
    elif var[0] == 'TF':
        if frames.TF is None:
            exit(55)
        frames.TF[var[1]].type = var_type
        frames.TF[var[1]].value = var_value


"""
parses value from symb (for instruction with two symb args)
"""


def pars_val_args(instruction, frames, args_type):
    first_val = 0
    second_val = 0
    if instruction.args[1].type == args_type:
        first_val = instruction.args[1].value
    elif instruction.args[1].type == 'var':
        argument_var = parse_symb(instruction.args[1].value.split('@'), frames)
        if argument_var.type != args_type:
            exit(53)

        first_val = argument_var.value
    else:
        exit(53)

    if instruction.args[2].type == args_type:
        second_val = instruction.args[2].value
    elif instruction.args[2].type == 'var':
        argument_var = parse_symb(instruction.args[2].value.split('@'), frames)
        if argument_var.type != args_type:
            exit(53)

        second_val = argument_var.value
    else:
        exit(53)  # incorrect types
    return [first_val, second_val]


"""
checks if two symb are equal
"""


def check_eq_symb(instruction, frames):
    if instruction.args[1].type == 'var':
        first_var = parse_symb(instruction.args[1].value.split('@'), frames)
        first_val = first_var.value
        first_type = first_var.type
    else:
        first_type = instruction.args[1].type
        first_val = instruction.args[1].value
    if instruction.args[2].type == 'var':
        second_var = parse_symb(instruction.args[2].value.split('@'), frames)
        second_val = second_var.value
        second_type = second_var.type
    else:
        second_type = instruction.args[2].type
        second_val = instruction.args[2].value
    if first_type == second_type or (first_type == 'nil' or second_type == 'nil'):
        return correct_type(first_val, first_type) == correct_type(second_val, second_type)
    else:
        exit(53)


"""
Checks if str is correct int format
"""


def operands_type(first_operand, second_operand, op_type):
    first_operand = str(first_operand)
    second_operand = str(second_operand)
    if op_type == 'int':
        return (first_operand.isdigit() or (first_operand.startswith('-') and first_operand[1:].isdigit())) \
               and (second_operand.isdigit() or (second_operand.startswith('-') and second_operand[1:].isdigit()))


"""
Fixes values based on types in var
"""


def correct_type(val, arg_type):
    if arg_type == 'string':
        val = str(val)
    elif arg_type == 'int':
        val = int(val)
    elif arg_type == 'bool':
        val = bool(val)
    return val


"""
gets values from symb in specific type
"""


def pars_val_symb(instruction, frames, symb_type):
    if symb_type == 'string':
        vals = pars_val_args(instruction, frames, 'string')
        return [str(vals[0]), str(vals[1])]
    elif symb_type == 'int':
        vals = pars_val_args(instruction, frames, 'int')
        return [int(vals[0]), int(vals[1])]
    elif symb_type == 'bool':
        vals = pars_val_args(instruction, frames, 'bool')
        return [bool(vals[0]), bool(vals[1])]


"""
MOVE
writes value to var
"""


def move_function(instruction, frames):
    if not check_var_exists(frames, instruction.args[0].value):
        exit(54)
    frame = instruction.args[0].value.split('@', 1)[0]
    var = instruction.args[0].value.split('@', 1)[1]

    if instruction.args[1].type == 'var':
        variable = parse_symb(instruction.args[1].value.split('@', 1), frames)
    else:
        variable = Variable(instruction.args[1].type, instruction.args[1].value)
    if frame == 'GF':
        frames.GF[var] = variable
    elif frame == 'LF':
        frames.LF[-1][var] = variable
    elif frame == 'TF':
        if frames.TF is None:
            exit(55)
        frames.TF[var] = variable


"""
FRAMES function
createframe just defines TF
push and pop uses stack in frames.LF
"""


def createframe_function(frames):
    frames.TF = {}


def pushframe_function(frames):
    if frames.TF is None:
        exit(55)
    frames.LF.append(frames.TF)
    frames.TF = None


def popframe_function(frames):
    if frames.LF:
        frames.TF = frames.LF.pop()
    else:
        exit(55)


"""
DEFVAR
creates and writes new variable for each frame
"""


def defvar_function(instruction, frames):
    check_var_exists(frames, instruction.args[0].value)
    var = instruction.args[0].value.split('@')

    if var[0] == 'GF':
        frames.GF[var[1]] = Variable('', '')
    elif var[0] == 'LF':
        frames.LF[-1][var[1]] = Variable('', '')
    elif var[0] == 'TF':
        if frames.TF is None:
            exit(55)
        frames.TF[var[1]] = Variable('', '')


"""
CALL
Jumps to label
"""


def call_function(instruction, instruction_list, frames, labels, input_file):
    if instruction.args[0].value in labels:
        interpret_instruction(instruction_list, frames, int(labels[instruction.args[0].value]), labels, input_file)


"""
RETURN
return to instruction index from call stack (in actuality one index after the call instruction)
"""


def return_function(instruction_list, frames, labels, input_files):
    if labels['call']:
        interpret_instruction(instruction_list, frames, int(labels['call'][-1]) + 1, labels, input_files)


"""
PUSHS
creates and pushes new variable to global var stack
"""


def pushs_function(instruction, frames):
    if instruction.args[0].type == 'var':
        var = parse_symb(instruction.args[0].value.split('@'), frames)
        var.value += '@' + var.type
        var_stack.append(var.value)
    else:
        var_stack.append(instruction.args[0].value + '@' + instruction.args[0].type)


"""
POPS
pops var from global var stack
"""


def pops_function(instruction, frames):
    if var_stack:
        var = var_stack.pop().split('@')
        assign_to_var(instruction.args[0].value.split('@'), frames, var[1], var[0])
    else:
        exit(56)


"""
ARITHMETICS functions
parses arg values and saves result
in elif instruction.name == of idiv makes sure 0 isn't used
"""


def add_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'int')
    if not operands_type(values[0], values[1], 'int'):
        exit(53)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'int', int(values[0]) + int(values[1]))


def sub_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'int')
    if not operands_type(values[0], values[1], 'int'):
        exit(53)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'int', int(values[0]) - int(values[1]))


def mul_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'int')
    if not operands_type(values[0], values[1], 'int'):
        exit(53)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'int', int(values[0]) * int(values[1]))


def idiv_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'int')
    if int(values[0]) == 0 or int(values[1]) == 0:
        exit(57)

    if not operands_type(values[0], values[1], 'int'):
        exit(53)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'int', int(values[0]) / int(values[1]))


"""
COMPARE functions
parses values from arguments and saves result
in elif instruction.name == of eq checks for nil values
"""


def lt_function(instruction, frames):
    values = pars_val_symb(instruction, frames, instruction.args[0].type)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', values[0] < values[1])


def gt_function(instruction, frames):
    values = pars_val_symb(instruction, frames, instruction.args[0].type)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', values[0] > values[1])


def eq_function(instruction, frames):
    values = []
    if instruction.args[0].type == 'nil':
        values[0] = 'nil'
        values[1] = instruction.args[1].value
    elif instruction.args[1].type == 'nil':
        values[0] = instruction.args[0].value
        values[1] = 'nil'
    else:
        values = pars_val_symb(instruction, frames, instruction.args[0].type)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', values[0] == values[1])


"""
BOOL functions
parses entered arguments and saves result
"""


def and_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'bool')
    assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', bool(values[0]) and bool(values[1]))


def or_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'bool')
    assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', bool(values[0]) or bool(values[1]))


def not_function(instruction, frames):
    if instruction.args[1].type == 'var':
        val = bool(parse_symb(instruction.args[1].value.split('@'), frames).value)
        assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', not val)
    elif instruction.args[1].type == 'bool':
        val = bool(instruction.args[1].value)
        assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', not val)
    else:
        exit(500)


"""
INT2STR
int to char
checks correct range for char
"""


def int2str_function(instruction, frames):
    if instruction.args[1].type == 'var' and check_var_type(instruction.args[1].value, frames, 'int'):
        val = int(parse_symb(instruction.args[1].value.split('@'), frames).value)
        if 0 <= val <= 255:
            assign_to_var(instruction.args[0].value.split('@'), frames, 'string', chr(val))
        else:
            exit(58)
    else:
        exit(500)


"""
STRI2INT
char at str index to int
checks correct range of values
"""


def stri2int_function(instruction, frames):
    val = char_at_index(instruction, frames, 2)
    if 0 <= val <= 255:
        str_val = str(parse_symb(instruction.args[1].value.split('@'), frames).value)
        assign_to_var(instruction.args[0].value.split('@'), frames, 'string', int(str_val[val]))
    else:
        exit(58)


"""
READS
reads var from input
either from input file or from stdin
checks correct types
"""


def read_function(instruction, frames, input_file):
    if input_file:
        read_input = input_file.readline().strip()
    else:
        read_input = input()
    if read_input is None:
        assign_to_var(instruction.args[0].value.split('@'), frames, 'nil', 'nil')
    else:
        if instruction.args[1].value == 'int':
            if not operands_type(read_input, 0, 'int'):
                exit(53)
            assign_to_var(instruction.args[0].value.split('@'), frames, 'int', int(read_input))
        elif instruction.args[1].value == 'string':
            if not type(read_input) == str:
                exit(53)
            assign_to_var(instruction.args[0].value.split('@'), frames, 'string', str(read_input))
        elif instruction.args[1].value == 'bool':
            if read_input == 'true':
                read_input = True
            else:
                read_input = False
            assign_to_var(instruction.args[0].value.split('@'), frames, 'bool', bool(read_input))


"""
WRITE
writes symb to output
corrects format of nil and bool types
"""


def write_function(instruction, frames):
    if instruction.args[0].type == 'var':
        var = parse_symb(instruction.args[0].value.split('@'), frames)
        if var.type == 'bool':
            if var.value:
                var.value = 'true'
            else:
                var.value = 'false'
            write_val = var.value
        elif var.type == 'nil':
            var.value = ''
            write_val = var.value
        else:
            write_val = var.value
            write_val = correct_type(write_val, str(var.type))
    else:
        write_val = instruction.args[0].value
        if 'nil' != instruction.args[0].type != 'bool':
            write_val = correct_type(write_val, str(instruction.args[0].type))
    print(write_val, end='')


"""
CONCAT
connects strings from symb
"""


def concat_function(instruction, frames):
    values = pars_val_args(instruction, frames, 'string')
    assign_to_var(instruction.args[0].value.split('@'), frames, 'string', str(values[0] + values[1]))


"""
STRLEN
return numeric length of string
"""


def strlen_function(instruction, frames):
    if instruction.args[1].type == 'var':
        val = str(parse_symb(instruction.args[1].value.split('@'), frames).value)
        assign_to_var(instruction.args[0].value.split('@'), frames, 'int', len(val))
    elif instruction.args[1].type == 'string':
        val = str(instruction.args[1].value)
        assign_to_var(instruction.args[0].value.split('@'), frames, 'int', len(val))
    else:
        exit(500)


"""
GETCHAR
gets and writes char from index in symb
"""


def getchar_function(instruction, frames):
    val = char_at_index(instruction, frames, 2)
    str_val = str(parse_symb(instruction.args[1].value.split('@'), frames).value)
    assign_to_var(instruction.args[0].value.split('@'), frames, 'string', str(str_val[val]))


"""
SETCHAR
changes char of var
gets the index of var to be changed
the new char to be changed
and creates new string to replace var
"""


def setchar_function(instruction, frames):
    change_str = str(parse_symb(instruction.args[2].value.split('@'), frames).value)
    if instruction.args[1].type == 'var':
        val = char_at_index(instruction, frames, 1)
    else:
        val = instruction.args[1].value
    final_str = str(parse_symb(instruction.args[2].value.split('@'), frames).value)
    final_str = final_str[:val] + change_str[0] + final_str[val + 1:]
    assign_to_var(instruction.args[0].value.split('@'), frames, 'string', final_str)


"""
TYPE
"""


def type_function(instruction, frames):
    if instruction.args[1].type == 'var':
        var_type = parse_symb(instruction.args[1].value.split('@'), frames)
        if (not var_type.type) or (not var_type.value):
            assign_to_var(instruction.args[0].value.split('@'), frames, 'string', '')
        assign_to_var(instruction.args[0].value.split('@'), frames, 'string', str(var_type.type))
    else:
        assign_to_var(instruction.args[0].value.split('@'), frames, 'string', str(instruction.args[1].type))


"""
EXIT
exit script with symb value
checks correct exit val range
"""


def exit_function(instruction, frames):
    if instruction.args[0].type == 'var':
        return_var = parse_symb(instruction.args[0].value.split('@'), frames)
        if return_var.type == 'int' and 0 <= return_var.value <= 49:
            exit(return_var.value)
        else:
            exit(57)
    elif instruction.args[0].type == 'int':
        return_val = int(instruction.args[0].value)
        if 0 <= return_val <= 49:
            exit(return_val)
        else:
            exit(57)


"""
DPRINT
prints symb to stderr
"""


def dprint_function(instruction, frames):
    if instruction.args[0].type == 'var':
        var_value = parse_symb(instruction.args[0].value.split('@'), frames).value
        sys.stderr.write(var_value)
    else:
        sys.stderr.write(instruction.args[0].value)


"""
BREAK
Releases all information about the current script run
"""


def break_function(instruction, frames):
    sys.stderr.write("Position in code: " + instruction.order)
    sys.stderr.write("Processed instructions: " + instruction.order)
    sys.stderr.write("GF frame")
    for gf_value in frames.GF:
        sys.stderr.write(gf_value)

    sys.stderr.write("LF frame")
    for lf_value in frames.LF:
        sys.stderr.write(lf_value)

    sys.stderr.write("LF frame")
    for tf_value in frames.TF:
        sys.stderr.write(tf_value)


"""
Jump fuctions
checks if label exists in labels dict
if necessary checks equality or inequality of symb
runs main instruction selection function with new starting position
"""


def jump_function(instruction, instruction_list, frames, labels, input_file):
    if instruction.args[0].value in labels:
        interpret_instruction(instruction_list, frames, int(labels[instruction.args[0].value]), labels, input_file)
    else:
        exit(52)


def jumpifeq_function(instruction, instruction_list, frames, labels, input_file):
    if check_eq_symb(instruction, frames):
        if not instruction.args[0].value in labels:
            exit(52)
        interpret_instruction(instruction_list, frames, int(labels[instruction.args[0].value]), labels, input_file)
        return True
    return False


def jumpifneq_function(instruction, instruction_list, frames, labels, input_file):
    if not check_eq_symb(instruction, frames):
        if not instruction.args[0].value in labels:
            exit(52)
        interpret_instruction(instruction_list, frames, int(labels[instruction.args[0].value]), labels, input_file)
        return True
    return False


"""
Selects instruction based on opcode from instruction_list and checks correct num of args
Instruction_list starts from index selected by caller (used for jumps to labels)
"""


def interpret_instruction(instruction_list, frames, start_index, labels, input_file):
    if start_index >= len(instruction_list):
        return  # if return to call instruction on last index
    for instruction in instruction_list[start_index:]:
        if instruction.name == 'MOVE':
            if len(instruction.args) != 2:
                exit(32)
            move_function(instruction, frames)
        elif instruction.name ==  'CREATEFRAME':
            if len(instruction.args) != 0:
                exit(32)
            createframe_function(frames)
        elif instruction.name == 'PUSHFRAME':
            if len(instruction.args) != 0:
                exit(32)
            pushframe_function(frames)
        elif instruction.name == 'POPFRAME':
            if len(instruction.args) != 0:
                exit(32)
            popframe_function(frames)
        elif instruction.name == 'DEFVAR':
            if len(instruction.args) != 1:
                exit(32)
            defvar_function(instruction, frames)
        elif instruction.name == 'CALL':
            if len(instruction.args) != 1:
                exit(32)
            call_function(instruction, instruction_list, frames, labels, input_file)
        elif instruction.name == 'RETURN':
            if len(instruction.args) != 0:
                exit(32)
            return_function(instruction_list, frames, labels, input_file)
            break
        elif instruction.name == 'PUSHS':
            if len(instruction.args) != 1:
                exit(32)
            pushs_function(instruction, frames)
        elif instruction.name == 'POPS':
            if len(instruction.args) != 1:
                exit(32)
            pops_function(instruction, frames)
        elif instruction.name == 'ADD':
            if len(instruction.args) != 3:
                exit(32)
            add_function(instruction, frames)
        elif instruction.name == 'SUB':
            if len(instruction.args) != 3:
                exit(32)
            sub_function(instruction, frames)
        elif instruction.name == 'MUL':
            if len(instruction.args) != 3:
                exit(32)
            mul_function(instruction, frames)
        elif instruction.name == 'IDIV':
            if len(instruction.args) != 3:
                exit(32)
            idiv_function(instruction, frames)
        elif instruction.name == 'LT':
            if len(instruction.args) != 3:
                exit(32)
            lt_function(instruction, frames)
        elif instruction.name == 'GT':
            if len(instruction.args) != 3:
                exit(32)
            gt_function(instruction, frames)
        elif instruction.name == 'EQ':
            if len(instruction.args) != 3:
                exit(32)
            eq_function(instruction, frames)
        elif instruction.name == 'AND':
            if len(instruction.args) != 3:
                exit(32)
            and_function(instruction, frames)
        elif instruction.name == 'OR':
            if len(instruction.args) != 3:
                exit(32)
            or_function(instruction, frames)
        elif instruction.name == 'NOT':
            if len(instruction.args) != 2:
                exit(32)
            not_function(instruction, frames)
        elif instruction.name == 'INT2CHAR':
            if len(instruction.args) != 2:
                exit(32)
            int2str_function(instruction, frames)
        elif instruction.name == 'STRI2INT':
            if len(instruction.args) != 3:
                exit(32)
            stri2int_function(instruction, frames)
        elif instruction.name == 'READ':
            if len(instruction.args) != 2:
                exit(32)
            read_function(instruction, frames, input_file)
        elif instruction.name == 'WRITE':
            if len(instruction.args) != 1:
                exit(32)
            write_function(instruction, frames)
        elif instruction.name == 'CONCAT':
            if len(instruction.args) != 3:
                exit(32)
            concat_function(instruction, frames)
        elif instruction.name == 'STRLEN':
            if len(instruction.args) != 2:
                exit(32)
            strlen_function(instruction, frames)
        elif instruction.name == 'GETCHAR':
            if len(instruction.args) != 3:
                exit(32)
            getchar_function(instruction, frames)
        elif instruction.name == 'SETCHAR':
            if len(instruction.args) != 3:
                exit(32)
            setchar_function(instruction, frames)
        elif instruction.name == 'TYPE':
            if len(instruction.args) != 2:
                exit(32)
            type_function(instruction, frames)
        elif instruction.name == 'EXIT':
            if len(instruction.args) != 1:
                exit(32)
            exit_function(instruction, frames)
        elif instruction.name == 'DPRINT':
            if len(instruction.args) != 1:
                exit(32)
            dprint_function(instruction, frames)
        elif instruction.name == 'BREAK':
            if len(instruction.args) != 0:
                exit(32)
            break_function(instruction, frames)
        elif instruction.name == 'LABEL':
            if len(instruction.args) != 1:
                exit(32)
            continue
        elif instruction.name == 'JUMP':
            if len(instruction.args) != 1:
                exit(32)
            jump_function(instruction, instruction_list, frames, labels, input_file)
            break
        elif instruction.name == 'JUMPIFEQ':
            if len(instruction.args) != 3:
                exit(32)
            if jumpifeq_function(instruction, instruction_list, frames, labels, input_file):
                break
        elif instruction.name == 'JUMPIFNEQ':
            if len(instruction.args) != 3:
                exit(32)
            if jumpifneq_function(instruction, instruction_list, frames, labels, input_file):
                break
        else:
            exit(32)  # wrong opcode


"""
loops through instruction to find labels before instructions are executed
also collects call instruction indexes in call stack
"""


def set_labels(instruction_list):
    labels = {}
    labels["call"] = deque()
    index = 0
    for instruction in instruction_list:
        if instruction.name == 'LABEL':
            if instruction.args[0].value not in labels:
                labels[instruction.args[0].value] = index
            else:
                exit(52)
        if instruction.name == 'CALL':
            labels['call'].append(index)
        index += 1
    return labels


"""
parses XML code
checks for specific parts of code like ippcode, instruciton, order etc.
collects and parses arguments from xml assigns them to arg class, args are sorted numerically
the same for instructions
"""


def parse(root):
    instructions_list = []
    if root.get('language') != 'IPPcode22':
        exit(32)
    if root.tag != 'program':
        exit(32)
    for child in root:
        arguments = []
        if child.tag != "instruction":
            exit(32)
        child_keys = list(child.attrib.keys())
        for args in child_keys:
            if not ('order', 'opcode' in args):
                exit(32)

        for subElem in child:
            if not (re.match("r*arg[123]", subElem.tag)):
                exit(32)
            result = re.search("^arg([1-3])$", subElem.tag)
            order = result.group(1)
            argument = Argument(subElem.get("type"), subElem.text, order)
            arguments.append(argument)

        arguments.sort(key=lambda s_arg: int(s_arg.order))

        instruction = Instruction(child.get("opcode").upper(), arguments, child.get("order"))
        instructions_list.append(instruction)
    instructions_list.sort(key=lambda instr: int(instr.order))

    # makes sure that all order numbers are unique
    order_list = []
    for ins in instructions_list:
        if int(ins.order) < 1:
            exit(32)
        order_list.append(ins.order)
        arg_index = 0
        for arg in ins.args:
            if arg.order != '1':
                if not (len(ins.args) >= int(arg.order) and ins.args[arg_index - 1] is not None):
                    exit(32)
            arg_index += 1

    if len(instructions_list) != len(set(order_list)):
        exit(32)

    return instructions_list


"""
parses input parameters and runs all main parts of script
"""


def main():
    ap = argparse.ArgumentParser(
        description='Python 3.8 script used to interpret IPPcode22 in xml format. Used by running python interpret.py with atleast one parameter')
    ap.add_argument("--source", nargs=1, type=str, help="source file with xml code")
    ap.add_argument("--input", nargs=1, help="file with input code")
    args = ap.parse_args()
    # at least one parameter must be entered
    if args.source is None and args.input is None:
        exit(10)
    # try to load source file
    if args.source is not None:
        source_file = args.source[0]
    else:
        source_file = sys.stdin
    # try to load input file
    if args.input is not None:
        input_file = open(args.input[0], "r")
    else:
        input_file = sys.stdin

    # xml load
    try:
        tree = ET.parse(source_file)
        root = tree.getroot()
        frames = Frames()
        instruction_list = parse(root)
        labels = set_labels(instruction_list)
        interpret_instruction(instruction_list, frames, 0, labels, input_file)
    except:
        exit(31)


main()
