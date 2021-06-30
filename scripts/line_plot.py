#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import os
import pickle
import statistics
import sys

ALGORITHM_NAMES = {
    "ArtificialBeeColony": "ABC",
    "FirstComeFirstServe": "FIFO",
    "GeneticAlgorithm": "GA",
    "LongestRemainingTimeFirst": "LRTF",
    "Random": "RAND",
    "RoundRobin": "RR",
    "ShortestElapsedTimeFirst": "SETF",
    "ShortestRemainingTimeFirst": "SRPT",
    "SimulatedAnnealing": "SA",
}

COLUMN_ORDER = {
    "instance_id": -1,
    "RAND": 0,
    "RR": 1,
    "FIFO": 2,
    "SRPT": 3,
    "SETF": 4,
    "LRTF": 5,
    "SA": 6,
    "ABC": 7,
    "GA": 8,
}

ALGORITHM_ORDER = {
    "RAND": 0,
    "RR": 1,
    "FIFO": 2,
    "SRPT": 3,
    "SETF": 4,
    "LRTF": 5,
    "SA": 6,
    "ABC": 7,
    "GA": 8,
}

POSSIBLE_ALGORITHM_TYPES = ("Offline", "OnlineWithMigrationsAndPreemptions", "Online")

DPI = 200


def split_filename(filename):
    type_and_data_idx = filename.find("-")
    data_idx = filename.find("_")

    return (
        filename[:type_and_data_idx],
        filename[type_and_data_idx + 1 : data_idx],
        filename[data_idx + 1 :],
    )


def prettify_algorithm_name(algorithm_name):
    return ALGORITHM_NAMES[algorithm_name]


def prettify_criteria(criteria):
    return criteria.capitalize()


def build_lineplot(
    min_instance_vals, directory_path, algorithm_type, criteria, assessment
):
    plt.figure(dpi=DPI)

    all_alg_results = {}
    checked_directories = os.listdir(directory_path)
    checked_directories.sort(key=int)
    prettified_alg_names = set()
    for checked_directory in checked_directories:
        print(checked_directory)
        result_files = [
            filename
            for filename in os.listdir(checked_directory)
            if split_filename(filename)[1] == algorithm_type
        ]

        results_dict = {}
        penalty_factors = set()
        instance_ids = set()
        instance_sizes = set()
        for result_file in result_files:
            algorithm_name, algorithm_type, instance_size = split_filename(result_file)
            prettified_algorithm_name = prettify_algorithm_name(algorithm_name)

            prettified_alg_names.add(prettified_algorithm_name)

            df = pd.read_csv("/".join((checked_directory, result_file)), delimiter="|")
            criteria_value = {}
            for instance_id in df["instance_id"]:
                criteria_value[instance_id] = df[df["instance_id"] == instance_id][
                    criteria
                ].values.tolist()
                instance_ids.add(instance_id)

            if not instance_size in results_dict:
                results_dict[instance_size] = {}
                instance_sizes.add(instance_size)

            results_dict[instance_size][prettified_algorithm_name] = criteria_value

        for instance_size in instance_sizes:
            normalized_results_dict = {}
            for instance_id in instance_ids:
                min_instance_val = min_instance_vals[instance_size][str(instance_id)]

                def normalize(val):
                    return (val - min_instance_val) / min_instance_val

                for alg, values in results_dict[instance_size].items():
                    if not alg in normalized_results_dict:
                        normalized_results_dict[alg] = {}
                    normalized_results_dict[alg][instance_id] = list(
                        map(normalize, values[instance_id])
                    )

            flattened_normalized_results_dict = {}
            for alg, values in normalized_results_dict.items():
                flattened_values = []
                for instance_values in values.values():
                    flattened_values.append(statistics.mean(instance_values))
                flattened_normalized_results_dict[alg] = statistics.mean(
                    flattened_values
                )

            if not checked_directory in all_alg_results:
                all_alg_results[checked_directory] = {}

            all_alg_results[checked_directory][
                instance_size
            ] = flattened_normalized_results_dict

    alg_final_results = {}
    for checked_directory in checked_directories:
        for alg in prettified_alg_names:
            alg_results = []
            if not alg in alg_final_results:
                alg_final_results[alg] = []

            for all_alg_values in all_alg_results[checked_directory].values():
                alg_results.append(all_alg_values[alg])

            alg_final_results[alg].append(statistics.mean(alg_results))

    output_path = "/".join(
        (
            directory_path,
            directory_path.strip("/").replace(".", "")
            + criteria
            + algorithm_type
            + "-assessment_"
            + assessment
            + ".png",
        )
    )
    for alg in sorted(prettified_alg_names, key=lambda x: ALGORITHM_ORDER[x]):
        print(f"plotting {alg}")
        plt.plot(checked_directories, alg_final_results[alg])

    lgd = plt.legend(
        sorted(prettified_alg_names, key=lambda x: ALGORITHM_ORDER[x]),
        ncol=1,
        bbox_to_anchor=(1.02, 1),
        borderaxespad=0,
    )
    print(f"ploting to {output_path}")
    plt.title(f"zależność jakości {criteria} od n")
    plt.xlabel("n")
    plt.ylabel(f"PRD z wartości {criteria}")
    plt.savefig(output_path, bbox_extra_artists=(lgd,), bbox_inches="tight")


def main():
    if len(sys.argv) == 3:
        print("Please provide path to results and alg type and criteria and assessment")
        exit(1)

    results_path = sys.argv[1]
    alg_type = sys.argv[2]
    criteria = sys.argv[3]
    assessment = criteria
    min_instance_vals_path = sys.argv[4]

    with open(min_instance_vals_path, "rb") as f:
        min_instance_vals = pickle.load(f)
    build_lineplot(min_instance_vals, results_path, alg_type, criteria, assessment)


if __name__ == "__main__":
    main()
