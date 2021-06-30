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


def build_boxplots(
    min_instance_vals, results_path, algorithm_type, criteria, n, m, assessment
):
    plt.figure(dpi=DPI)

    print(algorithm_type)
    print(results_path)
    result_files = [
        filename
        for filename in os.listdir(results_path)
        if split_filename(filename)[1] == algorithm_type
    ]

    print(result_files)

    prettified_alg_names = []
    results_dict = {}
    penalty_factors = set()
    instance_ids = set()
    for result_file in result_files:
        algorithm_name, algorithm_type = split_filename(result_file)
        prettified_algorithm_name = prettify_algorithm_name(algorithm_name)

        prettified_alg_names.append(prettified_algorithm_name)

        df = pd.read_csv("/".join((results_path, result_file)), delimiter="|")
        criteria_value = {}
        for instance_id in df["instance_id"]:
            criteria_value[instance_id] = df[df["instance_id"] == instance_id][
                criteria
            ].values.tolist()
            instance_ids.add(instance_id)

        results_dict[prettified_algorithm_name] = criteria_value

    normalized_results_dict = {}
    for instance_id in instance_ids:
        min_instance_val = min_instance_vals[f"{n}-{m}"][str(instance_id)]

        def normalize(val):
            return (val - min_instance_val) / min_instance_val

        for alg, values in results_dict.items():
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
        flattened_normalized_results_dict[alg] = flattened_values

    flattened_results_dict = {}
    for alg, values in results_dict.items():
        flattened_values = []
        for instance_values in values.values():
            flattened_values.append(statistics.mean(instance_values))
        flattened_results_dict[alg] = flattened_values

    df = pd.DataFrame.from_dict(flattened_results_dict)
    with open("/".join((results_path, "latex_notnormalized_meaned")), "w") as latex_out:
        latex_out.write(
            df[sorted(df.columns, key=lambda x: COLUMN_ORDER[x])].to_latex(
                float_format="%.2f"
            )
        )

    df = pd.DataFrame.from_dict(flattened_normalized_results_dict)

    with open("/".join((results_path, "latex_normalized")), "w") as latex_out:
        latex_out.write(
            df[sorted(df.columns, key=lambda x: COLUMN_ORDER[x])].to_latex(
                float_format="%.4f"
            )
        )

    boxplot = df.boxplot(
        column=sorted(prettified_alg_names, key=lambda x: ALGORITHM_ORDER[x])
    )
    boxplot.grid(False)
    output_path = "/".join(
        (
            results_path,
            results_path.strip("/").replace(".", "")
            + criteria
            + algorithm_type
            + "-"
            + n
            + "_"
            + m
            + "-assessment_"
            + assessment
            + "-NEW.png",
        )
    )
    print(f"ploting to {output_path}")
    plt.title(f"n = {n}, m = {m}, kryterium {assessment}")
    plt.xlabel("Algorytm")
    plt.ylabel(f"PRD z wartości {criteria}")
    plt.savefig(output_path)


def main():
    if len(sys.argv) == 4:
        print(
            "Please provide path to results and alg type and criteria and n and m and path to min values"
        )
        exit(1)

    results_path = sys.argv[1]
    alg_type = sys.argv[2]
    criteria = sys.argv[3]
    n = sys.argv[4]
    m = sys.argv[5]
    min_instance_vals_path = sys.argv[6]

    with open(min_instance_vals_path, "rb") as f:
        min_instance_vals = pickle.load(f)
    assessment = criteria
    build_boxplots(
        min_instance_vals, results_path, alg_type, criteria, n, m, assessment
    )


if __name__ == "__main__":
    main()
