#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

ALGORITHM_NAMES = {
    "ArtificialBeeColony": "ABC",
    "FirstComeFirstServe": "FCFS",
    "GeneticAlgorithm": "GA",
    "LongestRemainingTimeFirst": "LRTF",
    "Random": "RAND",
    "RoundRobin": "RR",
    "ShortestElapsedTimeFirst": "SETF",
    "ShortestRemainingTimeFirst": "SRTF",
    "SimulatedAnnealing": "SA",
}

POSSIBLE_ALGORITHM_TYPES = ("Offline", "OnlineWithMigrationsAndPreemptions", "Online")

DPI = 400


def split_filename(filename):
    type_and_data_idx = filename.find("-")
    type_and_data = filename[type_and_data_idx + 1 :]

    data_idx = type_and_data.find("-")
    if data_idx == -1:
        alg_type = type_and_data
    else:
        alg_type = type_and_data[:data_idx]

    return (filename[:type_and_data_idx], alg_type)


def prettify_algorithm_name(algorithm_name):
    return ALGORITHM_NAMES[algorithm_name]


def prettify_criteria(criteria):
    return criteria.capitalize()


def build_boxplots(results_path, algorithm_type, criteria):
    plt.figure(dpi=DPI)

    result_files = [
        filename
        for filename in os.listdir(results_path)
        if split_filename(filename)[1] == algorithm_type
    ]

    prettified_alg_names = []
    results_dict = {}
    penalty_factors = set()
    print(result_files)
    for result_file in result_files:
        algorithm_name, algorithm_type = split_filename(result_file)
        prettified_algorithm_name = prettify_algorithm_name(algorithm_name)

        prettified_alg_names.append(prettified_algorithm_name)

        df = pd.read_csv("/".join((results_path, result_file)), delimiter="|")
        criteria_value = df[criteria]

        results_dict[prettified_algorithm_name] = criteria_value

    boxplot = pd.DataFrame.from_dict(results_dict).boxplot(column=prettified_alg_names)

    output_path = "/".join(
        (
            results_path,
            results_path.strip("/") + criteria + algorithm_type + ".png",
        )
    )
    print(f"ploting to {output_path}")
    plt.title(f"{algorithm_type}")
    plt.xlabel("Algorithm")
    plt.ylabel(prettify_criteria(criteria))
    plt.savefig(output_path)


def main():
    if len(sys.argv) == 2:
        print("Please provide path to results and alg type and criteria")
        exit(1)

    results_path = sys.argv[1]
    alg_type = sys.argv[2]
    criteria = sys.argv[3]
    build_boxplots(results_path, alg_type, criteria)


if __name__ == "__main__":
    main()
