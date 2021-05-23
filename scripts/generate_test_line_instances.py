#! /usr/bin/python3

import shutil
import os
import subprocess
import json
import glob

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

SEED = 856725922

NUMBER_OF_INSTANCES = 30


def calculate_max_arrival_time(number_of_tasks, number_of_nodes):
    return 0.2 * ((100 * number_of_tasks) / number_of_nodes)


def generate_generator_config(seed, max_arrival_time, number_of_tasks, number_of_nodes):
    return {
        "seed": seed,
        "minMips": 1,
        "maxMips": 9,
        "minLength": 1,
        "maxLength": 999,
        "numberOfNodes": number_of_nodes,
        "numberOfTasks": number_of_tasks,
        "maxArrivalTime": max_arrival_time,
    }


def generate_instances():
    shutil.rmtree("instances", ignore_errors=True)
    os.makedirs("instances")

    print("generating instances...")
    for number_of_tasks, number_of_nodes in TASKS_TO_NODES:
        with open("generator_config.json", "w") as run_config:
            max_arrival_time = calculate_max_arrival_time(
                number_of_tasks, number_of_nodes
            )
            json.dump(
                generate_generator_config(
                    SEED,
                    max_arrival_time,
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
            f"instances/instances-{number_of_tasks}_{number_of_nodes}.json",
        )


if __name__ == "__main__":
    generate_instances()
