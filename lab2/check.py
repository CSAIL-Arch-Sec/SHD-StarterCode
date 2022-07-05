#!/usr/bin/env python3
import subprocess
import argparse
from tqdm import tqdm

'''
check.py
A simple autograder that you can use to check the answers to Lab 2.

This does not include partial credit for nearly correct answers.
'''

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
        print("\033[92mSuccess! Good job")
        print("\033[0m")
        return True
    else:
        print("\033[91mNot quite")
        print(f"We consider a success for this part to be {(error_rate * trials * 100.0) / trials}%")
        print("\033[0m")
        return False

def main():
    parser = argparse.ArgumentParser(description="Check your lab 2 code")
    parser.add_argument('part', type=str, help="Which part to check? 1, 2, or 3?")
    args = parser.parse_args()

    if args.part not in ['1','2','3', 'all']:
        print("error: unknown part")
        exit(-1)

    # Don't expect these hardcoded secrets to be the same when we grade!
    # Also, while this script will run forever, we will kill your autograder if it takes too long
    if args.part == '1' or args.part == 'all':
        print("Checking part 1...")
        check("part1", 100, b"MIT{k3rn3l_m3m0r135}", 0.95)
    if args.part == '2' or args.part == 'all':
        print("Checking part 2...")
        check("part2", 100, b"MIT{scary_sp3ctr3!}", 0.95)
    if args.part == '3' or args.part == 'all':
        print("Checking part 3...")
        check("part3", 5, b"MIT{h4rd3st}", 0.20)

if __name__=="__main__":
    main()
