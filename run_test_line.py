#! /usr/bin/python3

import random
from datetime import datetime
import json
import glob
import os
import shutil
import subprocess


def read_algorithm_config(line_config):
    with open("/".join(("test_line", line_config)), "r") as line_config_file:
        return json.load(line_config_file)


def calculate_max_arrival_time(number_of_tasks, number_of_nodes):
    return 0.2 * ((100 * number_of_tasks) / number_of_nodes)


def generate_generator_config(max_arrival_time, number_of_tasks, number_of_nodes):
    return {
        "minMips": 1,
        "maxMips": 9,
        "minLength": 1,
        "maxLength": 999,
        "numberOfNodes": number_of_nodes,
        "numberOfTasks": number_of_tasks,
        "maxArrivalTime": max_arrival_time,
    }


def generate_run_config(seeds, algorithm_config):
    return {
        "seeds": seeds,
        "penaltyFactors": [0.2],
        "algorithms": algorithm_config,
    }


GENERATOR_SEED = 41267891

TASKS_TO_NODES = (
    (20, 5),
    (20, 10),
    (20, 20),
    (50, 5),
    (50, 10),
    (50, 20),
    (100, 5),
    (100, 10),
    (100, 20),
    (250, 5),
    (250, 10),
    (250, 20),
    (500, 5),
    (500, 10),
    (500, 20),
)


def create_dir_tree(line_results_dir):
    os.mkdir(line_results_dir)

    shutil.rmtree("results", ignore_errors=True)
    shutil.rmtree("instances", ignore_errors=True)
    os.makedirs("instances", exist_ok=True)


def generate_instances():
    print("generating instances...")
    for number_of_tasks, number_of_nodes in TASKS_TO_NODES:
        with open("generator_config.json", "w") as run_config:
            json.dump(
                generate_generator_config(
                    calculate_max_arrival_time(number_of_tasks, number_of_nodes),
                    number_of_tasks,
                    number_of_nodes,
                ),
                run_config,
                indent=4,
            )
        subprocess.run(
            ["./scripts/generate_random_instances.py", str(NUMBER_OF_INSTANCES)]
        )
        generated_instances_file = glob.glob("instances_*.json")[0]
        print(f"generated {generated_instances_file}")
        shutil.move(
            generated_instances_file,
            f"instances/instances_{number_of_tasks}_{number_of_nodes}.json",
        )


random.seed(GENERATOR_SEED)

MAX_SEED = 2 ** 32 - 1
NUMBER_OF_RUNS_PER_INSTANCE = 50
NUMBER_OF_INSTANCES = 50

seeds = [random.randint(0, MAX_SEED) for i in range(NUMBER_OF_RUNS_PER_INSTANCE)]

line_configs = os.listdir("test_line")

line_results_dir = "test_line_results-" + datetime.today().strftime("%Y-%m-%d-%H:%M:%S")

create_dir_tree(line_results_dir)
generate_instances()

for line_config in line_configs:
    print(f"running for {line_config}")
    for number_of_tasks, number_of_nodes in TASKS_TO_NODES:
        print(
            f"running for number_of_tasks: {number_of_tasks}, number of nodes: {number_of_nodes}"
        )
        max_arrival_time = int(
            calculate_max_arrival_time(number_of_tasks, number_of_nodes)
        )
        print(f"arrival time: {max_arrival_time}")
        with open("run_config.json", "w") as run_config:
            json.dump(
                generate_run_config(
                    seeds,
                    read_algorithm_config(line_config),
                ),
                run_config,
                indent=4,
            )

        try:
            os.remove("instances.json")
        except FileNotFoundError:
            pass

        print(f"Getting instances/instances_{number_of_tasks}_{number_of_nodes}.json")
        shutil.move(
            f"instances/instances_{number_of_tasks}_{number_of_nodes}.json",
            "instances.json",
        )

        print(f"Running")
        subprocess.run(
            ["./bin/cloud-load-balancing-simulator"],
        )
        run_results_dir = os.listdir("results")[0]
        shutil.move(
            "/".join(("results", run_results_dir)),
            "/".join(
                (
                    line_results_dir,
                    line_config[: line_config.find(".json")]
                    + "-"
                    + str(max_arrival_time)
                    + "_"
                    + str(number_of_tasks)
                    + "_"
                    + str(number_of_nodes),
                )
            ),
        )
