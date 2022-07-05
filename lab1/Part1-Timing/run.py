import json
import os
import matplotlib.pyplot as plt
import subprocess
import csv
from tqdm import tqdm


# Run your code:
# Make sure that your code is using print_results_for_python
#executable_filename = ['make', 'run']

# Run reference code:
executable_filename = ['make', 'run-reference']

num_runs = 100

graph_repo = "data"
os.makedirs(graph_repo, exist_ok=True)

fancy_num_runs = range(0, num_runs, 1)
for run_id in tqdm(fancy_num_runs):
    p = subprocess.run(executable_filename,
            stdout=subprocess.PIPE, universal_newlines=True)

    reader = csv.reader(p.stdout.splitlines())

    dict_of_lists = dict()

    for store_lev in range(1, 4+1, 1):
        dict_of_lists[store_lev] = []

    store_level = 1
    for row in reader:
        temp = row[0].split(' ')[:-1]
        dict_of_lists[store_level] = list(map(int, temp)) 
        store_level+=1

    #print(dict_of_lists)
    filename = graph_repo+"/run"+str(run_id)+".json"
    jsonFile = open(filename, "w")
    jsonFile.write(json.dumps(dict_of_lists))
    jsonFile.close()

