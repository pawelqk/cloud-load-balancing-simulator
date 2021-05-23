#! /usr/bin/python3

import random
from datetime import datetime
import json
import glob
import os
import shutil
import subprocess


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
MAX_SEED = 2 ** 32 - 1
NUMBER_OF_RUNS_PER_INSTANCE = 50


def read_algorithm_config(line_config):
    with open("/".join(("test_line", line_config)), "r") as line_config_file:
        return json.load(line_config_file)


def generate_run_config(seeds, algorithm_config):
    return {
        "seeds": seeds,
        "penaltyFactors": [0.2],
        "algorithms": algorithm_config,
    }


def read_generator_seed():
    with open("generator_config.json") as generator_config:
        config = json.load(generator_config)
        return config["seed"]


def create_dir_tree(line_results_dir):
    os.mkdir(line_results_dir)

    shutil.rmtree("results", ignore_errors=True)


def main():
    random.seed(read_generator_seed())
    seeds = [random.randint(0, MAX_SEED) for i in range(NUMBER_OF_RUNS_PER_INSTANCE)]

    line_configs = os.listdir("test_line")
    line_results_dir = "test_line_results-" + datetime.today().strftime(
        "%Y-%m-%d-%H:%M:%S"
    )

    create_dir_tree(line_results_dir)

    for line_config in line_configs:
        print(f"running for {line_config}")
        for number_of_tasks, number_of_nodes in TASKS_TO_NODES:
            print(
                f"running for number_of_tasks: {number_of_tasks}, number of nodes: {number_of_nodes}"
            )

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

            print(
                f"Getting instances/instances-{number_of_tasks}_{number_of_nodes}.json"
            )
            shutil.copyfile(
                f"instances/instances-{number_of_tasks}_{number_of_nodes}.json",
                "instances.json",
            )

            print("Running")
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
                        + str(number_of_tasks)
                        + "_"
                        + str(number_of_nodes),
                    )
                ),
            )


if __name__ == "__main__":
    main()
