<?php
####IPP project 1 - parser
####Author: xjuhas04

###FUNCTIONS###
#Used for instructions with only one var argument
#Accepts instruction array and order of instruction number
function instr_var ($instruction, $order)
{
    if(count($instruction) == 2) #checks for correct num of args
    {
        if(($instruction[1] = is_type_var($instruction[1])) != NULL) #checks if var for lex errors
        {
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"var\">".$instruction[1]."</arg1>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect var in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
    
    }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
        exit(23);
    }
}
#Used for instructions with only one symb argument
#Accepts instruction array and order of instruction number
function instr_sym ($instruction, $order)
{
    if(count($instruction) == 2) #checks for correct num of args
    {
        if((($sym1=parse_type($instruction[1])) != NULL) #checks sym arg for lex errors
                    || (($sym1=parse_var($instruction[1])) != NULL))
        {
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"".$sym1[0]."\">".$sym1[1]."</arg1>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect argument in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
    }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
        exit(23);
    }
}

#Used for instructions with one var and two symb arguments
#Accepts instruction array and order of instruction number
function instr_var_2sym($instruction, $order)
{
    if (count($instruction) == 4) #checks for correct num of args
    {
        if (parse_var($instruction[1]) #checks var for lex errors, and then either
                && ((($sym1=parse_type($instruction[2])) != NULL)  #symb or var args for lex errors
                    || (($sym1=parse_var($instruction[2])) != NULL))
                        && ((($sym2=parse_type($instruction[3])) != NULL) 
                            || (($sym2=parse_var($instruction[3])) != NULL)))
        {
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"var\">".$instruction[1]."</arg1>\n");
            echo("\t\t<arg2 type=\"".$sym1[0]."\">".$sym1[1]."</arg2>\n");
            echo("\t\t<arg3 type=\"".$sym2[0]."\">".$sym2[1]."</arg3>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect argument in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
    }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
         exit(23);
    }
}

#Used for instructions with one var and one symb argument
#Accepts instruction array and order of instruction number
function instr_var_sym($instruction, $order)
{
    if (count($instruction) == 3) #checks for correct num of args
    {
        if (parse_var($instruction[1]) #checks var for lex errors and then either another var or
                &&  ((((($sym1=parse_type($instruction[2])) != NULL) #sym for lex errors 
                    || (($sym1=parse_var($instruction[2])) != NULL)))
                || ( (strcmp($instruction[0],"MOVE")==0) && ($sym1 = parse_type_no_value($instruction[2]))!=NULL)))
        {
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"var\">".$instruction[1]."</arg1>\n");
            echo("\t\t<arg2 type=\"".$sym1[0]."\">".$sym1[1]."</arg2>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect argument in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
    }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
         exit(23);
    }    
}

#Used for instructions with one var and one type argument
#Accepts instruction array and order of instruction number
function instr_var_type($instruction, $order)
{

    if (count($instruction) == 3) #checks for correct num of args
    {
        if (($instruction[1] = is_type_var($instruction[1])) != NULL 
            && (correct_type($instruction[2]))) #checks args for lex errors
        {                                                                           
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"var\">".$instruction[1]."</arg1>\n");
            echo("\t\t<arg2 type=\"type\">".$instruction[2]."</arg2>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect argument in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
     }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
         exit(23);
    }
}

#Used for instructions with only one label argument
#Accepts instruction array and order of instruction number
function instr_label($instruction, $order)
{
    if (count($instruction) == 2) #checks for correct num of args
    {
        if (($instruction[1] = is_name_valid($instruction[1])) != NULL) #checks if var for lex error
        {
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"label\">".$instruction[1]."</arg1>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect argument in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
     }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
         exit(23);
    }
}

