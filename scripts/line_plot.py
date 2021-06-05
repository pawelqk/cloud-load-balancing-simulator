#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

DPI = 400


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


def split_filename(filename):
    type_and_data_idx = filename.find("-")
    type_and_data = filename[type_and_data_idx + 1 :]

    data_idx = type_and_data.find("-")

    return (
        filename[:type_and_data_idx],
        type_and_data[:data_idx],
        type_and_data[data_idx:],
    )


def prettify_algorithm_name(algorithm_name):
    return ALGORITHM_NAMES[algorithm_name]


def prettify_criteria(criteria):
    return criteria.capitalize()


# get value that is being compared from data
def prepare_data(alg_data):
    idx = alg_data.find("_")
    return float(alg_data[1:idx])


def build_line_plot(results_path, algorithm_name, criteria):
    plt.figure(figsize=(15, 4), dpi=DPI)

    result_files = [
        filename
        for filename in os.listdir(results_path)
        if split_filename(filename)[0] == algorithm_name
    ]

    alg_datas = []
    results = {}
    for result_file in result_files:
        algorithm_name, _, alg_data = split_filename(result_file)
        alg_data = prepare_data(alg_data)
        alg_datas.append(alg_data)
        prettified_algorithm_name = prettify_algorithm_name(algorithm_name)

        df = pd.read_csv("/".join((results_path, result_file)), delimiter="|")
        results[alg_data] = df[criteria].mean()

    lists = sorted(results.items())  # sorted by key, return a list of tuples

    x, y = zip(*lists)  # unpack a list of pairs into two tuples

    plt.plot(x, y)
    plt.title(algorithm_name)
    plt.xlabel("Population size")
    plt.ylabel(prettify_criteria(criteria))
    plt.savefig(
        "/".join(
            (
                results_path,
                criteria + algorithm_name + "penalty_factors.png",
            )
        )
    )


def main():
    if len(sys.argv) != 4:
        print("Please provide path to results and alg name and criteria")
        exit(1)

    results_path = sys.argv[1]
    alg_name = sys.argv[2]
    criteria = sys.argv[3]
    build_line_plot(results_path, alg_name, criteria)


if __name__ == "__main__":
    main()
