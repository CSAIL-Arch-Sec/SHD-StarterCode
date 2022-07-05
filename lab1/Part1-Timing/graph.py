import os
import json
import matplotlib.pyplot as plt
import numpy as np
from tqdm import tqdm
from datetime import datetime

num_runs = 100
dict_of_dict_of_lists = dict()

graph_repo="data"
os.makedirs(graph_repo, exist_ok=True)

fancy_num_runs = range(0, num_runs, 1)
for run_id in tqdm(fancy_num_runs):
    filename = graph_repo+"/run"+str(run_id)+".json"
    with open(filename) as f:
        dict_of_dict_of_lists[run_id] = json.load(f)

l1_all  = []
l2_all  = []
l3_all  = []
mem_all = []

for run_id in tqdm(fancy_num_runs):
    l1_all  += dict_of_dict_of_lists[run_id]['1'] 
    l2_all  += dict_of_dict_of_lists[run_id]['2'] 
    l3_all  += dict_of_dict_of_lists[run_id]['3'] 
    mem_all += dict_of_dict_of_lists[run_id]['4']

#
# MAX 300
#
fig_all = plt.figure(figsize=(11.25, 7.5))
ax_all  = fig_all.add_subplot(1,1,1)
ax_all.set_xlabel("Access Time")
ax_all.set_ylabel("Number of Samples")

ax_all.hist(l1_all,  label="L1",   bins=np.arange(0, 300 ), alpha=0.5) 
ax_all.hist(l2_all,  label="L2",   bins=np.arange(0, 300 ), alpha=0.5) 
ax_all.hist(l3_all,  label="L3",   bins=np.arange(0, 300 ), alpha=0.5) 
ax_all.hist(mem_all, label="DRAM", bins=np.arange(0, 300 ), alpha=0.5) 
fig_all.legend()

os.makedirs("graphs", exist_ok=True)
now = datetime.now() 
date_time = now.strftime("%m:%d:%Y_%H:%M:%S")
fig_all.savefig(str("graphs/"+date_time+".pdf"))
plt.close(fig_all)
