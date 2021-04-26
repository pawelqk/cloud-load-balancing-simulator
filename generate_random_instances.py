#! /usr/bin/python3

import json
import random
from datetime import datetime

TYPE = "online"
ADDITIONAL_ACTIONS = ["migrations, preemptions"]
NUMBER_OF_INSTANCES = 1


def generate_random_node(config):
    return random.randint(config["minMips"], config["maxMips"])


def generate_random_task(config, max_task_mips):
    return {
        "requiredMips": random.randint(config["minMips"], max_task_mips),
        "length": random.randint(config["minLength"], config["maxLength"]),
        "arrivalTime": random.randint(0, config["maxArrivalTime"])
    }


with open("config.json", 'r') as file:
    config = json.load(file)

    random_instances = []
    for instance_id in range(NUMBER_OF_INSTANCES):
        mips_of_nodes = set()

        random_nodes = []
        for task_id in range(config["numberOfNodes"]):
            random_node_mips = generate_random_node(config)
            mips_of_nodes.add(random_node_mips)
            random_nodes.append(random_node_mips)

        max_task_mips = max(mips_of_nodes)

        random_tasks = []
        for task_id in range(config["numberOfTasks"]):
            random_tasks.append(generate_random_task(config, max_task_mips))

        random_instances.append({
            "id": instance_id,
            "nodesMips": random_nodes,
            "type": TYPE,
            "additionalActions": ADDITIONAL_ACTIONS,
            "tasks": random_tasks
        })

    current_time = datetime.today().strftime('%Y-%m-%d-%H:%M:%S')
    with open(f"instances_{current_time}.json", 'w') as output:
        output.write(json.dumps(random_instances, indent=4, sort_keys=True))