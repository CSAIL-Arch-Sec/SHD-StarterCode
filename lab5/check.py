#!/usr/bin/env python3
import subprocess
import argparse
from tqdm import tqdm
import os

'''
check.py
A simple autograder that you can use to check the answers to Lab 5.

This does not include partial credit for nearly correct answers.
'''

def print_success():
    print("\033[92mSuccess! Good job")
    print("\033[0m")

def print_failure(error_rate, trials):
    print("\033[91mNot quite")
    print(f"We consider a success for this part to be {(error_rate * trials * 100.0) / trials}%")
    print("\033[0m")

def check(name, trials, secret, error_rate):
    '''
    name: The name of the program to check
    trials: How many trials to run
    secret: The correct secret
    error_rate: A float between 0.0 and 1.0 indicating how much error is a success

    Returns True on success, False on failure, and prints message to stdout indicating the result
    '''
    good_runs=0

    for i in tqdm(range(trials)):
        p = subprocess.Popen([f'./{name}'], stdout=subprocess.PIPE)
        result,err = p.communicate()

        if secret in result:
            good_runs+=1

    print(f"You passed {good_runs} of {trials} runs ({(good_runs * 100.0) / trials}%)")
    if good_runs >= error_rate * trials:
        print_success()
        return True
    else:
        print_failure(error_rate, trials)
        return False

def main():
    parser = argparse.ArgumentParser(description="Check your lab 5 code")
    parser.add_argument('part', type=str, help="Which part to check? 1a, 1b, 1c, 2a, 2b, or 3?")
    args = parser.parse_args()

    if args.part not in ['1a','1b', '1c', '2a', '2b', '3', 'all']:
        print("error: unknown part")
        exit(-1)

    points = 0
    points_possible = 0

    if '1' in args.part or args.part == 'all':
        os.chdir('part1')
        os.system('make')
        os.chdir('..')
    
    if '2' in args.part or args.part == 'all':
        os.chdir('part2')
        os.system('make')
        os.chdir('..')
    
    if '3' in args.part or args.part == 'all':
        os.chdir('part3')
        os.system('make')
        os.chdir('..')

    # Don't expect these hardcoded secrets to be the same when we grade!
    # Also, while this script will run forever, we will kill your autograder if it takes too long
    if args.part == '1a' or args.part == 'all':
        print("Checking part 1A...")
        points_possible += 20
        if check("part1/a", 1000, b"Correct!!", 1.00):
            points += 20
    if args.part == '1b' or args.part == 'all':
        print("Checking part 1B...")
        points_possible += 20
        if check("part1/b", 1000, b"Correct!!", 0.95):
            points += 20
    if args.part == '1c' or args.part == 'all':
        print("Part 1C has no auto-checker")
        print_success()
    if args.part == '2a' or args.part == 'all':
        # We allow this to fail on occasion in case an 0x0A byte showed up randomly
        # in an address somewhere, stopping the overflow from completing.
        print("Checking part 2A...")
        points_possible += 10
        if check("part2/a", 1000, b"MIT{nic3_s0lv3!}", 0.50):
            points += 10
    if args.part == '2b' or args.part == 'all':
        # We allow this to fail on occasion in case an 0x0A byte showed up randomly
        # in an address somewhere, stopping the overflow from completing.
        print("Checking part 2B...")
        points_possible += 20
        if check("part2/b", 1000, b"MIT{lab5_p4rtTw0_1s_d0n3}", 0.50):
            points += 20
    if args.part == '3' or args.part == 'all':
        print("Checking part 3...")
        points_possible += 10
        os.chdir('part3')
        if check("part3", 1000, b"MIT{and_thats_all_folks!}", 0.50):
            points += 10
    
    print(f"Your score is {points} / {points_possible}")

    if args.part == 'all':
        print(f"You scored {(100.0 * points) / points_possible}% for Lab 5!")
        if points == points_possible:
            print("Great job!!!")
    else:
        print(f"You scored {(100.0 * points) / points_possible}% for this part!")

if __name__=="__main__":
    main()
