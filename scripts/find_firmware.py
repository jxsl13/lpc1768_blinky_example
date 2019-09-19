#!/#!/bin/env python3
from getpass import getpass
from subprocess import run, PIPE, Popen
import os
import pexpect
import re
import csv

#
# the black magic probe is capable of surviving more than 10k firmware flashes.
#

blackmagic_folder = "/Users/john/Desktop/blackmagic_uwebonnes"
project_folder = "/Users/john/Documents/PlatformIO/Projects/Testing_MCB1768"
debug_port = "/dev/cu.usbmodem79AC68971"
incorrect_driver = ["Nordic nRF51 M3/M4", "Nordic nRF51"] #"Nordic nRF51 M3/M4", "ARM Cortex-M", 


def p(x):
    if not x:
        return
    result = x.stdout.decode('utf-8')
    for line in result.split("\n"):
        print(line)
    
    cerr = x.stderr.decode("utf-8").split("\n")
    for line in cerr:
        print(line)

def older_commit():
    result = run(["git", "checkout", "HEAD~1"], capture_output=True)
    result = run(["git", "rev-parse", "HEAD"], capture_output=True)
    return result.stdout.decode('utf-8')


def fix_makefile():
    makefile = f"{blackmagic_folder}/src/Makefile"

    with open(makefile, mode='r') as f:
        lines = f.readlines()
        for idx, line in enumerate(lines):
            if "CFLAGS += -Wall -Wextra -Werror -Wno-char-subscripts\\" in line:
                #lines[idx] = line[:-2] + " -Wno-cast-function-type -Wno-implicit-fallthrough \\\n"
                lines[idx] = "CFLAGS += \\\n"
                break
        
        with open(makefile, mode="w") as ff:
            ff.writelines(lines)

def reset_makefile():
    backup = os.getcwd()
    os.chdir(f"{blackmagic_folder}/src")
    run(["git", "checkout", "Makefile"])
    os.chdir(backup)

def clean():
    backup = os.getcwd()
    os.chdir(blackmagic_folder)
    run(["make", "clean"])
    os.chdir(backup)


def pull_dependencies():
    print("GETTING DEPENDENCIES")
    backup = os.getcwd()
    os.chdir(blackmagic_folder)
    run(["git", "submodule", "update", "--init", "--recursive"])
    os.chdir(backup) 


def build_blackmagic_firmware():
    fix_makefile()
    clean()
    pull_dependencies()
    os.chdir(f"{blackmagic_folder}")
    result = run(["make", "all"])
    reset_makefile()
    return result.returncode == 0

def execute_step(password):

    print("========== EXECUTING STEP ==========")
    print(f"CWD: {blackmagic_folder}")

    print("GOING TO THE NEXT COMMIT")
    os.chdir(blackmagic_folder)
    git_hash = older_commit()
    print(git_hash)

    print("COMPILING BMP FIRMWARE")
    success = build_blackmagic_firmware()
    # if building fails, we don't want to continue, as you will
    # build and flash the same previous firmware.
    if not success:
        return (True, git_hash.strip(), None)

    print("FLASHING BMP FIRMWARE")
    os.chdir(f"{blackmagic_folder}/src")
    result = run(f'echo {password} | sudo -S dfu-util -d 1d50:6018,:6017 -s 0x08002000:leave -D blackmagic.bin', shell=True, capture_output=True)
    p(result)

    print(f"CWD: {project_folder}")
    os.chdir(project_folder)

    print("STARTING PIODEBUGGDB")
    child = pexpect.spawn('piodebuggdb')
    child.expect("(gdb)")
    child.sendline(f"target extended-remote {debug_port}")
    child.expect("(gdb)")
    child.sendline("monitor swdp_scan")
    child.expect("(gdb)")
    
    output = child.before.decode("utf-8")
    match = re.search(r"[\d]+[\ ]+(.*)", output)
    driver = match.group(1)
    print("DRIVER FOUND:", driver)

    child.sendline("q")
    print("========== FINISHED STEP ==========")

    return (False, git_hash.strip(), driver.strip())



if __name__ == "__main__":

    password = getpass("Please enter your password: ")
    filename = 'drivers.csv'
    exists = os.path.exists(filename)

    with open(filename, 'a+', newline='') as csvfile:
        fieldnames = ['Git_Hash', 'Driver']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        if not exists:
            # new file created, needs header
            writer.writeheader()
        

        skip, git_hash, driver = execute_step(password)
        print(git_hash, driver)
        writer.writerow({'Git_Hash': git_hash, 'Driver': driver})
        previous_hash = git_hash
        entry = True
        while entry or driver in incorrect_driver:
            csvfile.flush()
            entry = False
            skip, git_hash, driver = execute_step(password)

            if previous_hash == git_hash:
                print("EARLY EXIT: PREVIOUS HASH == CURRENT HASH")
                exit(0)
            else:
                previous_hash = git_hash
            
            if skip:
                writer.writerow({'Git_Hash': git_hash, 'Driver': 'BUILD FAILED'})
                print("SKIPPING")
                continue
            print(git_hash)
            print(driver)
            writer.writerow({'Git_Hash': git_hash, 'Driver': driver})
    
    print(f"Different driver seen: {driver} at hash {git_hash}")

