# Load a file and read it line by line

import os
import sys

def color(fix):
    input_file = sys.argv[2]
    component_name = sys.argv[3]
    r_color = sys.argv[4]
    g_color = sys.argv[5]
    b_color = sys.argv[6]

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

def remove_color():
    input_file = sys.argv[2]

    # Open file
    file_in = open(input_file, "r")

    new_lines = []

    for line in file_in:

        new_line = line

        if line.startswith("v "):
            components = new_line.split()
            if(len(components) != 4):
                new_line = " ".join(line.split(" ", 4)[:4]) + "\n"
        
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

def remove_texture():
    input_file = sys.argv[2]

    # Open file
    file_in = open(input_file, "r")

    new_lines = []

    for line in file_in:

        if line.startswith("vt "):
            continue

        new_line = line

        if line.startswith("f"):
            new_line = "f"
            components = line.split("f ")[1].split()
            for component in components:
                new_line += " " + component.split("/")[0] + "//" + component.split("/")[2]
            new_line += "\n" 
        
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

def texature():
    input_file = sys.argv[2]
    component_name = sys.argv[3]
    u = sys.argv[4]
    v = sys.argv[5]

    # Open file
    file_in = open(input_file, "r")

    # For loop on each line
    check = False
    done = False
    count = 0

    new_lines = []

    for line in file_in:

        new_line = line

        if line.startswith("o "):
            if line.split("o ")[1][:-1] == component_name:
                check = True

            else:
                if(check):
                    done = True
                check = False
        elif check and line.startswith("s "):
            new_lines.append("vt " + u + " " + v + "\n")
            count += 1
        elif line.startswith("vt "):
            count += 1
        elif check and line.startswith("f"):
            new_line = "f"
            components = line.split("f ")[1].split()
            for component in components:
                new_line += " " + component.split("/")[0] + "/" + str(count) + "/" + component.split("/")[2]
            new_line += "\n"
        elif done and line.startswith("f "):
            new_line = "f"
            components = line.split("f ")[1].split()
            for component in components:
                new_line += " " + component.split("/")[0] + "/" + str(count) + "/" + component.split("/")[2]
            new_line += "\n"
        
        new_lines.append(new_line)
            
    # Close file
    file_in.close()

    os.remove(input_file.split(".")[0] + ".old.obj")

    os.rename(input_file, input_file.split(".")[0] + ".old.obj")

    # Open file
    file_out = open(input_file, "w")

    # Write lines
    file_out.writelines(new_lines)

    # Close file
    file_out.close()

def main():
    if(sys.argv[1] == "--color"):
        color(False)
    elif(sys.argv[1] == "--color-fix"):
        color(True)
    elif(sys.argv[1] == "--color-rm"):
        remove_color()
    elif(sys.argv[1] == "--texture-rm"):
        remove_texture()
    elif(sys.argv[1] == "--texture"):
        texature()
    else:
        print("Usage:\n")
        print("python colorObj.py --color <input_file> <component_name> <r_color> <g_color> <b_color>")
        print("python colorObj.py --color-fix <input_file> <component_name> <r_color> <g_color> <b_color>")
        print("python colorObj.py --color-rm <input_file>")
        print("python colorObj.py --texture-rm <input_file>")
        print("python colorObj.py --texture <input_file> <component_name> <u> <v>")
        exit()

if __name__ == "__main__":
    main()
