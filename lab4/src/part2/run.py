import json
import os
import subprocess
import csv

# Change this line if you want to use the reference implementation!
executable_filename = './part2_1'
num_runs = 100

graph_repo = "data"
os.makedirs(graph_repo, exist_ok=True)

fancy_num_runs = range(0, num_runs, 1)
for run_id in fancy_num_runs:
    p = subprocess.run(['sudo', executable_filename],
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