#Used for instructions with one label and two symb arguments
#Accepts instruction array and order of instruction number
function instr_label_2sym($instruction, $order)
{
    if (count($instruction) == 4) #checks for correct number of instructions
    {
        if (($instruction[1] = is_name_valid($instruction[1])) != NULL #checks args for lex errors
                && ((($sym1=parse_type($instruction[2])) != NULL) 
                    || (($sym1=parse_var($instruction[2])) != NULL))
                        && ((($sym2=parse_type($instruction[3])) != NULL) 
                            || (($sym2=parse_var($instruction[3])) != NULL)))
        {
            echo ("\t<instruction order=\"".$order."\" opcode=\""
                .strtoupper($instruction[0])."\">\n");
            echo("\t\t<arg1 type=\"label\">".$instruction[1]."</arg1>\n");
            echo("\t\t<arg2 type=\"".$sym1[0]."\">".$sym1[1]."</arg2>\n");
            echo("\t\t<arg3 type=\"".$sym2[0]."\">".$sym2[1]."</arg3>\n");
            echo ("\t</instruction>\n");
        }
        else
        {
            fwrite(STDERR,"Incorrect argument in instruction: ".$instruction[0].
                ", with order number: ".$order.".\n");
            exit(23);
        }
 
    }
    else
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction: "
            .$instruction[0].", with order number: ".$order.".\n");
         exit(23);
    }
}
#Used for instruction with no args
#Accepts instruction array and order of instruction number
function instr_no_arg($instruction, $order)
{
    if (count($instruction) == 1) #checks for correct num of args
    {
        echo ("\t<instruction order=\"".$order."\" opcode=\"".strtoupper($instruction[0])."\">\n");
        echo ("\t</instruction>\n");
    }
    else 
    {
        fwrite(STDERR,"Incorrect amount of arguments in instruction:
            ".$instruction[0].", with order number: ".$order.".\n");
         exit(23);
    }
}
#makes sure correct types are used in arguments
#accepts type string split from constant string 
function correct_type($type)
{
    if ((strcmp($type, "bool") == 0)
    || (strcmp($type, "int") == 0)
    || (strcmp($type,"nil") == 0)
    || (strcmp($type,"string") == 0))
        return true;
    else
        return false;
}
#In cases where constant doesn't require value (f.e. MOVE)
#accepts type of constant
function parse_type_no_value($type)
{
    #seperates strings in sym with @
    $type = array_filter(explode('@', $type),'strlen');
    #only allows string type to be without value
    if ((count($type) == 1) && (strcmp($type[0],"string") == 0)) 
         return array($type[0],""); 
    else
        return NULL;
}
#parses type (symb) checks induvidial parts for errors and if required rewrittes strings
#accepts type arg (symb)
function parse_type($type)
{
    #seperates strings in sym with @
    $type = explode('@', $type);

    if (count($type) == 1) #if it only contains type and no value
        return NULL; 
    else
    {
        if (($type[1] = is_type_valid($type[0],$type[1])) != NULL) #checks for lex errors
            return $type;
        else
           return NULL; #script should fail
    }
}
#checks if name only contains allowed characters
#accepts var name
function is_name_valid ($name) 
{
    if (empty($name)) #if name is empty
        return NULL;
    #or contains ilegal char script will fail
    else if (!preg_match('/^[a-zA-Z_\-$&%*!?][a-zA-Z_\-$&%*!?0-9]*$/',$name))
        return NULL;                                                            
    else #checks for special characters that should be replaced
    {
        return rewrite_string($name);    
    }
}

#removes comments from end of line
#accepts line
function remove_comments($line)
{
    $temp = explode('#',$line);
    return trim($temp[0]);
}

#checks if var def has correct structure and parses into an array
#accepts var string
function parse_var($string)
{
    $var = explode('@',$string);
    #checks for correct frame & validity of name
    if (($var [0] == "GF" || $var[0] == "LF" || $var[0] == "TF") 
            && ($name = is_name_valid($var[1])) != NULL)
        return  array("var", implode("@",array($var[0],$name))); #returns new array with var
    else
        return NULL;
}
#checks if var def has correct structure
#accepts var string
function is_type_var($string)
{
    $var = explode('@',$string);
    #checks for correct frame & validity of name
    if (($var [0] == "GF" || $var[0] == "LF" || $var[0] == "TF") 
            && ($name = is_name_valid($var[1])) != NULL)
                return implode("@",array($var[0],$name));
    else
        NULL;
}
#rewrites string special characters 
#accepts string with special characters
function rewrite_string($string)
{
        if(preg_match('/&/',$string))
            return str_replace("&","&amp;",$string);
        else if (preg_match('/</',$string))
            return str_replace("<","&lt;",$string);
        else if (preg_match('/>/',$string))
            return str_replace(">","&gt;",$string);
        return
            $string;
}
#checks is string has correct structure
#accepts string
function check_string($string)
{
    if (preg_match("/\\s#/",$string))
        return false;
    if (preg_match("/\\\\/",$string))
    {
        $backslash = substr_count($string,'\\');
        $matches = preg_match_all("/\\\\[0-9]{3}/",$string,$matches,PREG_PATTERN_ORDER);
        return $backslash == $matches ? rewrite_string($string) : NULL;
    }
    return rewrite_string($string);
}

#checks is symb have correct type of value 
function is_type_valid($type, $const)
{
    if (strcmp($type, "bool") == 0)
        return ((strcmp($const, "true") == 0)
            || (strcmp($const, "false") == 0)) ? $const : NULL;
    else if(strcmp($type, "int") == 0)
        return (is_numeric($const) ? $const : NULL);
    else if(strcmp($type,"nil") == 0)
        return (strcmp($const,"nil") == 0) ? $const : NULL;
    else if (strcmp($type,"string") == 0)
    {
        return check_string($const);
    }
    else
    {
        return NULL;
    }
}
#function displaying info about script
function help_func()
{
    echo ("parse.php script (written in PHP 8.1).\n");
    echo ("Used for reading source code of IPPCODE22 from stdin\n");
    echo ("checking it for lexical and syntax errors and exporting it as xml on stdin.\n");
    echo ("Can be used with parameter --help to display this message.\n");
    echo ("Example: php parse.php --help");
    echo ("Script accepts files from stdin and also prints output on stdin");
    echo ("Example: php parse.php <FILE.src >OUTPUT.xml");
    exit(0);
}
###MAIN###
ini_set('display_errors', 'stderr');
#if there is more than arg checks for parameters
if ($argc > 1)
{
    if($argv[1] == "--help")
    {
        help_func();
    }
    else 
    {   #runned with unsupported parameter
        fwrite(STDERR,"Script was runned with unrecognized parameter: ".$argv[2].".\n");
        exit(10);
    }
}
$header = false;
#reads file from stdin line by line 
for ($order = 0;$line = fgets(STDIN); $order++)
{
    $line = remove_comments($line);
    if (!$header) #makes sure header is on top of file
    {
        if (strcmp(strtoupper($line),".IPPCODE22") == 0)
        {
            $header = true;
            echo ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            echo ("<program language=\"IPPcode22\">\n");
            continue;
        }
        else if(empty($line))
        { #skips empty lines
            $order--;
            continue;
        }
        else 
        {
            fwrite(STDERR,"Source file does not contain required header.\n");
            exit(21);
        }
    }
    
    #Divides lines into instructions based on whitespace 
    #also removes indexes with just whitespace
    $instruction = array_values(array_filter(array_map('trim', explode (' ' , trim($line, "\n")))));
    if (empty($instruction)) #skips empty instructions
    {
        $order--;
        continue;
    }
    switch(strtoupper($instruction[0]))
    {
        #function calls and work with frames
        case 'MOVE':
            instr_var_sym($instruction, $order);
            break;
        case 'CREATEFRAME':
            instr_no_arg($instruction, $order);
            break;
        case 'PUSHFRAME':
            instr_no_arg($instruction, $order);
            break;
        case 'POPFRAME':
            instr_no_arg($instruction, $order);
            break;
        case 'DEFVAR':
            instr_var($instruction, $order);
            break;
        case 'CALL':
            instr_label($instruction, $order);
            break;
        case 'RETURN':
            instr_no_arg($instruction, $order);
            break;
        
        #Work with stack
        case 'PUSHS':
           instr_sym($instruction, $order);
           break;
        case 'POPS':
            instr_var($instruction, $order);
            break;
        #Arithmetic... instruction
        case 'ADD':
            instr_var_2sym($instruction, $order);
            break;
        case 'SUB':
            instr_var_2sym($instruction, $order);
            break;
        case 'MUL':
            instr_var_2sym($instruction, $order);
            break;
        case 'IDIV':
            instr_var_2sym($instruction, $order);
            break;
        case 'LT':
            instr_var_2sym($instruction, $order);
            break;
        case 'GT':
            instr_var_2sym($instruction, $order);
            break;
        case 'EQ':
            instr_var_2sym($instruction, $order);
            break;
        case 'AND':
            instr_var_2sym($instruction, $order);
            break;
        case 'OR':
            instr_var_2sym($instruction, $order);
            break;
        case 'NOT':
            instr_var_sym($instruction,$order);
            break;
        case 'INT2CHAR':
            instr_var_sym($instruction, $order);
            break;
        case 'STRI2INT':
            instr_var_2sym($instruction, $order);
            break;

        #Input and output instructions.
        case 'READ':
            instr_var_type($instruction, $order);
            break;
        case 'WRITE':
           instr_sym($instruction, $order); 
           break;
        #String instructions.
        case 'CONCAT':
            instr_var_2sym($instruction, $order);
            break;
        case 'STRLEN':
            instr_var_sym($instruction, $order);
            break;
        case 'GETCHAR':
            instr_var_2sym($instruction, $order);
            break;
        case 'SETCHAR':
            instr_var_2sym($instruction, $order);
            break;

        #Type instruction
        case 'TYPE':
            instr_var_sym($instruction, $order);
            break;

        #Control of program instructions
        case 'LABEL':
            instr_label($instruction, $order);
            break;
        case 'JUMP':
            instr_label($instruction, $order);
            break; 
        case 'JUMPIFEQ':
            instr_label_2sym($instruction, $order);
            break;
        case 'JUMPIFNEQ':
            instr_label_2sym($instruction, $order);
            break;
        case 'EXIT':
            instr_sym($instruction, $order); 
            break;
        #Debugging instructions
        case 'DPRINT':
            instr_sym($instruction, $order); 
            break;
        case 'BREAK' :
            instr_no_arg($instruction, $order);
            break;
        default:
            if (!empty($instruction[0]))
            {
                if ($instruction[0][0] == '#') #comment skipped
                {
                    $order--;
                    break;
                }
                else
                {
                    fwrite(STDERR,"Unrecognized instruction.\n");
                    exit(22);
                }
            }
            else
            {
                #empty line skipped
                $order--;
            }
            

    }
}
if(!$header) #empty file without header
{
    fwrite(STDERR,"Header missing.\n");
    exit(21);
}
echo("</program>\n");
?>
