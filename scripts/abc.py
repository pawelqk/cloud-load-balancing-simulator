#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

DPI = 1200


def split_filename(filename):
    type_and_data_idx = filename.find("-")
    type_and_data = filename[type_and_data_idx + 1 :]

    data_idx = type_and_data.find("-")
    alg_type = type_and_data[:data_idx]
    alg_data = type_and_data[data_idx + 1 :]

    return (filename[:type_and_data_idx], alg_type, alg_data)


def split_data(alg_data):
    return alg_data.split("_")


def build_pop_size_impact(results_path, criteria):
    plt.figure(dpi=DPI)

    result_files = [
        filename
        for filename in os.listdir(results_path)
        if split_filename(filename)[0] == "ArtificialBeeColony"
    ]

    results = {}

    for result_file in result_files:
        algorithm_name, _, algorithm_data = split_filename(result_file)

        pop_size, max_iterations_without_change, max_iterations = split_data(
            algorithm_data
        )

        df = pd.read_csv("/".join((results_path, result_file)), delimiter="|")
        results[int(pop_size)] = df[criteria].mean()

    pop_sizes, criteria_vals = zip(*(sorted(results.items())))

    plt.plot(pop_sizes, criteria_vals)
    plt.savefig(
        "/".join(
            (
                results_path,
                criteria + "pop_size.png",
            )
        )
    )


def main():
    if len(sys.argv) == 2:
        print("Please provide path to results and alg type and criteria")
        exit(1)

    results_path = sys.argv[1]
    criteria = sys.argv[2]
    build_pop_size_impact(results_path, criteria)


if __name__ == "__main__":
    main()
