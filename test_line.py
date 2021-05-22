# for each given node count and task count perform test line:
# 1. generate 10-30 (?) instances
# 2. run all the algorithms from config for each instance changing the seed every single time

import random
import json
import shutil
import subprocess


def generate_config(seed):
    return {
        "seed": seed,
        "penaltyFactor": 0.2,
        "algorithms": [
            {
                "name": "Shortest remaining time first",
                "configuration": {
                    "type": "Online"
                }
            }
            # {
            #    "name": "Genetic algorithm",
            #    "configuration": {
            #        "type": "Offline",
            #        "mutationProbability": 0.05,
            #        "populationSize": 100,
            #        "maxIterations": 1000,
            #        "eliteIndividualsInInitialGenerationRatio": 0.05,
            #        "eliteIndividualsInNextGenerationRatio": 0.05,
            #        "criteria": "Makespan",
            #    },
            # }
        ],
        "minMips": 5,
        "maxMips": 100,
        "minLength": 10,
        "maxLength": 500,
        "numberOfNodes": 15,
        "numberOfTasks": 100,
        "maxArrivalTime": 20,
    }


MAX_SEED = 2 ** 32 - 1  # TODO: switch to fast 64 uint
NUMBER_OF_RUNS_PER_INSTANCE = 10
RESULTS_PATH = "/home/pakwasni/dev/masters/results"

seeds = [random.randint(0, MAX_SEED) for i in range(NUMBER_OF_RUNS_PER_INSTANCE)]

with open("config.json", "w") as config_file:
    json.dump(generate_config(seeds[1]), config_file, indent=4)

shutil.rmtree("results")
subprocess.run("./bin/cloud-load-balancing-simulator")
