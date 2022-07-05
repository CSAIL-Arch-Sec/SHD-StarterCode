import argparse
import json
import logging
import os
import sys
import threading
import time

from flask import Flask, send_from_directory
from selenium import webdriver
from selenium.webdriver.chrome.options import Options

# Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("--browser", type=str, choices=["chrome", "firefox", "safari"], default="chrome", help="Browser to run automation in.")
parser.add_argument("--domains", type=str, default="google.com,youtube.com,baidu.com,facebook.com", help="Comma-separated list of domain names to collect traces from. Defaults to google.com,youtube.com,baidu.com,facebook.com")
parser.add_argument("--enable_countermeasure", type=bool, default=False, help="Set to true to enable the countermeasure. Browser must be set to Chrome. Defaults to false.")
parser.add_argument("--num_traces_per_domain", type=int, default=40, help="Number of traces to collect per domain.")
parser.add_argument("--trace_length", type=int, default=5000, help="The length of each recorded trace, in milliseconds. Defaults to 5000.")

required = parser.add_argument_group("required arguments")
required.add_argument("--out_filename", type=str, required=True, help="Name of the output file to save traces to.")
required.add_argument("--part", type=int, choices=[2, 3, 4], required=True, help="Set to the part of the lab you're working on.")

opts = parser.parse_args()

if opts.browser != "chrome" and opts.enable_countermeasure:
    print("Browser must be set to Chrome in order to enable the countermeasure.")
    sys.exit(1)

if os.path.exists(opts.out_filename):
    print(f"WARNING: Data already exists at {opts.out_filename}. What do you want to do?")
    res = input("[C]ancel [O]verwrite ").lower()
    
    if res == "o":
        os.remove(opts.out_filename)
    else:
        sys.exit(1)

# Start serving attacker app
app = Flask(__name__)

# Disable Flask logs
os.environ["WERKZEUG_RUN_MAIN"] = "true"
log = logging.getLogger("werkzeug")
log.disabled = True

@app.route("/")
def root():
    return send_from_directory(f"part{opts.part}", "index.html")

@app.route("/<path:path>")
def static_dir(path):
    return send_from_directory(f"part{opts.part}", path)

flask_thread = threading.Thread(target=app.run, kwargs={"port": 1234})
flask_thread.setDaemon(True)
flask_thread.start()

# Setup
def get_browser(victim):
    if opts.browser == "chrome":
        chrome_opts = Options()
        chrome_opts.add_experimental_option("excludeSwitches", ["enable-automation"])

        if opts.enable_countermeasure and victim:
            # Victim has the extension enabled -- attacker does not
            chrome_opts.add_extension("part4/extension.crx")

        return webdriver.Chrome(options=chrome_opts)
    elif opts.browser == "firefox":
        return webdriver.Firefox()
    elif opts.browser == "safari":
        return webdriver.Safari()

attacker = get_browser(victim=False)
attacker.get("http://localhost:1234")
attacker.execute_script(f"window.trace_length = {opts.trace_length}")
attacker.execute_script(f"window.using_automation_script = true")

def collect_trace(url):
    victim = get_browser(victim=True)

    attacker.execute_script("collectTrace()")
    victim.get(url)

    time.sleep(float(opts.trace_length) / 1000)

    while attacker.execute_script("return recording"):
        time.sleep(0.1)

    victim.quit()
    return attacker.execute_script("return traces")[-1]

# Collect traces
urls = [f"https://www.{domain}" for domain in opts.domains.split(",")]
traces = []
labels = []

for url in urls:
    for i in range(opts.num_traces_per_domain):
        traces.append(collect_trace(url))
        labels.append(url)

with open(opts.out_filename, "w") as out:
    json.dump({
        "traces": traces,
        "labels": labels
    }, out, separators=(",", ":"))

attacker.quit()
