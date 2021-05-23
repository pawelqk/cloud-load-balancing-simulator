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


def build_penalty_factor_impact(results_path, algorithm_name, criteria):
    plt.figure(dpi=DPI)

    result_files = [
        filename
        for filename in os.listdir(results_path)
        if split_filename(filename)[0] == algorithm_name
    ]

    assert len(result_files) == 1

    result_file = result_files[0]
    algorithm_name, _ = split_filename(result_file)
    prettified_algorithm_name = prettify_algorithm_name(algorithm_name)

    df = pd.read_csv("/".join((results_path, result_file)), delimiter="|")
    results = []
    mean_criterias = (
        df.groupby("penalty_factor").mean().sort_values("penalty_factor").reset_index()
    )  # mean value of all instance ids

    mean_criterias.plot(x="penalty_factor", y=criteria, kind="line")
    plt.title(algorithm_name)
    plt.xlabel("Penalty factor")
    plt.ylabel(prettify_criteria(criteria))
    plt.savefig(
        "/".join(
            (
                results_path,
                criteria + algorithm_name + "penalty_factors.png",
            )
        )
    )


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
        penalty_factors = set(df["penalty_factor"])
        for penalty_factor in penalty_factors:
            mean_criteria_by_penalty_factor = (
                df[df["penalty_factor"] == penalty_factor]
                .groupby("instance_id")
                .mean()  # mean value of all instance ids
            )

            if penalty_factor not in results_dict:
                results_dict[penalty_factor] = {}

            results_dict[penalty_factor][
                prettified_algorithm_name
            ] = mean_criteria_by_penalty_factor[criteria]

    for penalty_factor in penalty_factors:
        boxplot = pd.DataFrame.from_dict(results_dict[penalty_factor]).boxplot(
            column=prettified_alg_names
        )

        plt.title(f"{algorithm_type} in {results_path}")
        plt.xlabel("Algorithm")
        plt.ylabel(prettify_criteria(criteria))
        plt.savefig(
            "/".join(
                (
                    results_path,
                    results_path.strip("/")
                    + criteria
                    + algorithm_type
                    + str(penalty_factor)
                    + ".png",
                )
            )
        )


def main():
    if len(sys.argv) == 2:
        print("Please provide path to results and alg type and criteria")
        exit(1)

    results_path = sys.argv[1]
    alg_type = sys.argv[2]
    # alg_name = sys.argv[2]
    criteria = sys.argv[3]
    build_boxplots(results_path, alg_type, criteria)
    # build_penalty_factor_impact(results_path, alg_name, criteria)


if __name__ == "__main__":
    main()
