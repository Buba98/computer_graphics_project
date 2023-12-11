# Load a file and read it line by line

import os
import sys

def main():

    print("Number of arguments: ", len(sys.argv))
    print("Argument List: ", str(sys.argv))

    input_file = sys.argv[1]
    component_name = sys.argv[2]
    r_color = sys.argv[3]
    g_color = sys.argv[4]
    b_color = sys.argv[5]

    # Copy file
    new_file = input_file.split(".")[0] + ".colored.obj"

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
            new_line = line[:-1] + " " + r_color + " " + g_color + " " + b_color + "\n"
        
        new_lines.append(new_line)
            
    # Close file
    file_in.close()

    # Open file
    file_out = open(new_file, "w")

    # Write lines
    file_out.writelines(new_lines)

    # Close file
    file_out.close()



if __name__ == "__main__":
    main()