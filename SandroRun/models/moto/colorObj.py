# Load a file and read it line by line

import os
import sys

def main():

    if(len(sys.argv) < 6 or sys.argv[1] == "-h" or sys.argv[1] == "--help"):
        print("Usage: python colorObj.py <input_file> <component_name> <r_color> <g_color> <b_color> [-fix]")
        exit()

    input_file = sys.argv[1]
    component_name = sys.argv[2]
    r_color = sys.argv[3]
    g_color = sys.argv[4]
    b_color = sys.argv[5]
    if(len(sys.argv) == 7 and sys.argv[6] == "-fix"):
        fix = True
    else:
        fix = False


    # Open file
    file_in = open(input_file, "r")


    # For loop on each line
    check = False

    new_lines = []

    for line in file_in:

        new_line = line

        if line.startswith("o "):
            if line.split("o ")[1][:-1] == component_name:
                check = True

            else:
                check = False
        elif check and line.startswith("v "):
            if(fix):
                new_line = " ".join(line.split(" ", 4)[:4]) + " " + r_color + " " + g_color + " " + b_color + "\n"
            else:
                new_line = line[:-1] + " " + r_color + " " + g_color + " " + b_color + "\n"
        
        new_lines.append(new_line)
            
    # Close file
    file_in.close()

    os.rename(input_file, input_file.split(".")[0] + ".old.obj")

    # Open file
    file_out = open(input_file, "w")

    # Write lines
    file_out.writelines(new_lines)

    # Close file
    file_out.close()



if __name__ == "__main__":
    main()
