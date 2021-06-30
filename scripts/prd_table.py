#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import os
import pickle
import json
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

ROW_ORDER = {
    "20-5": 0,
    "20-10": 1,
    "50-5": 2,
    "50-10": 3,
    "100-5": 4,
    "20-x": 5,
    "50-x": 6,
    "x-5": 7,
    "x-10": 8,
    "x-x": 9,
}

TRANSLATED_DIRNAMES = {
    "20-5": "(20, 5)",
    "20-10": "(20, 10)",
    "50-5": "(50, 5)",
    "50-10": "(50, 10)",
    "100-5": "(100, 5)",
    "20-x": "(20, *)",
    "50-x": "(50, *)",
    "x-5": "(*, 5)",
    "x-10": "(*, 10)",
    "x-x": "(*, *)",
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


def build_prd_table(min_instance_vals, directory_path, algorithm_type, criteria):
    plt.figure(dpi=DPI)

    all_alg_results = {}
    checked_directories = sorted(os.listdir(directory_path), key=lambda x: ROW_ORDER[x])
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

    prettified_alg_names = sorted(
        prettified_alg_names, key=lambda x: ALGORITHM_ORDER[x]
    )

    alg_final_results = {}
    for checked_directory in checked_directories:
        better_dir_name = TRANSLATED_DIRNAMES[checked_directory]
        if not better_dir_name in alg_final_results:
            alg_final_results[better_dir_name] = []

        for alg in prettified_alg_names:
            alg_results = []

            for all_alg_values in all_alg_results[checked_directory].values():
                alg_results.append(all_alg_values[alg])

            alg_final_results[better_dir_name].append(statistics.mean(alg_results))

    df = pd.DataFrame.from_dict(
        alg_final_results, orient="index", columns=prettified_alg_names
    )

    with open("latex_prd", "w") as file:
        file.write(df.to_latex(float_format="%.2f"))


def main():
    if len(sys.argv) == 4:
        print("Please provide path to results and alg type and criteria")
        exit(1)

    results_path = sys.argv[1]
    alg_type = sys.argv[2]
    criteria = sys.argv[3]
    min_instance_vals_path = sys.argv[4]

    with open(min_instance_vals_path, "rb") as f:
        min_instance_vals = pickle.load(f)

    build_prd_table(min_instance_vals, results_path, alg_type, criteria)


if __name__ == "__main__":
    main()
