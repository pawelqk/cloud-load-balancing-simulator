#! /usr/bin/python3

import json
import random
from datetime import datetime
import sys

# Workaround for working with large jsons
# See https://stackoverflow.com/questions/52137608/writing-a-large-json-array-to-file/52137753#52137753
class IteratorAsList(list):
    def __init__(self, it):
        self.it = it

    def __iter__(self):
        return self.it

    def __len__(self):
        return 1


def generate_random_node(config):
    return random.randint(config["minMips"], config["maxMips"])


def generate_random_task(config, max_task_mips):
    return {
        "requiredMips": random.randint(config["minMips"], max_task_mips),
        "length": random.randint(config["minLength"], config["maxLength"]),
        "arrivalTime": random.randint(0, config["maxArrivalTime"]),
    }


def generate_random_instances(config, number_of_instances):
    for instance_id in range(number_of_instances):
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

        yield {"id": instance_id, "nodesMips": random_nodes, "tasks": random_tasks}


def main():
    if len(sys.argv) == 1:
        print("Please provide number of instances")
        exit(1)

    number_of_instances = int(sys.argv[1])

    with open("config.json", "r") as file:
        config = json.load(file)

        random_instances = []

        current_time = datetime.today().strftime("%Y-%m-%d-%H:%M:%S")
        with open(f"instances_{current_time}.json", "w") as output:
            output.write(
                json.dumps(
                    IteratorAsList(generate_random_instances(config, number_of_instances)),
                    indent=4,
                    sort_keys=True,
                )
            )


if __name__ == "__main__":
    main()
